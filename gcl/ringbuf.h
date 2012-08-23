/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#ifndef GCL_RINGBUF_H
#define GCL_RINGBUF_H

#ifndef GCL_ERROR
#define GCL_ERROR(errnum, ...)
#endif

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GCL_RINGBUF_MINIMAL_CAPACITY    (15)
#define GCL_RINGBUF_INITIAL_CAPACITY    (15)
#define GCL_RINGBUF_GROWTH_FACTOR       (2)

#define _gcl_ringbuf_forward(buf, ptr) \
    (++(ptr) == (buf)->data_end ? (ptr) = (buf)->data : (ptr))

#define _gcl_ringbuf_for_each_ptr(ptr, buf) \
    for ((ptr) = (buf)->begin; \
         (ptr) != (buf)->end; \
         _gcl_ringbuf_forward(buf, ptr))

/* This is a dummy type used only during development. */
typedef struct { double x, y; } _T;

typedef struct _C _C_t;
typedef struct _C_pos _C_pos_t;
typedef struct _C_range _C_range_t;

struct _C_pos {
    struct _C *buf;
    _T *ptr;
};

struct _C_range {
    struct _C *buf;
    _T *begin;
    _T *end;
};

struct _C {
    _T *data;
    _T *data_end;
    _T *begin;
    _T *end;
    void (*destroy_elem)(_T);
};

/* This is a dummy macro used only during development. */
#define _funcspecs

_funcspecs bool __C_contiguous(struct _C *buf);
_funcspecs void __C_move_data(_T *begin, _T *end, _T *dest);
_funcspecs _T *__C_do_resize_grow(struct _C *buf, size_t n);
_funcspecs _T *init__C(struct _C *buf, size_t n, void (*destroy_elem)(_T));
_funcspecs void destroy__C(struct _C *buf);
_funcspecs size_t _C_length(_C_t *buf);
_funcspecs bool _C_empty(_C_t *buf);
_funcspecs size_t _C_capacity(_C_t *buf);
_funcspecs size_t _C_max_capacity(void);
_funcspecs _T *_C_reserve(_C_t *buf, size_t n);

_funcspecs bool __C_contiguous(struct _C *buf)
{
    return buf->end - buf->begin >= 0;
}

_funcspecs void __C_move_data(_T *begin, _T *end, _T *dest)
{
    assert(begin <= end);

    size_t n = (size_t) (end - begin);

    if (n > 0)
        memmove(dest, begin, n * sizeof(_T));
}

_funcspecs _T *__C_do_resize_grow(struct _C *buf, size_t n)
{
    assert(n >= _C_capacity(buf) && n <= _C_max_capacity());

    if (n < GCL_RINGBUF_MINIMAL_CAPACITY)
        n = GCL_RINGBUF_MINIMAL_CAPACITY;

    if (n == _C_capacity(buf))
        return buf->data;

    size_t old_cap = _C_capacity(buf);
    size_t begin_offset = (size_t) (buf->begin - buf->data);
    size_t end_offset = (size_t) (buf->end - buf->data);

    _T *data = realloc(buf->data, (n + 1) * sizeof(_T));

    if (!data) {
        GCL_ERROR(errno, "Reallocating memory for ring buffer failed");
        return NULL;
    }

    if (!__C_contiguous(buf)) {

        size_t left_part = end_offset;
        size_t right_part = old_cap - begin_offset;

        if (left_part < right_part && left_part <= n - old_cap) {
            __C_move_data(data, data + end_offset, data + old_cap);
            end_offset += old_cap;
        } else {
            __C_move_data(data + begin_offset, data + old_cap,
                          data + n - right_part);
            begin_offset += n - old_cap;
        }
    }

    buf->data = data;
    buf->data_end = data + n;
    buf->begin = data + begin_offset;
    buf->end = data + end_offset;

    return data;
}

_funcspecs _T *init__C(struct _C *buf, size_t n, void (*destroy_elem)(_T))
{
    if (n < GCL_RINGBUF_INITIAL_CAPACITY)
        n = GCL_RINGBUF_INITIAL_CAPACITY;

    _T *data = malloc((n + 1) * sizeof(_T));

    if (!data) {
        GCL_ERROR(errno, "Allocating memory for ring buffer failed");
        return NULL;
    }

    *buf = (struct _C) {
        .data = data,
        .data_end = data + n,
        .begin = data,
        .end = data,
        .destroy_elem = destroy_elem
    };

    return data;
}

_funcspecs void destroy__C(struct _C *buf)
{
    _T *ptr;

    if (buf->destroy_elem) {
        _gcl_ringbuf_for_each_ptr(ptr, buf)
            buf->destroy_elem(*ptr);
    }

    free(buf->data);
}

_funcspecs size_t _C_length(_C_t *buf)
{
    return __C_contiguous(buf) ?
        buf->end - buf->begin :
        buf->end - buf->begin + (buf->data_end - buf->data);
}

_funcspecs bool _C_empty(_C_t *buf)
{
    return buf->begin == buf->end;
}

_funcspecs size_t _C_capacity(_C_t *buf)
{
    return buf->data_end - buf->data - 1;
}

_funcspecs size_t _C_max_capacity(void)
{
    return SIZE_MAX / sizeof(_T) - 1;
}

_funcspecs _T *_C_reserve(_C_t *buf, size_t n)
{
    assert(n <= _C_max_capacity());

    if (n > _C_capacity(buf))
        return __C_do_resize_grow(buf, n);
    else
        return buf->data;
}

#endif

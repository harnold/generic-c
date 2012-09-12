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
#include <stddef.h>
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

_funcspecs _C_pos_t __C_pos(struct _C *buf, _T *ptr);
_funcspecs _C_range_t __C_range(struct _C *buf, _T *begin, _T *end);
_funcspecs void __C_ptr_inc(struct _C *buf, _T **ptr);
_funcspecs void __C_ptr_dec(struct _C *buf, _T **ptr);
_funcspecs _T *__C_ptr_add(struct _C *buf, _T *ptr, ptrdiff_t offset);
_funcspecs _T *__C_ptr_sub(struct _C *buf, _T *ptr, ptrdiff_t offset);
_funcspecs bool __C_valid_index(struct _C *buf, size_t i);
_funcspecs bool __C_valid_ptr(struct _C *buf, _T *ptr);
_funcspecs bool __C_valid_pos(struct _C *buf, struct _C_pos pos);
_funcspecs bool __C_ptr_in_left_part(struct _C *buf, _T *ptr);
_funcspecs bool __C_ptr_in_right_part(struct _C *buf, _T *ptr);
_funcspecs _T *__C_ptr_of_index(struct _C *buf, size_t i);
_funcspecs size_t __C_index_of_ptr(struct _C *buf, _T *ptr);
_funcspecs bool __C_contiguous(struct _C *buf);
_funcspecs bool __C_full(struct _C *buf);
_funcspecs void __C_move_data(_T *begin, _T *end, _T *dest);
_funcspecs _T *__C_do_resize_grow(struct _C *buf, size_t n);
_funcspecs _T *__C_grow(struct _C *buf);
_funcspecs _T *init__C(struct _C *buf, size_t n, void (*destroy_elem)(_T));
_funcspecs void destroy__C(struct _C *buf);
_funcspecs size_t _C_length(_C_t *buf);
_funcspecs bool _C_empty(_C_t *buf);
_funcspecs size_t _C_capacity(_C_t *buf);
_funcspecs size_t _C_max_capacity(void);
_funcspecs _T *_C_reserve(_C_t *buf, size_t n);
_funcspecs _T _C_front(_C_t *buf);
_funcspecs _T _C_back(_C_t *buf);
_funcspecs _T _C_at(_C_t *buf, size_t i);
_funcspecs _C_pos_t _C_begin(_C_t *buf);
_funcspecs _C_pos_t _C_end(_C_t *buf);
_funcspecs bool _C_at_begin(_C_t *buf, _C_pos_t pos);
_funcspecs bool _C_at_end(_C_t *buf, _C_pos_t pos);
_funcspecs _T _C_get(_C_pos_t pos);
_funcspecs _T *_C_get_ptr(_C_pos_t pos);
_funcspecs void _C_set(_C_pos_t pos, _T val);
_funcspecs _C_pos_t _C_next(_C_pos_t pos);
_funcspecs _C_pos_t _C_prev(_C_pos_t pos);
_funcspecs void _C_forward(_C_pos_t *pos);
_funcspecs void _C_backward(_C_pos_t *pos);
_funcspecs _C_range_t _C_all(_C_t *buf);
_funcspecs _C_range_t _C_range(_C_pos_t begin, _C_pos_t end);
_funcspecs _C_pos_t _C_range_begin(_C_range_t range);
_funcspecs _C_pos_t _C_range_end(_C_range_t range);
_funcspecs bool _C_range_at_begin(_C_range_t range, _C_pos_t pos);
_funcspecs bool _C_range_at_end(_C_range_t range, _C_pos_t pos);
_funcspecs _C_range_t _C_range_from_pos(_C_t *buf, _C_pos_t pos);
_funcspecs _C_range_t _C_range_to_pos(_C_t *buf, _C_pos_t pos);
_funcspecs size_t _C_range_length(_C_range_t range);
_funcspecs bool _C_range_empty(_C_range_t range);
_funcspecs _C_pos_t _C_insert(_C_t *buf, _C_pos_t pos, _T val);
_funcspecs _C_pos_t _C_insert_front(_C_t *buf, _T val);
_funcspecs _C_pos_t _C_insert_back(_C_t *buf, _T val);
_funcspecs _C_pos_t _C_release(_C_t *buf, _C_pos_t pos);
_funcspecs _C_pos_t _C_remove(_C_t *buf, _C_pos_t pos);
_funcspecs void _C_remove_front(_C_t *buf);
_funcspecs void _C_remove_back(_C_t *buf);
_funcspecs void _C_clear(_C_t *buf);

_funcspecs _C_pos_t __C_pos(struct _C *buf, _T *ptr)
{
    return (struct _C_pos) { .buf = buf, .ptr = ptr };
}

_funcspecs _C_range_t __C_range(struct _C *buf, _T *begin, _T *end)
{
    return (struct _C_range) { .buf = buf, .begin = begin, .end = end };
}

_funcspecs void __C_ptr_inc(struct _C *buf, _T **ptr)
{
    (*ptr)++;
    if (*ptr == buf->data_end)
        *ptr = buf->data;
}

_funcspecs void __C_ptr_dec(struct _C *buf, _T **ptr)
{
    if (*ptr == buf->data)
        *ptr = buf->data_end;
    (*ptr)--;
}

_funcspecs _T *__C_ptr_add(struct _C *buf, _T *ptr, ptrdiff_t offset)
{
    return ptr + (offset < buf->data_end - ptr ?
                  offset :
                  offset - (buf->data_end - buf->data));
}

_funcspecs _T *__C_ptr_sub(struct _C *buf, _T *ptr, ptrdiff_t offset)
{
    return ptr - (offset > ptr - buf->data ?
                  offset - (buf->data_end - buf->data) :
                  offset);
}

_funcspecs bool __C_valid_index(struct _C *buf, size_t i)
{
    return i < _C_length(buf);
}

_funcspecs bool __C_valid_ptr(struct _C *buf, _T *ptr)
{
    if (__C_contiguous(buf))
        return ptr >= buf->begin && ptr <= buf->end;
    else
        return (ptr >= buf->begin && ptr < buf->data_end)
            || (ptr >= buf->data && ptr <= buf->end);
}

_funcspecs bool __C_valid_pos(struct _C *buf, struct _C_pos pos)
{
    return pos.buf == buf && __C_valid_ptr(buf, pos.ptr);
}

_funcspecs bool __C_ptr_in_left_part(struct _C *buf, _T *ptr)
{
    assert(__C_valid_ptr(buf, ptr) && !__C_contiguous(buf));
    return ptr >= buf->data && ptr <= buf->end;
}

_funcspecs bool __C_ptr_in_right_part(struct _C *buf, _T *ptr)
{
    assert(__C_valid_ptr(buf, ptr) && !__C_contiguous(buf));
    return ptr >= buf->begin && ptr <= buf->data_end;
}

_funcspecs size_t __C_index_of_ptr(struct _C *buf, _T *ptr)
{
    assert(__C_valid_ptr(buf, ptr) && ptr != buf->end);
    return __C_contiguous(buf) || __C_ptr_in_right_part(buf, ptr) ?
        (ptr - buf->begin) :
        (ptr - buf->data) + (buf->data_end - buf->begin);
}

_funcspecs _T *__C_ptr_of_index(struct _C *buf, size_t i)
{
    assert(__C_valid_index(buf, i));
    return __C_ptr_add(buf, buf->begin, i);
}

_funcspecs bool __C_contiguous(struct _C *buf)
{
    return buf->end - buf->begin >= 0;
}

_funcspecs bool __C_full(struct _C *buf)
{
    ptrdiff_t d = buf->end - buf->begin;
    return (d >= 0 && d == _C_capacity(buf) - 1) || d == -1;
}

_funcspecs void __C_move_data(_T *begin, _T *end, _T *dest)
{
    assert(begin <= end);

    size_t n = (size_t) (end - begin);

    if (n > 0)
        memmove(dest, begin, n * sizeof(_T));
}

_funcspecs _T *__C_do_resize_shrink(struct _C *buf, size_t n)
{
    assert(n >= _C_length(buf) && n <= _C_capacity(buf));

    if (n < GCL_RINGBUF_MINIMAL_CAPACITY)
        n = GCL_RINGBUF_MINIMAL_CAPACITY;

    if (n == _C_capacity(buf))
        return buf->data;

    size_t begin = (size_t) (buf->begin - buf->data);
    size_t end = (size_t) (buf->end - buf->data);

    if (__C_contiguous(buf)) {
        if (end > n) {
            __C_move_data(buf->begin, buf->end, buf->data);
            begin = 0;
            end = (size_t) (buf->end - buf->begin);
        }
    } else {
        _T *new_begin = buf->data + (n + 1) - (buf->data_end - buf->begin);
        __C_move_data(buf->begin, buf->data_end, new_begin);
        begin = (size_t) (new_begin - buf->data);
    }

    _T *data = realloc(buf->data, (n + 1) * sizeof(_T));

    if (!data) {
        GCL_ERROR(errno, "Reallocating memory for ring buffer failed");
        return NULL;
    }

    buf->data = data;
    buf->data_end = data + (n + 1);
    buf->begin = data + begin;
    buf->end = data + end;

    return data;
}

_funcspecs _T *__C_do_resize_grow(struct _C *buf, size_t n)
{
    assert(n >= _C_capacity(buf) && n <= _C_max_capacity());

    if (n < GCL_RINGBUF_MINIMAL_CAPACITY)
        n = GCL_RINGBUF_MINIMAL_CAPACITY;

    if (n == _C_capacity(buf))
        return buf->data;

    size_t data_end = (size_t) (buf->data_end - buf->data);
    size_t begin = (size_t) (buf->begin - buf->data);
    size_t end = (size_t) (buf->end - buf->data);

    _T *data = realloc(buf->data, (n + 1) * sizeof(_T));

    if (!data) {
        GCL_ERROR(errno, "Reallocating memory for ring buffer failed");
        return NULL;
    }

    if (!__C_contiguous(buf)) {

        size_t left_part = end;
        size_t right_part = data_end - begin;
        size_t size_delta = (n + 1) - data_end;

        if (left_part < right_part && left_part < size_delta) {
            __C_move_data(data, data + end, data + data_end);
            end += data_end;
        } else {
            __C_move_data(data + begin, data + data_end,
                          data + (n + 1) - right_part);
            begin += size_delta;
        }
    }

    buf->data = data;
    buf->data_end = data + (n + 1);
    buf->begin = data + begin;
    buf->end = data + end;

    return data;
}

_funcspecs _T *__C_grow(struct _C *buf)
{
    size_t max_cap = _C_max_capacity();
    size_t old_cap = _C_capacity(buf);

    if (old_cap == max_cap)
        return NULL;

    size_t new_cap = (size_t) ((old_cap + 1) * GCL_RINGBUF_GROWTH_FACTOR) - 1;

    if (new_cap > max_cap)
        new_cap = max_cap;

    return __C_do_resize_grow(buf, new_cap);
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
        (buf->end - buf->begin) :
        (buf->end - buf->begin) + (buf->data_end - buf->data);
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
    return (size_t) (SIZE_MAX / (GCL_RINGBUF_GROWTH_FACTOR * sizeof(_T))) - 1;
}

_funcspecs _T *_C_reserve(_C_t *buf, size_t n)
{
    assert(n <= _C_max_capacity());

    if (n > _C_capacity(buf))
        return __C_do_resize_grow(buf, n);
    else
        return buf->data;
}

_funcspecs _T *_C_shrink(_C_t *buf)
{
    return __C_do_resize_shrink(buf, _C_length(buf));
}

_funcspecs _T _C_front(_C_t *buf)
{
    assert(!_C_empty(buf));
    return *buf->begin;
}

_funcspecs _T _C_back(_C_t *buf)
{
    assert(!_C_empty(buf));
    return *__C_ptr_sub(buf, buf->end, 1);
}

_funcspecs _T _C_at(_C_t *buf, size_t i)
{
    assert(__C_valid_index(buf, i));
    return *__C_ptr_add(buf, buf->begin, i);
}

_funcspecs _C_pos_t _C_begin(_C_t *buf)
{
    return __C_pos(buf, buf->begin);
}

_funcspecs _C_pos_t _C_end(_C_t *buf)
{
    return __C_pos(buf, buf->end);
}

_funcspecs bool _C_at_begin(_C_t *buf, _C_pos_t pos)
{
    return pos.ptr == buf->begin;
}

_funcspecs bool _C_at_end(_C_t *buf, _C_pos_t pos)
{
    return pos.ptr == buf->end;
}

_funcspecs _T _C_get(_C_pos_t pos)
{
    return *pos.ptr;
}

_funcspecs _T *_C_get_ptr(_C_pos_t pos)
{
    return pos.ptr;
}

_funcspecs void _C_set(_C_pos_t pos, _T val)
{
    *pos.ptr = val;
}

_funcspecs _C_pos_t _C_next(_C_pos_t pos)
{
    return __C_pos(pos.buf, __C_ptr_add(pos.buf, pos.ptr, 1));
}

_funcspecs _C_pos_t _C_prev(_C_pos_t pos)
{
    return __C_pos(pos.buf, __C_ptr_sub(pos.buf, pos.ptr, 1));
}

_funcspecs void _C_forward(_C_pos_t *pos)
{
    __C_ptr_inc(pos->buf, &pos->ptr);
}

_funcspecs void _C_backward(_C_pos_t *pos)
{
    __C_ptr_dec(pos->buf, &pos->ptr);
}

_funcspecs _C_range_t _C_all(_C_t *buf)
{
    return __C_range(buf, buf->begin, buf->end);
}

_funcspecs _C_range_t _C_range(_C_pos_t begin, _C_pos_t end)
{
    assert(begin.buf == end.buf);
    return __C_range(begin.buf, begin.ptr, end.ptr);
}

_funcspecs _C_pos_t _C_range_begin(_C_range_t range)
{
    return __C_pos(range.buf, range.begin);
}

_funcspecs _C_pos_t _C_range_end(_C_range_t range)
{
    return __C_pos(range.buf, range.end);
}

_funcspecs bool _C_range_at_begin(_C_range_t range, _C_pos_t pos)
{
    return pos.ptr == range.begin;
}

_funcspecs bool _C_range_at_end(_C_range_t range, _C_pos_t pos)
{
    return pos.ptr == range.end;
}

_funcspecs _C_range_t _C_range_from_pos(_C_t *buf, _C_pos_t pos)
{
    assert(__C_valid_pos(buf, pos));
    return __C_range(buf, pos.ptr, buf->end);
}

_funcspecs _C_range_t _C_range_to_pos(_C_t *buf, _C_pos_t pos)
{
    assert(__C_valid_pos(buf, pos));
    return __C_range(buf, buf->begin, pos.ptr);
}

_funcspecs size_t _C_range_length(_C_range_t range)
{
    return range.end - range.begin >= 0 ?
        (range.end - range.begin) :
        (range.end - range.begin) + (range.buf->data_end - range.buf->data);
}

_funcspecs bool _C_range_empty(_C_range_t range)
{
    return range.begin == range.end;
}

_funcspecs _C_pos_t _C_insert(_C_t *buf, _C_pos_t pos, _T val)
{
    assert(__C_valid_pos(buf, pos));

    if (__C_full(buf)) {
        size_t i = __C_index_of_ptr(buf, pos.ptr);
        if (!__C_grow(buf)) {
            GCL_ERROR(0, "Increasing ring buffer capacity failed");
            return __C_pos(buf, NULL);
        }
        pos.ptr = __C_ptr_of_index(buf, i);
    }

    assert(_C_capacity(buf) > _C_length(buf));

    bool shift_right = __C_contiguous(buf) ?
        buf->begin == buf->data || buf->end - pos.ptr < pos.ptr - buf->begin :
        __C_ptr_in_left_part(buf, pos.ptr);

    if (shift_right) {
        __C_move_data(pos.ptr, buf->end, pos.ptr + 1);
        *pos.ptr = val;
        __C_ptr_inc(buf, &buf->end);
    } else {
        __C_move_data(buf->begin, pos.ptr, buf->begin - 1);
        *(--pos.ptr) = val;
        __C_ptr_dec(buf, &buf->begin);
    }

    return pos;
}

_funcspecs _C_pos_t _C_insert_front(_C_t *buf, _T val)
{
    if (__C_full(buf)) {
        if (!__C_grow(buf)) {
            GCL_ERROR(0, "Increasing ring buffer capacity failed");
            return __C_pos(buf, NULL);
        }
    }

    assert(_C_capacity(buf) > _C_length(buf));

    __C_ptr_dec(buf, &buf->begin);
    *buf->begin = val;
    return __C_pos(buf, buf->begin);
}

_funcspecs _C_pos_t _C_insert_back(_C_t *buf, _T val)
{
    if (__C_full(buf)) {
        if (!__C_grow(buf)) {
            GCL_ERROR(0, "Increasing ring buffer capacity failed");
            return __C_pos(buf, NULL);
        }
    }

    assert(_C_capacity(buf) > _C_length(buf));

    *buf->end = val;
    __C_ptr_inc(buf, &buf->end);
    return __C_pos(buf, buf->end - 1);
}

_funcspecs _C_pos_t _C_release(_C_t *buf, _C_pos_t pos)
{
    assert(__C_valid_pos(buf, pos) && !_C_at_end(buf, pos));

    bool shift_right = __C_contiguous(buf) ?
        pos.ptr - buf->begin < buf->end - pos.ptr :
        __C_ptr_in_right_part(buf, pos.ptr);

    if (shift_right) {
        __C_move_data(buf->begin, pos.ptr, buf->begin + 1);
        __C_ptr_inc(buf, &buf->begin);
    } else {
        __C_move_data(pos.ptr + 1, buf->end, pos.ptr);
        __C_ptr_dec(buf, &buf->end);
    }

    return pos;
}

_funcspecs _C_pos_t _C_remove(_C_t *buf, _C_pos_t pos)
{
    assert(__C_valid_pos(buf, pos) && !_C_at_end(buf, pos));

    if (buf->destroy_elem)
        buf->destroy_elem(*pos.ptr);

    return _C_release(buf, pos);
}

_funcspecs void _C_remove_front(_C_t *buf)
{
    assert(!_C_empty(buf));
    _C_remove(buf, _C_begin(buf));
}

_funcspecs void _C_remove_back(_C_t *buf)
{
    assert(!_C_empty(buf));
    _C_remove(buf, _C_end(buf));
}

_funcspecs void _C_clear(_C_t *buf)
{
    _T *ptr;

    if (buf->destroy_elem) {
        _gcl_ringbuf_for_each_ptr(ptr, buf)
            buf->destroy_elem(*ptr);
    }

    buf->begin = buf->data;
    buf->end = buf->data;
}

#endif

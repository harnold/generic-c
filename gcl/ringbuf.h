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
#include <stdlib.h>

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

#endif

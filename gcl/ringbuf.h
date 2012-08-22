/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#ifndef GCL_RINGBUF_H
#define GCL_RINGBUF_H

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


#endif

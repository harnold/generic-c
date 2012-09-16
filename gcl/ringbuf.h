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

#define GCL_GENERATE_RINGBUF_TYPES(_C, _T) \
\
typedef struct _C _C##_t; \
typedef struct _C##_pos _C##_pos_t; \
typedef struct _C##_range _C##_range_t; \
\
struct _C##_pos { \
    struct _C *buf; \
    _T *ptr; \
}; \
\
struct _C##_range { \
    struct _C *buf; \
    _T *begin; \
    _T *end; \
}; \
\
struct _C { \
    _T *data; \
    _T *data_end; \
    _T *begin; \
    _T *end; \
    void (*destroy_elem)(_T); \
};

#define GCL_GENERATE_RINGBUF_FUNCTIONS_STATIC(_C, _T) \
    GCL_GENERATE_RINGBUF_LONG_FUNCTION_DECLS(_C, _T, static) \
    GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DECLS(_C, _T, static inline) \
    GCL_GENERATE_RINGBUF_LONG_FUNCTION_DEFS(_C, _T, static) \
    GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DEFS(_C, _T, static inline)

#define GCL_GENERATE_RINGBUF_FUNCTIONS_EXTERN_H(_C, _T) \
    GCL_GENERATE_RINGBUF_LONG_FUNCTION_DECLS(_C, _T, ) \
    GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DECLS(_C, _T, inline) \
    GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DEFS(_C, _T, inline)

#define GCL_GENERATE_RINGBUF_FUNCTIONS_EXTERN_C(_C, _T) \
    GCL_GENERATE_RINGBUF_LONG_FUNCTION_DEFS(_C, _T, ) \
    GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_RINGBUF_LONG_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs _T *_##_C##_do_resize_shrink(struct _C *buf, size_t n); \
_funcspecs _T *_##_C##_do_resize_grow(struct _C *buf, size_t n); \
_funcspecs _T *_##_C##_grow(struct _C *buf); \
_funcspecs _T *init__C(struct _C *buf, size_t n, void (*destroy_elem)(_T)); \
_funcspecs void destroy__C(struct _C *buf); \
_funcspecs _C##_pos_t _C##_insert(_C##_t *buf, _C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *buf, _T val); \
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *buf, _T val); \
_funcspecs _C##_pos_t _C##_release(_C##_t *buf, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_remove(_C##_t *buf, _C##_pos_t pos); \
_funcspecs void _C##_clear(_C##_t *buf);

#define GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs _C##_pos_t _##_C##_pos(struct _C *buf, _T *ptr); \
_funcspecs _C##_range_t _##_C##_range(struct _C *buf, _T *begin, _T *end); \
_funcspecs void _##_C##_ptr_inc(struct _C *buf, _T **ptr); \
_funcspecs void _##_C##_ptr_dec(struct _C *buf, _T **ptr); \
_funcspecs _T *_##_C##_ptr_add(struct _C *buf, _T *ptr, ptrdiff_t offset); \
_funcspecs _T *_##_C##_ptr_sub(struct _C *buf, _T *ptr, ptrdiff_t offset); \
_funcspecs bool _##_C##_valid_index(struct _C *buf, size_t i); \
_funcspecs bool _##_C##_valid_ptr(struct _C *buf, _T *ptr); \
_funcspecs bool _##_C##_valid_pos(struct _C *buf, struct _C##_pos pos); \
_funcspecs bool _##_C##_ptr_in_left_part(struct _C *buf, _T *ptr); \
_funcspecs bool _##_C##_ptr_in_right_part(struct _C *buf, _T *ptr); \
_funcspecs _T *_##_C##_ptr_of_index(struct _C *buf, size_t i); \
_funcspecs size_t _##_C##_index_of_ptr(struct _C *buf, _T *ptr); \
_funcspecs bool _##_C##_contiguous(struct _C *buf); \
_funcspecs bool _##_C##_full(struct _C *buf); \
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest); \
_funcspecs size_t _C##_length(_C##_t *buf); \
_funcspecs bool _C##_empty(_C##_t *buf); \
_funcspecs size_t _C##_capacity(_C##_t *buf); \
_funcspecs size_t _C##_max_capacity(void); \
_funcspecs _T *_C##_reserve(_C##_t *buf, size_t n); \
_funcspecs _T *_C##_shrink(_C##_t *buf); \
_funcspecs _T _C##_front(_C##_t *buf); \
_funcspecs _T _C##_back(_C##_t *buf); \
_funcspecs _T _C##_at(_C##_t *buf, size_t i); \
_funcspecs _C##_pos_t _C##_begin(_C##_t *buf); \
_funcspecs _C##_pos_t _C##_end(_C##_t *buf); \
_funcspecs bool _C##_at_begin(_C##_t *buf, _C##_pos_t pos); \
_funcspecs bool _C##_at_end(_C##_t *buf, _C##_pos_t pos); \
_funcspecs _T _C##_get(_C##_pos_t pos); \
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos); \
_funcspecs void _C##_set(_C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos); \
_funcspecs void _C##_forward(_C##_pos_t *pos); \
_funcspecs void _C##_backward(_C##_pos_t *pos); \
_funcspecs _C##_range_t _C##_all(_C##_t *buf); \
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end); \
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range); \
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range); \
_funcspecs bool _C##_range_at_begin(_C##_range_t range, _C##_pos_t pos); \
_funcspecs bool _C##_range_at_end(_C##_range_t range, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *buf, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *buf, _C##_pos_t pos); \
_funcspecs size_t _C##_range_length(_C##_range_t range); \
_funcspecs bool _C##_range_empty(_C##_range_t range); \
_funcspecs void _C##_remove_front(_C##_t *buf); \
_funcspecs void _C##_remove_back(_C##_t *buf);

#define GCL_GENERATE_RINGBUF_LONG_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs bool _##_C##_valid_ptr(struct _C *buf, _T *ptr) \
{ \
    if (_##_C##_contiguous(buf)) \
        return ptr >= buf->begin && ptr <= buf->end; \
    else \
        return (ptr >= buf->begin && ptr < buf->data_end) \
            || (ptr >= buf->data && ptr <= buf->end); \
} \
\
_funcspecs _T *_##_C##_do_resize_shrink(struct _C *buf, size_t n) \
{ \
    assert(n >= _C##_length(buf) && n <= _C##_capacity(buf)); \
\
    if (n < GCL_RINGBUF_MINIMAL_CAPACITY) \
        n = GCL_RINGBUF_MINIMAL_CAPACITY; \
\
    if (n == _C##_capacity(buf)) \
        return buf->data; \
\
    size_t begin = (size_t) (buf->begin - buf->data); \
    size_t end = (size_t) (buf->end - buf->data); \
\
    if (_##_C##_contiguous(buf)) { \
        if (end > n) { \
            _##_C##_move_data(buf->begin, buf->end, buf->data); \
            begin = 0; \
            end = (size_t) (buf->end - buf->begin); \
        } \
    } else { \
        _T *new_begin = buf->data + (n + 1) - (buf->data_end - buf->begin); \
        _##_C##_move_data(buf->begin, buf->data_end, new_begin); \
        begin = (size_t) (new_begin - buf->data); \
    } \
\
    _T *data = realloc(buf->data, (n + 1) * sizeof(_T)); \
\
    if (!data) { \
        GCL_ERROR(errno, "Reallocating memory for ring buffer failed"); \
        return NULL; \
    } \
\
    buf->data = data; \
    buf->data_end = data + (n + 1); \
    buf->begin = data + begin; \
    buf->end = data + end; \
\
    return data; \
} \
\
_funcspecs _T *_##_C##_do_resize_grow(struct _C *buf, size_t n) \
{ \
    assert(n >= _C##_capacity(buf) && n <= _C##_max_capacity()); \
\
    if (n < GCL_RINGBUF_MINIMAL_CAPACITY) \
        n = GCL_RINGBUF_MINIMAL_CAPACITY; \
\
    if (n == _C##_capacity(buf)) \
        return buf->data; \
\
    size_t data_end = (size_t) (buf->data_end - buf->data); \
    size_t begin = (size_t) (buf->begin - buf->data); \
    size_t end = (size_t) (buf->end - buf->data); \
\
    _T *data = realloc(buf->data, (n + 1) * sizeof(_T)); \
\
    if (!data) { \
        GCL_ERROR(errno, "Reallocating memory for ring buffer failed"); \
        return NULL; \
    } \
\
    if (!_##_C##_contiguous(buf)) { \
\
        size_t left_part = end; \
        size_t right_part = data_end - begin; \
        size_t size_delta = (n + 1) - data_end; \
\
        if (left_part < right_part && left_part < size_delta) { \
            _##_C##_move_data(data, data + end, data + data_end); \
            end += data_end; \
        } else { \
            _##_C##_move_data(data + begin, data + data_end, \
                          data + (n + 1) - right_part); \
            begin += size_delta; \
        } \
    } \
\
    buf->data = data; \
    buf->data_end = data + (n + 1); \
    buf->begin = data + begin; \
    buf->end = data + end; \
\
    return data; \
} \
\
_funcspecs _T *_##_C##_grow(struct _C *buf) \
{ \
    size_t max_cap = _C##_max_capacity(); \
    size_t old_cap = _C##_capacity(buf); \
\
    if (old_cap == max_cap) \
        return NULL; \
\
    size_t new_cap = (size_t) ((old_cap + 1) * GCL_RINGBUF_GROWTH_FACTOR) - 1; \
\
    if (new_cap > max_cap) \
        new_cap = max_cap; \
\
    return _##_C##_do_resize_grow(buf, new_cap); \
} \
\
_funcspecs _T *init__C(struct _C *buf, size_t n, void (*destroy_elem)(_T)) \
{ \
    if (n < GCL_RINGBUF_INITIAL_CAPACITY) \
        n = GCL_RINGBUF_INITIAL_CAPACITY; \
\
    _T *data = malloc((n + 1) * sizeof(_T)); \
\
    if (!data) { \
        GCL_ERROR(errno, "Allocating memory for ring buffer failed"); \
        return NULL; \
    } \
\
    *buf = (struct _C) { \
        .data = data, \
        .data_end = data + n, \
        .begin = data, \
        .end = data, \
        .destroy_elem = destroy_elem \
    }; \
\
    return data; \
} \
\
_funcspecs void destroy__C(struct _C *buf) \
{ \
    _T *ptr; \
\
    if (buf->destroy_elem) { \
        _gcl_ringbuf_for_each_ptr(ptr, buf) \
            buf->destroy_elem(*ptr); \
    } \
\
    free(buf->data); \
} \
\
_funcspecs _C##_pos_t _C##_insert(_C##_t *buf, _C##_pos_t pos, _T val) \
{ \
    assert(_##_C##_valid_pos(buf, pos)); \
\
    if (_##_C##_full(buf)) { \
        size_t i = _##_C##_index_of_ptr(buf, pos.ptr); \
        if (!_##_C##_grow(buf)) { \
            GCL_ERROR(0, "Increasing ring buffer capacity failed"); \
            return _##_C##_pos(buf, NULL); \
        } \
        pos.ptr = _##_C##_ptr_of_index(buf, i); \
    } \
\
    assert(_C##_capacity(buf) > _C##_length(buf)); \
\
    bool shift_right = _##_C##_contiguous(buf) ? \
        buf->begin == buf->data || buf->end - pos.ptr < pos.ptr - buf->begin : \
        _##_C##_ptr_in_left_part(buf, pos.ptr); \
\
    if (shift_right) { \
        _##_C##_move_data(pos.ptr, buf->end, pos.ptr + 1); \
        *pos.ptr = val; \
        _##_C##_ptr_inc(buf, &buf->end); \
    } else { \
        _##_C##_move_data(buf->begin, pos.ptr, buf->begin - 1); \
        *(--pos.ptr) = val; \
        _##_C##_ptr_dec(buf, &buf->begin); \
    } \
\
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *buf, _T val) \
{ \
    if (_##_C##_full(buf)) { \
        if (!_##_C##_grow(buf)) { \
            GCL_ERROR(0, "Increasing ring buffer capacity failed"); \
            return _##_C##_pos(buf, NULL); \
        } \
    } \
\
    assert(_C##_capacity(buf) > _C##_length(buf)); \
\
    _##_C##_ptr_dec(buf, &buf->begin); \
    *buf->begin = val; \
    return _##_C##_pos(buf, buf->begin); \
} \
\
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *buf, _T val) \
{ \
    if (_##_C##_full(buf)) { \
        if (!_##_C##_grow(buf)) { \
            GCL_ERROR(0, "Increasing ring buffer capacity failed"); \
            return _##_C##_pos(buf, NULL); \
        } \
    } \
\
    assert(_C##_capacity(buf) > _C##_length(buf)); \
\
    *buf->end = val; \
    _##_C##_ptr_inc(buf, &buf->end); \
    return _##_C##_pos(buf, buf->end - 1); \
} \
\
_funcspecs _C##_pos_t _C##_release(_C##_t *buf, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(buf, pos) && !_C##_at_end(buf, pos)); \
\
    bool shift_right = _##_C##_contiguous(buf) ? \
        pos.ptr - buf->begin < buf->end - pos.ptr : \
        _##_C##_ptr_in_right_part(buf, pos.ptr); \
\
    if (shift_right) { \
        _##_C##_move_data(buf->begin, pos.ptr, buf->begin + 1); \
        _##_C##_ptr_inc(buf, &buf->begin); \
    } else { \
        _##_C##_move_data(pos.ptr + 1, buf->end, pos.ptr); \
        _##_C##_ptr_dec(buf, &buf->end); \
    } \
\
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_remove(_C##_t *buf, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(buf, pos) && !_C##_at_end(buf, pos)); \
\
    if (buf->destroy_elem) \
        buf->destroy_elem(*pos.ptr); \
\
    return _C##_release(buf, pos); \
} \
\
_funcspecs void _C##_clear(_C##_t *buf) \
{ \
    _T *ptr; \
\
    if (buf->destroy_elem) { \
        _gcl_ringbuf_for_each_ptr(ptr, buf) \
            buf->destroy_elem(*ptr); \
    } \
\
    buf->begin = buf->data; \
    buf->end = buf->data; \
}

#define GCL_GENERATE_RINGBUF_SHORT_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs _C##_pos_t _##_C##_pos(struct _C *buf, _T *ptr) \
{ \
    return (struct _C##_pos) { .buf = buf, .ptr = ptr }; \
} \
\
_funcspecs _C##_range_t _##_C##_range(struct _C *buf, _T *begin, _T *end) \
{ \
    return (struct _C##_range) { .buf = buf, .begin = begin, .end = end }; \
} \
\
_funcspecs void _##_C##_ptr_inc(struct _C *buf, _T **ptr) \
{ \
    (*ptr)++; \
    if (*ptr == buf->data_end) \
        *ptr = buf->data; \
} \
\
_funcspecs void _##_C##_ptr_dec(struct _C *buf, _T **ptr) \
{ \
    if (*ptr == buf->data) \
        *ptr = buf->data_end; \
    (*ptr)--; \
} \
\
_funcspecs _T *_##_C##_ptr_add(struct _C *buf, _T *ptr, ptrdiff_t offset) \
{ \
    return ptr + (offset < buf->data_end - ptr ? \
                  offset : \
                  offset - (buf->data_end - buf->data)); \
} \
\
_funcspecs _T *_##_C##_ptr_sub(struct _C *buf, _T *ptr, ptrdiff_t offset) \
{ \
    return ptr - (offset > ptr - buf->data ? \
                  offset - (buf->data_end - buf->data) : \
                  offset); \
} \
\
_funcspecs bool _##_C##_valid_index(struct _C *buf, size_t i) \
{ \
    return i < _C##_length(buf); \
} \
\
_funcspecs bool _##_C##_valid_pos(struct _C *buf, struct _C##_pos pos) \
{ \
    return pos.buf == buf && _##_C##_valid_ptr(buf, pos.ptr); \
} \
\
_funcspecs bool _##_C##_ptr_in_left_part(struct _C *buf, _T *ptr) \
{ \
    assert(_##_C##_valid_ptr(buf, ptr) && !_##_C##_contiguous(buf)); \
    return ptr >= buf->data && ptr <= buf->end; \
} \
\
_funcspecs bool _##_C##_ptr_in_right_part(struct _C *buf, _T *ptr) \
{ \
    assert(_##_C##_valid_ptr(buf, ptr) && !_##_C##_contiguous(buf)); \
    return ptr >= buf->begin && ptr <= buf->data_end; \
} \
\
_funcspecs size_t _##_C##_index_of_ptr(struct _C *buf, _T *ptr) \
{ \
    assert(_##_C##_valid_ptr(buf, ptr) && ptr != buf->end); \
    return _##_C##_contiguous(buf) || _##_C##_ptr_in_right_part(buf, ptr) ? \
        (ptr - buf->begin) : \
        (ptr - buf->data) + (buf->data_end - buf->begin); \
} \
\
_funcspecs _T *_##_C##_ptr_of_index(struct _C *buf, size_t i) \
{ \
    assert(_##_C##_valid_index(buf, i)); \
    return _##_C##_ptr_add(buf, buf->begin, i); \
} \
\
_funcspecs bool _##_C##_contiguous(struct _C *buf) \
{ \
    return buf->end - buf->begin >= 0; \
} \
\
_funcspecs bool _##_C##_full(struct _C *buf) \
{ \
    ptrdiff_t d = buf->end - buf->begin; \
    return (d >= 0 && d == _C##_capacity(buf) - 1) || d == -1; \
} \
\
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest) \
{ \
    if (begin < end) \
        memmove(dest, begin, (end - begin) * sizeof(_T)); \
} \
\
_funcspecs size_t _C##_length(_C##_t *buf) \
{ \
    return _##_C##_contiguous(buf) ? \
        (buf->end - buf->begin) : \
        (buf->end - buf->begin) + (buf->data_end - buf->data); \
} \
\
_funcspecs bool _C##_empty(_C##_t *buf) \
{ \
    return buf->begin == buf->end; \
} \
\
_funcspecs size_t _C##_capacity(_C##_t *buf) \
{ \
    return buf->data_end - buf->data - 1; \
} \
\
_funcspecs size_t _C##_max_capacity(void) \
{ \
    return (size_t) (SIZE_MAX / (GCL_RINGBUF_GROWTH_FACTOR * sizeof(_T))) - 1; \
} \
\
_funcspecs _T *_C##_reserve(_C##_t *buf, size_t n) \
{ \
    assert(n <= _C##_max_capacity()); \
\
    if (n > _C##_capacity(buf)) \
        return _##_C##_do_resize_grow(buf, n); \
    else \
        return buf->data; \
} \
\
_funcspecs _T *_C##_shrink(_C##_t *buf) \
{ \
    return _##_C##_do_resize_shrink(buf, _C##_length(buf)); \
} \
\
_funcspecs _T _C##_front(_C##_t *buf) \
{ \
    assert(!_C##_empty(buf)); \
    return *buf->begin; \
} \
\
_funcspecs _T _C##_back(_C##_t *buf) \
{ \
    assert(!_C##_empty(buf)); \
    return *_##_C##_ptr_sub(buf, buf->end, 1); \
} \
\
_funcspecs _T _C##_at(_C##_t *buf, size_t i) \
{ \
    assert(_##_C##_valid_index(buf, i)); \
    return *_##_C##_ptr_add(buf, buf->begin, i); \
} \
\
_funcspecs _C##_pos_t _C##_begin(_C##_t *buf) \
{ \
    return _##_C##_pos(buf, buf->begin); \
} \
\
_funcspecs _C##_pos_t _C##_end(_C##_t *buf) \
{ \
    return _##_C##_pos(buf, buf->end); \
} \
\
_funcspecs bool _C##_at_begin(_C##_t *buf, _C##_pos_t pos) \
{ \
    return pos.ptr == buf->begin; \
} \
\
_funcspecs bool _C##_at_end(_C##_t *buf, _C##_pos_t pos) \
{ \
    return pos.ptr == buf->end; \
} \
\
_funcspecs _T _C##_get(_C##_pos_t pos) \
{ \
    return *pos.ptr; \
} \
\
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos) \
{ \
    return pos.ptr; \
} \
\
_funcspecs void _C##_set(_C##_pos_t pos, _T val) \
{ \
    *pos.ptr = val; \
} \
\
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos) \
{ \
    return _##_C##_pos(pos.buf, _##_C##_ptr_add(pos.buf, pos.ptr, 1)); \
} \
\
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos) \
{ \
    return _##_C##_pos(pos.buf, _##_C##_ptr_sub(pos.buf, pos.ptr, 1)); \
} \
\
_funcspecs void _C##_forward(_C##_pos_t *pos) \
{ \
    _##_C##_ptr_inc(pos->buf, &pos->ptr); \
} \
\
_funcspecs void _C##_backward(_C##_pos_t *pos) \
{ \
    _##_C##_ptr_dec(pos->buf, &pos->ptr); \
} \
\
_funcspecs _C##_range_t _C##_all(_C##_t *buf) \
{ \
    return _##_C##_range(buf, buf->begin, buf->end); \
} \
\
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end) \
{ \
    assert(begin.buf == end.buf); \
    return _##_C##_range(begin.buf, begin.ptr, end.ptr); \
} \
\
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range) \
{ \
    return _##_C##_pos(range.buf, range.begin); \
} \
\
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range) \
{ \
    return _##_C##_pos(range.buf, range.end); \
} \
\
_funcspecs bool _C##_range_at_begin(_C##_range_t range, _C##_pos_t pos) \
{ \
    return pos.ptr == range.begin; \
} \
\
_funcspecs bool _C##_range_at_end(_C##_range_t range, _C##_pos_t pos) \
{ \
    return pos.ptr == range.end; \
} \
\
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *buf, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(buf, pos)); \
    return _##_C##_range(buf, pos.ptr, buf->end); \
} \
\
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *buf, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(buf, pos)); \
    return _##_C##_range(buf, buf->begin, pos.ptr); \
} \
\
_funcspecs size_t _C##_range_length(_C##_range_t range) \
{ \
    return range.end - range.begin >= 0 ? \
        (range.end - range.begin) : \
        (range.end - range.begin) + (range.buf->data_end - range.buf->data); \
} \
\
_funcspecs bool _C##_range_empty(_C##_range_t range) \
{ \
    return range.begin == range.end; \
} \
\
_funcspecs void _C##_remove_front(_C##_t *buf) \
{ \
    assert(!_C##_empty(buf)); \
    _C##_remove(buf, _C##_begin(buf)); \
} \
\
_funcspecs void _C##_remove_back(_C##_t *buf) \
{ \
    assert(!_C##_empty(buf)); \
    _C##_remove(buf, _C##_end(buf)); \
}

#endif

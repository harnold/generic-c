/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#ifndef GCL_VECTOR_H
#define GCL_VECTOR_H

#include <gcl/malloc.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define GCL_VECTOR_MINIMAL_CAPACITY     (16)
#define GCL_VECTOR_INITIAL_CAPACITY     (16)
#define GCL_VECTOR_GROWTH_FACTOR        (2)

#define _gcl_vector_for_each_pos(pos, vec) \
    for ((pos) = (vec)->data; \
         (pos) != (vec)->end; \
         (pos)++)

#define GCL_GENERATE_VECTOR_TYPES(_C, _T) \
\
typedef struct _C _C##_t; \
typedef _T *_C##_pos_t; \
typedef struct _C##_range _C##_range_t; \
typedef _T _C##_elem_t; \
\
struct _C##_range { \
    _T *begin; \
    _T *end; \
}; \
\
struct _C { \
    _T *data; \
    _T *data_end; \
    _T *end; \
    void (*destroy_elem)(_T); \
};

#define GCL_GENERATE_VECTOR_FUNCTIONS_STATIC(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, static) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, static inline) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, static) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, static inline)

#define GCL_GENERATE_VECTOR_FUNCTIONS_EXTERN_H(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, ) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, inline) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, inline)

#define GCL_GENERATE_VECTOR_FUNCTIONS_EXTERN_C(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, ) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs _T *_##_C##_do_resize(struct _C *vec, size_t n); \
_funcspecs _T *_##_C##_grow(struct _C *vec); \
_funcspecs _T *init_##_C(struct _C *vec, size_t n, void (*destroy_elem)(_T)); \
_funcspecs void destroy_##_C(struct _C *vec); \
_funcspecs _C##_pos_t _C##_insert(_C##_t *vec, _C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *vec, _T val); \
_funcspecs _C##_pos_t _C##_release(_C##_t *vec, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_remove(_C##_t *vec, _C##_pos_t pos); \
_funcspecs void _C##_clear(_C##_t *vec);

#define GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs bool _##_C##_valid_index(const struct _C *vec, size_t i); \
_funcspecs bool _##_C##_valid_pos(const struct _C *vec, _T *pos); \
_funcspecs size_t _##_C##_index_of_pos(const struct _C *vec, _T *pos); \
_funcspecs _C##_pos_t _##_C##_pos_of_index(struct _C *vec, size_t i); \
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest); \
_funcspecs size_t _C##_length(const _C##_t *vec); \
_funcspecs bool _C##_empty(const _C##_t *vec); \
_funcspecs size_t _C##_capacity(const _C##_t *vec); \
_funcspecs size_t _C##_max_capacity(void); \
_funcspecs _T *_C##_reserve(_C##_t *vec, size_t n); \
_funcspecs _T *_C##_shrink(_C##_t *vec); \
_funcspecs _C##_pos_t _C##_begin(_C##_t *vec); \
_funcspecs _C##_pos_t _C##_end(_C##_t *vec); \
_funcspecs bool _C##_at_begin(const _C##_t *vec, _C##_pos_t pos); \
_funcspecs bool _C##_at_end(const _C##_t *vec, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos); \
_funcspecs void _C##_forward(_C##_pos_t *pos); \
_funcspecs void _C##_backward(_C##_pos_t *pos); \
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end); \
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range); \
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range); \
_funcspecs bool _C##_range_at_begin(_C##_range_t range, _C##_pos_t pos); \
_funcspecs bool _C##_range_at_end(_C##_range_t range, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_all(_C##_t *vec); \
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *vec, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *vec, _C##_pos_t pos); \
_funcspecs size_t _C##_range_length(_C##_range_t range); \
_funcspecs bool _C##_range_empty(_C##_range_t range); \
_funcspecs _T _C##_front(const _C##_t *vec); \
_funcspecs _T _C##_back(const _C##_t *vec); \
_funcspecs _T _C##_at(const _C##_t *vec, size_t i); \
_funcspecs _T _C##_get(_C##_pos_t pos); \
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos); \
_funcspecs void _C##_set(_C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *vec, _T val); \
_funcspecs void _C##_remove_front(_C##_t *vec); \
_funcspecs void _C##_remove_back(_C##_t *vec);

#define GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs _T *_##_C##_do_resize(struct _C *vec, size_t n) \
{ \
    assert(n >= _C##_length(vec) && n <= _C##_max_capacity()); \
\
    if (n < GCL_VECTOR_MINIMAL_CAPACITY) \
        n = GCL_VECTOR_MINIMAL_CAPACITY; \
\
    if (n == _C##_capacity(vec)) \
        return vec->data; \
\
    size_t length = _C##_length(vec); \
    _T *data = gcl_realloc(vec->data, n * sizeof(_T)); \
\
    vec->data = data; \
    vec->data_end = data + n; \
    vec->end = data + length; \
\
    return data; \
} \
\
_funcspecs _T *_##_C##_grow(struct _C *vec) \
{ \
    size_t max_cap = _C##_max_capacity(); \
    size_t old_cap = _C##_capacity(vec); \
\
    if (old_cap == max_cap) \
        return NULL; \
\
    size_t new_cap = (size_t) (old_cap * GCL_VECTOR_GROWTH_FACTOR); \
\
    if (new_cap > max_cap) \
        new_cap = max_cap; \
\
    return _##_C##_do_resize(vec, new_cap); \
} \
\
_funcspecs _T *init_##_C(struct _C *vec, size_t n, void (*destroy_elem)(_T)) \
{ \
    if (n < GCL_VECTOR_INITIAL_CAPACITY) \
        n = GCL_VECTOR_INITIAL_CAPACITY; \
\
    _T *data = gcl_malloc(n * sizeof(_T)); \
\
    *vec = (struct _C) { \
        .data = data, \
        .data_end = data + n, \
        .end = data, \
        .destroy_elem = destroy_elem \
    }; \
\
    return data; \
} \
\
_funcspecs void destroy_##_C(struct _C *vec) \
{ \
    _T *pos; \
\
    if (vec->destroy_elem) { \
        _gcl_vector_for_each_pos(pos, vec) \
            vec->destroy_elem(*pos); \
    } \
\
    free(vec->data); \
} \
\
_funcspecs _C##_pos_t _C##_insert(_C##_t *vec, _C##_pos_t pos, _T val) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
\
    if (_C##_capacity(vec) <= _C##_length(vec)) { \
        size_t i = _##_C##_index_of_pos(vec, pos); \
        _##_C##_grow(vec); \
        pos = _##_C##_pos_of_index(vec, i); \
    } \
\
    assert(_C##_capacity(vec) > _C##_length(vec)); \
\
    if (pos < vec->end) \
        _##_C##_move_data(pos, vec->end, pos + 1); \
\
    *pos = val; \
    vec->end++; \
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *vec, _T val) \
{ \
    if (_C##_capacity(vec) <= _C##_length(vec)) \
        _##_C##_grow(vec); \
\
    assert(_C##_capacity(vec) > _C##_length(vec)); \
\
    *vec->end++ = val; \
    return vec->end - 1; \
} \
\
_funcspecs _C##_pos_t _C##_release(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos) && pos != vec->end); \
\
    _##_C##_move_data(pos + 1, vec->end, pos); \
    vec->end--; \
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_remove(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos) && pos != vec->end); \
\
    if (vec->destroy_elem) \
        vec->destroy_elem(*pos); \
\
    return _C##_release(vec, pos); \
} \
\
_funcspecs void _C##_clear(_C##_t *vec) \
{ \
    _T *pos; \
\
    if (vec->destroy_elem) { \
        _gcl_vector_for_each_pos(pos, vec) \
            vec->destroy_elem(*pos); \
    } \
\
    vec->end = vec->data; \
}

#define GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs bool _##_C##_valid_index(const struct _C *vec, size_t i) \
{ \
    return i < _C##_length(vec); \
} \
\
_funcspecs bool _##_C##_valid_pos(const struct _C *vec, _T *pos) \
{ \
    return pos >= vec->data && pos <= vec->end; \
} \
\
_funcspecs size_t _##_C##_index_of_pos(const struct _C *vec, _T *pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos) && pos != vec->end); \
    return (size_t) (pos - vec->data); \
} \
\
_funcspecs _C##_pos_t _##_C##_pos_of_index(struct _C *vec, size_t i) \
{ \
    assert(_##_C##_valid_index(vec, i)); \
    return vec->data + i; \
} \
\
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest) \
{ \
    if (begin < end) \
        memmove(dest, begin, (end - begin) * sizeof(_T)); \
} \
\
_funcspecs size_t _C##_length(const _C##_t *vec) \
{ \
    return (size_t) (vec->end - vec->data); \
} \
\
_funcspecs bool _C##_empty(const _C##_t *vec) \
{ \
    return vec->data == vec->end; \
} \
\
_funcspecs size_t _C##_capacity(const _C##_t *vec) \
{ \
    return (size_t) (vec->data_end - vec->data); \
} \
\
_funcspecs size_t _C##_max_capacity(void) \
{ \
    return (size_t) (SIZE_MAX / (GCL_VECTOR_GROWTH_FACTOR * sizeof(_T))); \
} \
\
_funcspecs _T *_C##_reserve(_C##_t *vec, size_t n) \
{ \
    assert(n <= _C##_max_capacity()); \
 \
    if (n > _C##_capacity(vec)) \
        return _##_C##_do_resize(vec, n); \
    else \
        return vec->data; \
} \
\
_funcspecs _T *_C##_shrink(_C##_t *vec) \
{ \
    return _##_C##_do_resize(vec, _C##_length(vec)); \
} \
\
_funcspecs _C##_pos_t _C##_begin(_C##_t *vec) \
{ \
    return vec->data; \
} \
\
_funcspecs _C##_pos_t _C##_end(_C##_t *vec) \
{ \
    return vec->end; \
} \
\
_funcspecs bool _C##_at_begin(const _C##_t *vec, _C##_pos_t pos) \
{ \
    return pos == vec->data; \
} \
\
_funcspecs bool _C##_at_end(const _C##_t *vec, _C##_pos_t pos) \
{ \
    return pos == vec->end; \
} \
\
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos) \
{ \
    return pos + 1; \
} \
\
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos) \
{ \
    return pos - 1; \
} \
\
_funcspecs void _C##_forward(_C##_pos_t *pos) \
{ \
    (*pos)++; \
} \
\
_funcspecs void _C##_backward(_C##_pos_t *pos) \
{ \
    (*pos)--; \
} \
\
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end) \
{ \
    return (struct _C##_range) { begin, end }; \
} \
\
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range) \
{ \
    return range.begin; \
} \
\
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range) \
{ \
    return range.end; \
} \
\
_funcspecs bool _C##_range_at_begin(_C##_range_t range, _C##_pos_t pos) \
{ \
    return pos == range.begin; \
} \
\
_funcspecs bool _C##_range_at_end(_C##_range_t range, _C##_pos_t pos) \
{ \
    return pos == range.end; \
} \
\
_funcspecs _C##_range_t _C##_all(_C##_t *vec) \
{ \
    return _C##_range(vec->data, vec->end); \
} \
\
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
    return _C##_range(pos, vec->end); \
} \
\
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
    return _C##_range(vec->data, pos); \
} \
\
_funcspecs size_t _C##_range_length(_C##_range_t range) \
{ \
    assert(range.begin <= range.end); \
    return (size_t) (range.begin - range.end); \
} \
\
_funcspecs bool _C##_range_empty(_C##_range_t range) \
{ \
    return range.begin == range.end; \
} \
\
_funcspecs _T _C##_front(const _C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    return *vec->data; \
} \
\
_funcspecs _T _C##_back(const _C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    return *(vec->end - 1); \
} \
\
_funcspecs _T _C##_at(const _C##_t *vec, size_t i) \
{ \
    assert(_##_C##_valid_index(vec, i)); \
    return *(vec->data + i); \
} \
\
_funcspecs _T _C##_get(_C##_pos_t pos) \
{ \
    return *pos; \
} \
\
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos) \
{ \
    return pos; \
} \
\
_funcspecs void _C##_set(_C##_pos_t pos, _T val) \
{ \
    *pos = val; \
} \
\
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *vec, _T val) \
{ \
    return _C##_insert(vec, vec->data, val); \
} \
\
_funcspecs void _C##_remove_front(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    _C##_remove(vec, _C##_begin(vec)); \
} \
\
_funcspecs void _C##_remove_back(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    _C##_remove(vec, _C##_end(vec) - 1); \
}

#endif

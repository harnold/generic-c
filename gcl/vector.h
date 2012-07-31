/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#ifndef GCL_VECTOR_H
#define GCL_VECTOR_H

#ifndef GCL_ERROR
#define GCL_ERROR(errnum, ...)
#endif

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define GCL_VECTOR_MINIMAL_CAPACITY      (16)
#define GCL_VECTOR_INITIAL_CAPACITY      (16)
#define GCL_VECTOR_GROWTH_FACTOR         (2)

#define gcl_vector_data(vec)             ((vec)->data)
#define gcl_vector_length(vec)           ((vec)->length)
#define gcl_vector_capacity(vec)         ((vec)->capacity)
#define gcl_vector_begin(vec)            ((vec)->data)
#define gcl_vector_end(vec)              ((vec)->data + (vec)->length)
#define gcl_vector_all(vec)              { gcl_vector_begin(vec), gcl_vector_end(vec) }
#define gcl_vector_range(begin, end)     { begin, end }
#define gcl_vector_range_begin(range)    ((range).begin)
#define gcl_vector_range_end(range)      ((range).end)
#define gcl_vector_range_from_pos(vec, pos) { pos, gcl_vector_end(vec) }
#define gcl_vector_range_to_pos(vec, pos) { gcl_vector_begin(vec), pos }
#define gcl_vector_range_length(range)   ((size_t) ((range).end - (range).begin))
#define gcl_vector_range_empty(range)    ((range).begin == (range).end)

#define gcl_vector_for_each_pos(pos, vec) \
    for ((pos) = gcl_vector_begin(vec); \
         (pos) != gcl_vector_end(vec); \
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
    size_t length; \
    size_t capacity; \
    void (*destroy_elem)(_T); \
};

#define GCL_GENERATE_VECTOR_FUNCTIONS_STATIC(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, static) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, static inline) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, static) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, static inline)

#define GCL_GENERATE_VECTOR_FUNCTIONS_EXTERN_H(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, ) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, inline)

#define GCL_GENERATE_VECTOR_FUNCTIONS_EXTERN_C(_C, _T) \
    GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, ) \
    GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_VECTOR_LONG_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs _T *_##_C##_do_resize(struct _C *vec, size_t n); \
_funcspecs _T *_##_C##_grow(struct _C *vec, size_t n); \
_funcspecs _T *init_##_C(struct _C *vec, size_t n, void (*destroy_elem)(_T)); \
_funcspecs void destroy_##_C(struct _C *vec); \
_funcspecs _T *_C##_reserve(_C##_t *vec, size_t n); \
_funcspecs _T *_C##_shrink(_C##_t *vec); \
_funcspecs _T *_C##_data(_C##_t *vec); \
_funcspecs _T _C##_front(_C##_t *vec); \
_funcspecs _T _C##_back(_C##_t *vec); \
_funcspecs _T _C##_at(_C##_t *vec, size_t i); \
_funcspecs _T _C##_get(_C##_pos_t pos); \
_funcspecs _T *_C##_get_ptr(_C#_pos_t pos); \
_funcspecs void _C##_set(_C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos); \
_funcspecs void _C##_forward(_C##_pos_t *pos); \
_funcspecs void _C##_backward(_C##_pos_t *pos); \
_funcspecs _C##_pos_t _C##_insert(_C##_t *vec, _C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *vec, _T val); \
_funcspecs _C##_pos_t _C##_release(_C##_t *vec, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_remove(_C##_t *vec, _C##_pos_t pos); \
_funcspecs void _C##_clear(_C##_t *vec);

#define GCL_GENERATE_VECTOR_SHORT_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs bool _##_C##_valid_index(struct _C *vec, size_t i); \
_funcspecs bool _##_C##_valid_pos(struct _C *vec, _T *pos); \
_funcspecs bool _##_C##_valid_range(struct _C *vec, _T *begin, _T *end); \
_funcspecs size_t _##_C##_index_of_pos(struct _C *vec, _T *pos); \
_funcspecs _C##_pos_t _##_C##_pos_of_index(struct _C *vec, size_t i); \
_funcspecs size_t _##_C##_max_capacity(void); \
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest); \
_funcspecs _C##_pos_t _C##_begin(_C##_t *vec); \
_funcspecs _C##_pos_t _C##_end(_C##_t *vec); \
_funcspecs _C##_range_t _C##_all(_C##_t *vec); \
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end); \
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range); \
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range); \
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *vec, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *vec, _C##_pos_t pos); \
_funcspecs size_t _C##_range_length(_C##_range_t range); \
_funcspecs bool _C##_range_empty(_C##_range_t range); \
_funcspecs size_t _C##_capacity(_C##_t *vec); \
_funcspecs size_t _C##_length(_C##_t *vec); \
_funcspecs bool _C##_empty(_C##_t *vec); \
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *vec, _T val); \
_funcspecs void _C##_remove_front(_C##_t *vec); \
_funcspecs void _C##_remove_back(_C##_t *vec);

#define GCL_GENERATE_VECTOR_LONG_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs _T *_##_C##_do_resize(struct _C *vec, size_t n) \
{ \
    assert(n <= _##_C##_max_capacity()); \
\
    _T *data; \
\
    if (n < GCL_VECTOR_MINIMAL_CAPACITY) \
        n = GCL_VECTOR_MINIMAL_CAPACITY; \
\
    if (!(data = realloc(vec->data, n * sizeof(_T)))) { \
        GCL_ERROR(errno, "Reallocating memory for vector data failed"); \
        return NULL; \
    } \
\
    vec->data = data; \
    vec->capacity = n; \
    return data; \
} \
\
_funcspecs _T *_##_C##_grow(struct _C *vec, size_t n) \
{ \
    assert(n > vec->length); \
\
    size_t max_cap = _##_C##_max_capacity(); \
    size_t new_cap; \
\
    if (n > max_cap) \
        return NULL; \
\
    new_cap = vec->capacity * GCL_VECTOR_GROWTH_FACTOR; \
\
    if (new_cap > max_cap) \
        new_cap = max_cap; \
\
    if (new_cap < n) \
        new_cap = n; \
\
    return _##_C##_do_resize(vec, new_cap); \
} \
\
_funcspecs _T *init_##_C(struct _C *vec, size_t n, void (*destroy_elem)(_T)) \
{ \
    _T *data; \
\
    if (n < GCL_VECTOR_INITIAL_CAPACITY) \
        n = GCL_VECTOR_INITIAL_CAPACITY; \
\
    if (!(data = malloc(n * sizeof(_T)))) { \
        GCL_ERROR(errno, "Allocating memory for vector data failed"); \
        return NULL; \
    } \
\
    vec->data = data; \
    vec->length = 0; \
    vec->capacity = n; \
    vec->destroy_elem = destroy_elem; \
    return data; \
} \
\
_funcspecs void destroy_##_C(struct _C *vec) \
{ \
    _T *pos; \
\
    if (vec->destroy_elem) { \
        gcl_vector_for_each_pos(pos, vec) \
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
    if (vec->capacity <= vec->length) { \
        size_t i = _##_C##_index_of_pos(vec, pos); \
        if (!_##_C##_grow(vec, vec->length + 1)) { \
            GCL_ERROR(0, "Increasing vector capacity failed"); \
            return NULL; \
        } \
        pos = _##_C##_pos_of_index(vec, i); \
    } \
\
    assert(vec->capacity > vec->length); \
\
    if (pos < gcl_vector_end(vec)) \
        _##_C##_move_data(pos, gcl_vector_end(vec), pos + 1); \
\
    *pos = val; \
    vec->length++; \
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *vec, _T val) \
{ \
    if (vec->capacity <= vec->length) { \
        if (!_##_C##_grow(vec, vec->length + 1)) { \
            GCL_ERROR(0, "Increasing vector capacity failed"); \
            return NULL; \
        } \
    } \
\
    assert(vec->capacity > vec->length); \
\
    vec->data[vec->length++] = val; \
    return gcl_vector_end(vec) - 1; \
} \
\
_funcspecs _C##_pos_t _C##_release(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos) && pos != gcl_vector_end(vec)); \
\
    if (pos < gcl_vector_end(vec) - 1) \
        _##_C##_move_data(pos + 1, gcl_vector_end(vec), pos); \
\
    vec->length--; \
    return pos; \
} \
\
_funcspecs _C##_pos_t _C##_remove(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos) && pos != gcl_vector_end(vec)); \
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
        gcl_vector_for_each_pos(pos, vec) \
            vec->destroy_elem(*pos); \
    } \
\
    vec->length = 0; \
}

#define GCL_GENERATE_VECTOR_SHORT_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs bool _##_C##_valid_index(struct _C *vec, size_t i) \
{ \
    return i < vec->length; \
} \
\
_funcspecs bool _##_C##_valid_pos(struct _C *vec, _T *pos) \
{ \
    return gcl_vector_begin(vec) <= pos && pos <= gcl_vector_end(vec); \
} \
\
_funcspecs bool _##_C##_valid_range(struct _C *vec, _T *begin, _T *end) \
{ \
    return _##_C##_valid_pos(vec, begin) \
        && _##_C##_valid_pos(vec, end) \
        && begin <= end; \
} \
\
_funcspecs size_t _##_C##_index_of_pos(struct _C *vec, _T *pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
    return (size_t) (pos - gcl_vector_begin(vec)); \
} \
\
_funcspecs _C##_pos_t _##_C##_pos_of_index(struct _C *vec, size_t i) \
{ \
    assert(_##_C##_valid_index(vec, i)); \
    return gcl_vector_begin(vec) + i; \
} \
\
_funcspecs size_t _##_C##_max_capacity(void) \
{ \
    return (size_t) (SIZE_MAX / (GCL_VECTOR_GROWTH_FACTOR * sizeof(_T))); \
} \
\
_funcspecs void _##_C##_move_data(_T *begin, _T *end, _T *dest) \
{ \
    assert(begin <= end); \
    memmove(dest, begin, (size_t) (end - begin) * sizeof(_T)); \
} \
\
_funcspecs _C##_pos_t _C##_begin(_C##_t *vec) \
{ \
    return gcl_vector_begin(vec); \
} \
\
_funcspecs _C##_pos_t _C##_end(_C##_t *vec) \
{ \
    return gcl_vector_end(vec); \
} \
\
_funcspecs _C##_range_t _C##_all(_C##_t *vec) \
{ \
    return (struct _C##_range) gcl_vector_all(vec); \
} \
\
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range) \
{ \
    return gcl_vector_range_begin(range); \
} \
\
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range) \
{ \
    return gcl_vector_range_end(range); \
} \
\
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
    return (struct _C##_range) gcl_vector_range_from_pos(vec, pos); \
} \
\
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *vec, _C##_pos_t pos) \
{ \
    assert(_##_C##_valid_pos(vec, pos)); \
    return (struct _C##_range) gcl_vector_range_to_pos(vec, pos); \
} \
\
_funcspecs size_t _C##_range_length(_C##_range_t range) \
{ \
    return gcl_vector_range_length(range); \
} \
\
_funcspecs bool _C##_range_empty(_C##_range_t range) \
{ \
    return gcl_vector_range_empty(range); \
} \
\
_funcspecs size_t _C##_capacity(_C##_t *vec) \
{ \
    return gcl_vector_capacity(vec); \
} \
\
_funcspecs size_t _C##_length(_C##_t *vec) \
{ \
    return gcl_vector_length(vec); \
} \
\
_funcspecs bool _C##_empty(_C##_t *vec) \
{ \
    return gcl_vector_length(vec) == 0; \
} \
\
_funcspecs _T *_C##_reserve(_C##_t *vec, size_t n) \
{ \
    assert(n <= _##_C##_max_capacity()); \
\
    if (n <= vec->capacity) \
        return vec->data; \
\
    return _##_C##_do_resize(vec, n); \
} \
\
_funcspecs _T *_C##_shrink(_C##_t *vec) \
{ \
    return _##_C##_do_resize(vec, vec->length); \
} \
\
_funcspecs _T *_C##_data(_C##_t *vec) \
{ \
    return vec->data; \
} \
\
_funcspecs _T _C##_front(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    return vec->data[0]; \
} \
\
_funcspecs _T _C##_back(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    return vec->data[vec->length - 1]; \
} \
\
_funcspecs _T _C##_at(_C##_t *vec, size_t i) \
{ \
    assert(_##_C##_valid_index(vec, i)); \
    return vec->data[i]; \
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
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *vec, _T val) \
{ \
    return _C##_insert(vec, gcl_vector_begin(vec), val); \
} \
\
_funcspecs void _C##_remove_front(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    _C##_remove(vec, gcl_vector_begin(vec)); \
} \
\
_funcspecs void _C##_remove_back(_C##_t *vec) \
{ \
    assert(!_C##_empty(vec)); \
    _C##_remove(vec, gcl_vector_end(vec) - 1); \
}

#endif

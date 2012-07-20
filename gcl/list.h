/*
 * Copyright 2012 Holger Arnold.
 *
 * Distributed under the Boost Software License, Version 1.0,
 * as published at http://www.boost.org/LICENSE_1_0.txt.
 */

#ifndef GCL_LIST_H
#define GCL_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef GCL_ERROR
#define GCL_ERROR(errnum, ...)
#endif

#define gcl_list_begin(list)             ((list)->end.next)
#define gcl_list_end(list)               (&(list)->end)
#define gcl_list_all(list)               { gcl_list_begin(list), gcl_list_end(list) }
#define gcl_list_range(begin, end)       { begin, end }
#define gcl_list_range_begin(range)      ((range).begin)
#define gcl_list_range_end(range)        ((range).end)
#define gcl_list_range_from_pos(list, pos) { pos, gcl_list_end(list) }
#define gcl_list_range_to_pos(list, pos) { gcl_list_begin(list), pos }
#define gcl_list_range_empty(range)      ((range).begin == (range).end)

#define gcl_list_for_each_node(node, list) \
    for ((node) = gcl_list_begin(list); \
         (node) != gcl_list_end(list); \
         (node) = (node)->next)

#define gcl_list_for_each_node_safe(node, tmp, list) \
    for ((node) = gcl_list_begin(list), (tmp) = (node)->next; \
         (node) != gcl_list_end(list); \
         (node) = (tmp), (tmp) = (node)->next)

#define GCL_GENERATE_LIST_TYPES(_C, _T) \
\
typedef struct _C _C##_t; \
typedef struct _C##_node *_C##_pos_t; \
typedef struct _C##_range _C##_range_t; \
typedef _T _C##_elem_t; \
\
struct _C##_node { \
    struct _C##_node *next; \
    struct _C##_node *prev; \
    _T elem; \
}; \
\
struct _C##_range { \
    struct _C##_node *begin; \
    struct _C##_node *end; \
}; \
\
struct _C { \
    struct _C##_node end; \
    void (*destroy_elem)(_T); \
};

#define GCL_GENERATE_LIST_FUNCTIONS_STATIC(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DEFS(_C, _T, static)

#define GCL_GENERATE_LIST_FUNCTIONS_STATIC_INLINE(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DEFS(_C, _T, static inline)

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_H(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_C(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DEFS(_C, _T, )

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_INLINE_H(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DEFS(_C, _T, inline)

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_INLINE_C(_C, _T) \
    GCL_GENERATE_LIST_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_LIST_FUNCTION_DECLS(_C, _T, _funspecs) \
\
_funspecs void init_##_C(struct _C *list, void (*destroy_elem)(_T));  \
_funspecs void destroy_##_C(struct _C *list);  \
_funspecs void _C##_link_nodes(struct _C##_node *prev, struct _C##_node *next);  \
_funspecs void _C##_unlink_node(struct _C##_node* node);  \
_funspecs _C##_pos_t _C##_begin(_C##_t *list);  \
_funspecs _C##_pos_t _C##_end(_C##_t *list);  \
_funspecs _C##_range_t _C##_all(_C##_t *list);  \
_funspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end);  \
_funspecs _C##_pos_t _C##_range_begin(_C##_range_t range);  \
_funspecs _C##_pos_t _C##_range_end(_C##_range_t range);  \
_funspecs _C##_range_t _C##_range_from_pos(_C##_t *list, _C##_pos_t pos);  \
_funspecs _C##_range_t _C##_range_to_pos(_C##_t *list, _C##_pos_t pos);  \
_funspecs bool _C##_range_empty(_C##_range_t range);  \
_funspecs bool _C##_empty(_C##_t *list);  \
_funspecs _T _C##_front(_C##_t *list);  \
_funspecs _T _C##_back(_C##_t *list);  \
_funspecs _T _C##_get(_C##_pos_t pos);  \
_funspecs void _C##_set(_C##_pos_t pos, _T val);  \
_funspecs _C##_pos_t _C##_next(_C##_pos_t pos);  \
_funspecs _C##_pos_t _C##_prev(_C##_pos_t pos);  \
_funspecs void _C##_forward(_C##_pos_t *pos);  \
_funspecs void _C##_backward(_C##_pos_t *pos);  \
_funspecs _C##_pos_t _C##_insert(_C##_t *list, _C##_pos_t pos, _T val);  \
_funspecs _C##_pos_t _C##_insert_front(_C##_t *list, _T val);  \
_funspecs _C##_pos_t _C##_insert_back(_C##_t *list, _T val);  \
_funspecs _C##_pos_t _C##_release(_C##_t *list, _C##_pos_t pos);  \
_funspecs _C##_pos_t _C##_remove(_C##_t *list, _C##_pos_t pos);  \
_funspecs void _C##_remove_front(_C##_t *list);  \
_funspecs void _C##_remove_back(_C##_t *list);  \
_funspecs void _C##_clear(_C##_t *list);  \
_funspecs void _C##_move(_C##_t *dest_list, _C##_pos_t dest_pos, _C##_t *src_list, _C##_pos_t src_pos);  \
_funspecs void _C##_move_front(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos);  \
_funspecs void _C##_move_back(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos);  \
_funspecs void _C##_splice(_C##_t *dest_list, _C##_pos_t pos, _C##_t *src_list, _C##_range_t range);  \
_funspecs void _C##_splice_front(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range);  \
_funspecs void _C##_splice_back(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range);

#define GCL_GENERATE_LIST_FUNCTION_DEFS(_C, _T, _funspecs) \
\
_funspecs void init_##_C(struct _C *list, void (*destroy_elem)(_T)) \
{ \
    list->end.next = &list->end; \
    list->end.prev = &list->end; \
    list->destroy_elem = destroy_elem; \
} \
\
_funspecs void destroy_##_C(struct _C *list) \
{ \
    struct _C##_node *node, *tmp; \
\
    if (list->destroy_elem) { \
        gcl_list_for_each_node(node, list) \
            list->destroy_elem(node->elem); \
    } \
\
    gcl_list_for_each_node_safe(node, tmp, list) \
        free(node); \
} \
\
_funspecs void _C##_link_nodes(struct _C##_node *prev, struct _C##_node *next) \
{ \
    prev->next = next; \
    next->prev = prev; \
} \
\
_funspecs void _C##_unlink_node(struct _C##_node* node) \
{ \
    _C##_link_nodes(node->prev, node->next); \
} \
\
_funspecs _C##_pos_t _C##_begin(_C##_t *list) \
{ \
    return gcl_list_begin(list); \
} \
\
_funspecs _C##_pos_t _C##_end(_C##_t *list) \
{ \
    return gcl_list_end(list); \
} \
\
_funspecs _C##_range_t _C##_all(_C##_t *list) \
{ \
    return (struct _C##_range) gcl_list_all(list); \
} \
\
_funspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end) \
{ \
    return (struct _C##_range) gcl_list_range(begin, end); \
} \
\
_funspecs _C##_pos_t _C##_range_begin(_C##_range_t range) \
{ \
    return gcl_list_range_begin(range); \
} \
\
_funspecs _C##_pos_t _C##_range_end(_C##_range_t range) \
{ \
    return gcl_list_range_end(range); \
} \
\
_funspecs _C##_range_t _C##_range_from_pos(_C##_t *list, _C##_pos_t pos) \
{ \
    return (struct _C##_range) gcl_list_range_from_pos(list, pos); \
} \
\
_funspecs _C##_range_t _C##_range_to_pos(_C##_t *list, _C##_pos_t pos) \
{ \
    return (struct _C##_range) gcl_list_range_to_pos(list, pos); \
} \
\
_funspecs bool _C##_range_empty(_C##_range_t range) \
{ \
    return gcl_list_range_empty(range); \
} \
\
_funspecs bool _C##_empty(_C##_t *list) \
{ \
    return gcl_list_begin(list) == gcl_list_end(list); \
} \
\
_funspecs _T _C##_front(_C##_t *list) \
{ \
    assert(!_C##_empty(list)); \
    return gcl_list_begin(list)->elem; \
} \
\
_funspecs _T _C##_back(_C##_t *list) \
{ \
    assert(!_C##_empty(list)); \
    return gcl_list_end(list)->prev->elem; \
} \
\
_funspecs _T _C##_get(_C##_pos_t pos) \
{ \
    return pos->elem; \
} \
\
_funspecs void _C##_set(_C##_pos_t pos, _T val) \
{ \
    pos->elem = val; \
} \
\
_funspecs _C##_pos_t _C##_next(_C##_pos_t pos) \
{ \
    return pos->next; \
} \
\
_funspecs _C##_pos_t _C##_prev(_C##_pos_t pos) \
{ \
    return pos->prev; \
} \
\
_funspecs void _C##_forward(_C##_pos_t *pos) \
{ \
    *pos = (*pos)->next; \
} \
\
_funspecs void _C##_backward(_C##_pos_t *pos) \
{ \
    *pos = (*pos)->prev; \
} \
\
_funspecs _C##_pos_t _C##_insert(_C##_t *list, _C##_pos_t pos, _T val) \
{ \
    struct _C##_node *node; \
\
    if (!(node = malloc(sizeof(*node)))) { \
        GCL_ERROR(errno, "Allocating memory for list node failed"); \
        return NULL; \
    } \
\
    node->elem = val; \
    _C##_link_nodes(pos->prev, node); \
    _C##_link_nodes(node, pos); \
    return node; \
} \
\
_funspecs _C##_pos_t _C##_insert_front(_C##_t *list, _T val) \
{ \
    return _C##_insert(list, gcl_list_begin(list), val); \
} \
\
_funspecs _C##_pos_t _C##_insert_back(_C##_t *list, _T val) \
{ \
    return _C##_insert(list, gcl_list_end(list), val); \
} \
\
_funspecs _C##_pos_t _C##_release(_C##_t *list, _C##_pos_t pos) \
{ \
    assert(pos != gcl_list_end(list)); \
\
    _C##_pos_t next = pos->next; \
\
    _C##_unlink_node(pos); \
    free(pos); \
    return next; \
} \
\
_funspecs _C##_pos_t _C##_remove(_C##_t *list, _C##_pos_t pos) \
{ \
    assert(pos != gcl_list_end(list)); \
\
    if (list->destroy_elem) \
        list->destroy_elem(pos->elem); \
\
    return _C##_release(list, pos); \
} \
\
_funspecs void _C##_remove_front(_C##_t *list) \
{ \
    assert(!_C##_empty(list)); \
    _C##_remove(list, gcl_list_begin(list)); \
} \
\
_funspecs void _C##_remove_back(_C##_t *list) \
{ \
    assert(!_C##_empty(list)); \
    _C##_remove(list, gcl_list_end(list)->prev); \
} \
\
_funspecs void _C##_clear(_C##_t *list) \
{ \
    struct _C##_node *node, *tmp; \
\
    if (list->destroy_elem) { \
        gcl_list_for_each_node(node, list) \
            list->destroy_elem(node->elem); \
    } \
\
    gcl_list_for_each_node_safe(node, tmp, list) \
        free(node); \
\
    init_##_C(list, list->destroy_elem); \
} \
\
_funspecs void _C##_move(_C##_t *dest_list, _C##_pos_t dest_pos, _C##_t *src_list, _C##_pos_t src_pos) \
{ \
    assert(src_pos != gcl_list_end(src_list)); \
\
    _C##_unlink_node(src_pos); \
    _C##_link_nodes(dest_pos->prev, src_pos); \
    _C##_link_nodes(src_pos, dest_pos); \
} \
\
_funspecs void _C##_move_front(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos) \
{ \
    assert(pos != gcl_list_end(src_list)); \
    _C##_move(dest_list, gcl_list_begin(dest_list), src_list, pos); \
} \
\
_funspecs void _C##_move_back(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos) \
{ \
    assert(pos != gcl_list_end(src_list)); \
    _C##_move(dest_list, gcl_list_end(dest_list), src_list, pos); \
} \
\
_funspecs void _C##_splice(_C##_t *dest_list, _C##_pos_t pos, _C##_t *src_list, _C##_range_t range) \
{ \
    _C##_link_nodes(range.end->prev, pos); \
    _C##_link_nodes(range.begin->prev, range.end); \
    _C##_link_nodes(pos->prev, range.begin); \
} \
\
_funspecs void _C##_splice_front(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range) \
{ \
    _C##_splice(dest_list, gcl_list_begin(dest_list), src_list, range); \
} \
\
_funspecs void _C##_splice_back(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range) \
{ \
    _C##_splice(dest_list, gcl_list_end(dest_list), src_list, range); \
}

#endif

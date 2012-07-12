#ifndef GC_LIST_H
#define GC_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef GC_ERROR
#define GC_ERROR(errnum, ...)
#endif

#define gc_list_begin(list)             ((list)->end.next)
#define gc_list_end(list)               (&(list)->end)
#define gc_list_all(list)               { gc_list_begin(list), gc_list_end(list) }
#define gc_list_range(begin, end)       { begin, end }
#define gc_list_range_begin(range)      ((range).begin)
#define gc_list_range_end(range)        ((range).end)
#define gc_list_range_from_pos(list, pos) { pos, gc_list_end(list) }
#define gc_list_range_to_pos(list, pos) { gc_list_begin(list), pos }
#define gc_list_range_empty(range)      ((range).begin == (range).end)

#define gc_list_for_each_node(node, list) \
    for ((node) = gc_list_begin(list); \
         (node) != gc_list_end(list); \
         (node) = (node)->next)

#define gc_list_for_each_node_safe(node, tmp, list) \
    for ((node) = gc_list_begin(list), (tmp) = (node)->next; \
         (node) != gc_list_end(list); \
         (node) = (tmp), (tmp) = (node)->next)

#define GC_GENERATE_LIST_TYPE(C, T) \
    GC_GENERATE_LIST_INTF(C, T) \
    GC_GENERATE_LIST_IMPL(C, T)

#define GC_GENERATE_LIST_INTF(C, T) \
\
typedef struct C C##_t; \
typedef struct C##_node *C##_pos_t; \
typedef struct C##_range C##_range_t; \
typedef T C##_elem_t; \
\
struct C##_node { \
    struct C##_node *next; \
    struct C##_node *prev; \
    T elem; \
}; \
\
struct C##_range { \
    struct C##_node *begin; \
    struct C##_node *end; \
}; \
\
struct C { \
    struct C##_node end; \
    void (*destroy_elem)(T); \
}; \
\
static inline void init_##C(struct C *list, void (*destroy_elem)(T)); \
static inline void destroy_##C(struct C *list); \
\
static inline void C##_link_nodes(struct C##_node *prev, struct C##_node *next); \
static inline void C##_unlink_node(struct C##_node* node); \
\
static inline C##_pos_t C##_begin(C##_t *list); \
static inline C##_pos_t C##_end(C##_t *list); \
static inline C##_range_t C##_all(C##_t *list); \
static inline C##_range_t C##_range(C##_pos_t begin, C##_pos_t end); \
static inline C##_pos_t C##_range_begin(C##_range_t range); \
static inline C##_pos_t C##_range_end(C##_range_t range); \
static inline C##_range_t C##_range_from_pos(C##_t *list, C##_pos_t pos); \
static inline C##_range_t C##_range_to_pos(C##_t *list, C##_pos_t pos); \
static inline bool C##_range_empty(C##_range_t range); \
\
static inline bool C##_empty(C##_t *list); \
\
static inline T C##_front(C##_t *list); \
static inline T C##_back(C##_t *list); \
\
static inline T C##_get(C##_pos_t pos); \
static inline void C##_set(C##_pos_t pos, T val); \
static inline C##_pos_t C##_next(C##_pos_t pos); \
static inline C##_pos_t C##_prev(C##_pos_t pos); \
static inline void C##_forward(C##_pos_t *pos); \
static inline void C##_backward(C##_pos_t *pos); \
\
static inline C##_pos_t C##_insert(C##_t *list, C##_pos_t pos, T val); \
static inline void C##_insert_front(C##_t *list, T val); \
static inline void C##_insert_back(C##_t *list, T val); \
static inline C##_pos_t C##_release(C##_t *list, C##_pos_t pos); \
static inline C##_pos_t C##_remove(C##_t *list, C##_pos_t pos); \
static inline void C##_remove_front(C##_t *list); \
static inline void C##_remove_back(C##_t *list); \
static inline void C##_clear(C##_t *list); \
\
static inline void C##_move(C##_t *dest_list, C##_pos_t dest_pos, C##_t *src_list, C##_pos_t src_pos); \
static inline void C##_move_front(C##_t *dest_list, C##_t *src_list, C##_pos_t pos); \
static inline void C##_move_back(C##_t *dest_list, C##_t *src_list, C##_pos_t pos); \
static inline void C##_splice(C##_t *dest_list, C##_pos_t pos, C##_t *src_list, C##_range_t range); \
static inline void C##_splice_front(C##_t *dest_list, C##_t *src_list, C##_range_t range); \
static inline void C##_splice_back(C##_t *dest_list, C##_t *src_list, C##_range_t range);

#define GC_GENERATE_LIST_IMPL(C, T) \
\
static inline void init_##C(struct C *list, void (*destroy_elem)(T)) \
{ \
    list->end.next = &list->end; \
    list->end.prev = &list->end; \
    list->destroy_elem = destroy_elem; \
} \
\
static inline void destroy_##C(struct C *list) \
{ \
    struct C##_node *node, *tmp; \
\
    if (list->destroy_elem) { \
        gc_list_for_each_node(node, list) \
            list->destroy_elem(node->elem); \
    } \
\
    gc_list_for_each_node_safe(node, tmp, list) \
        free(node); \
} \
\
static inline void C##_link_nodes(struct C##_node *prev, struct C##_node *next) \
{ \
    prev->next = next; \
    next->prev = prev; \
} \
\
static inline void C##_unlink_node(struct C##_node* node) \
{ \
    C##_link_nodes(node->prev, node->next); \
} \
\
static inline C##_pos_t C##_begin(C##_t *list) \
{ \
    return gc_list_begin(list); \
} \
\
static inline C##_pos_t C##_end(C##_t *list) \
{ \
    return gc_list_end(list); \
} \
\
static inline C##_range_t C##_all(C##_t *list) \
{ \
    return (struct C##_range) gc_list_all(list); \
} \
\
static inline C##_range_t C##_range(C##_pos_t begin, C##_pos_t end) \
{ \
    return (struct C##_range) gc_list_range(begin, end); \
} \
\
static inline C##_pos_t C##_range_begin(C##_range_t range) \
{ \
    return gc_list_range_begin(range); \
} \
\
static inline C##_pos_t C##_range_end(C##_range_t range) \
{ \
    return gc_list_range_end(range); \
} \
\
static inline C##_range_t C##_range_from_pos(C##_t *list, C##_pos_t pos) \
{ \
    return (struct C##_range) gc_list_range_from_pos(list, pos); \
} \
\
static inline C##_range_t C##_range_to_pos(C##_t *list, C##_pos_t pos) \
{ \
    return (struct C##_range) gc_list_range_to_pos(list, pos); \
} \
\
static inline bool C##_range_empty(C##_range_t range) \
{ \
    return gc_list_range_empty(range); \
} \
\
static inline bool C##_empty(C##_t *list) \
{ \
    return gc_list_begin(list) == gc_list_end(list); \
} \
\
static inline T C##_front(C##_t *list) \
{ \
    assert(!C##_empty(list)); \
    return gc_list_begin(list)->elem; \
} \
\
static inline T C##_back(C##_t *list) \
{ \
    assert(!C##_empty(list)); \
    return gc_list_end(list)->prev->elem; \
} \
\
static inline T C##_get(C##_pos_t pos) \
{ \
    return pos->elem; \
} \
\
static inline void C##_set(C##_pos_t pos, T val) \
{ \
    pos->elem = val; \
} \
\
static inline C##_pos_t C##_next(C##_pos_t pos) \
{ \
    return pos->next; \
} \
\
static inline C##_pos_t C##_prev(C##_pos_t pos) \
{ \
    return pos->prev; \
} \
\
static inline void C##_forward(C##_pos_t *pos) \
{ \
    *pos = (*pos)->next; \
} \
\
static inline void C##_backward(C##_pos_t *pos) \
{ \
    *pos = (*pos)->prev; \
} \
\
static inline C##_pos_t C##_insert(C##_t *list, C##_pos_t pos, T val) \
{ \
    struct C##_node *node; \
\
    if (!(node = malloc(sizeof(*node)))) { \
        GC_ERROR(errno, "Allocating memory for list node failed"); \
        return NULL; \
    } \
\
    node->elem = val; \
    C##_link_nodes(pos->prev, node); \
    C##_link_nodes(node, pos); \
    return node; \
} \
\
static inline void C##_insert_front(C##_t *list, T val) \
{ \
    C##_insert(list, gc_list_begin(list), val); \
} \
\
static inline void C##_insert_back(C##_t *list, T val) \
{ \
    C##_insert(list, gc_list_end(list), val); \
} \
\
static inline C##_pos_t C##_release(C##_t *list, C##_pos_t pos) \
{ \
    C##_pos_t next; \
\
    assert(pos != gc_list_end(list)); \
\
    next = pos->next;
    C##_unlink_node(pos); \
    free(pos); \
    return next; \
} \
\
static inline C##_pos_t C##_remove(C##_t *list, C##_pos_t pos) \
{ \
    assert(pos != gc_list_end(list)); \
\
    if (list->destroy_elem) \
        list->destroy_elem(pos->elem); \
\
    return C##_release(list, pos); \
} \
\
static inline void C##_remove_front(C##_t *list) \
{ \
    assert(!C##_empty(list)); \
    C##_remove(list, gc_list_begin(list)); \
} \
\
static inline void C##_remove_back(C##_t *list) \
{ \
    assert(!C##_empty(list)); \
    C##_remove(list, gc_list_end(list)->prev); \
} \
\
static inline void C##_clear(C##_t *list) \
{ \
    struct C##_node *node, *tmp; \
\
    if (list->destroy_elem) { \
        gc_list_for_each_node(node, list) \
            list->destroy_elem(node->elem); \
    } \
\
    gc_list_for_each_node_safe(node, tmp, list) \
        free(node); \
\
    init_##C(list, list->destroy_elem); \
} \
\
static inline void C##_move(C##_t *dest_list, C##_pos_t dest_pos, C##_t *src_list, C##_pos_t src_pos) \
{ \
    assert(src_pos != gc_list_end(src_list)); \
\
    C##_unlink_node(src_pos); \
    C##_link_nodes(dest_pos->prev, src_pos); \
    C##_link_nodes(src_pos, dest_pos); \
} \
\
static inline void C##_move_front(C##_t *dest_list, C##_t *src_list, C##_pos_t pos) \
{ \
    assert(pos != gc_list_end(src_list)); \
    C##_move(dest_list, gc_list_begin(dest_list), src_list, pos); \
} \
\
static inline void C##_move_back(C##_t *dest_list, C##_t *src_list, C##_pos_t pos) \
{ \
    assert(pos != gc_list_end(src_list)); \
    C##_move(dest_list, gc_list_end(dest_list), src_list, pos); \
} \
\
static inline void C##_splice(C##_t *dest_list, C##_pos_t pos, C##_t *src_list, C##_range_t range) \
{ \
    C##_link_nodes(range.end->prev, pos); \
    C##_link_nodes(range.begin->prev, range.end); \
    C##_link_nodes(pos->prev, range.begin); \
} \
\
static inline void C##_splice_front(C##_t *dest_list, C##_t *src_list, C##_range_t range) \
{ \
    C##_splice(dest_list, gc_list_begin(dest_list), src_list, range); \
} \
\
static inline void C##_splice_back(C##_t *dest_list, C##_t *src_list, C##_range_t range) \
{ \
    C##_splice(dest_list, gc_list_end(dest_list), src_list, range); \
}

#endif

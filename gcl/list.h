#ifndef GCL_LIST_H
#define GCL_LIST_H

#include <gcl/malloc.h>

#include <assert.h>
#include <stdbool.h>

#define _gcl_list_for_each_node(node, list, _C) \
	for ((node) = _C##_begin(list); \
	     (node) != _C##_end(list); \
	     (node) = (node)->next)

#define _gcl_list_for_each_node_safe(node, tmp, list, _C) \
	for ((node) = _C##_begin(list), (tmp) = (node)->next; \
	     (node) != _C##_end(list); \
	     (node) = (tmp), (tmp) = (node)->next)

#define GCL_GENERATE_LIST_TYPES(_C, _T) \
\
typedef struct _C _C##_t; \
typedef struct _C##_node *_C##_pos_t; \
typedef struct _C##_range _C##_range_t; \
typedef _T _C##_elem_t; \
\
struct _C##_node_base { \
	struct _C##_node *next; \
	struct _C##_node *prev; \
}; \
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
	struct _C##_node_base end; \
	void (*destroy_elem)(_T); \
};

#define GCL_GENERATE_LIST_FUNCTIONS_STATIC(_C, _T) \
	GCL_GENERATE_LIST_LONG_FUNCTION_DECLS(_C, _T, static) \
	GCL_GENERATE_LIST_SHORT_FUNCTION_DECLS(_C, _T, static inline) \
	GCL_GENERATE_LIST_LONG_FUNCTION_DEFS(_C, _T, static) \
	GCL_GENERATE_LIST_SHORT_FUNCTION_DEFS(_C, _T, static inline)

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_H(_C, _T) \
	GCL_GENERATE_LIST_LONG_FUNCTION_DECLS(_C, _T, ) \
	GCL_GENERATE_LIST_SHORT_FUNCTION_DECLS(_C, _T, inline) \
	GCL_GENERATE_LIST_SHORT_FUNCTION_DEFS(_C, _T, inline)

#define GCL_GENERATE_LIST_FUNCTIONS_EXTERN_C(_C, _T) \
	GCL_GENERATE_LIST_LONG_FUNCTION_DEFS(_C, _T, ) \
	GCL_GENERATE_LIST_SHORT_FUNCTION_DECLS(_C, _T, )

#define GCL_GENERATE_LIST_LONG_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs void destroy_##_C(struct _C *list); \
_funcspecs _C##_pos_t _C##_insert(_C##_t *list, _C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_release(_C##_t *list, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_remove(_C##_t *list, _C##_pos_t pos); \
_funcspecs void _C##_clear(_C##_t *list); \
_funcspecs void _C##_move(_C##_t *dest_list, _C##_pos_t dest_pos, _C##_t *src_list, _C##_pos_t src_pos); \
_funcspecs void _C##_splice(_C##_t *dest_list, _C##_pos_t pos, _C##_t *src_list, _C##_range_t range);

#define GCL_GENERATE_LIST_SHORT_FUNCTION_DECLS(_C, _T, _funcspecs) \
\
_funcspecs void init_##_C(struct _C *list, void (*destroy_elem)(_T)); \
_funcspecs void _C##_link_nodes(struct _C##_node *prev, struct _C##_node *next); \
_funcspecs void _C##_unlink_node(struct _C##_node* node); \
_funcspecs bool _C##_empty(_C##_t *list); \
_funcspecs _C##_pos_t _C##_begin(_C##_t *list); \
_funcspecs _C##_pos_t _C##_end(_C##_t *list); \
_funcspecs bool _C##_at_begin(_C##_t *list, _C##_pos_t pos); \
_funcspecs bool _C##_at_end(_C##_t *list, _C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos); \
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos); \
_funcspecs void _C##_forward(_C##_pos_t *pos); \
_funcspecs void _C##_backward(_C##_pos_t *pos); \
_funcspecs _C##_range_t _C##_range(_C##_pos_t begin, _C##_pos_t end); \
_funcspecs _C##_pos_t _C##_range_begin(_C##_range_t range); \
_funcspecs _C##_pos_t _C##_range_end(_C##_range_t range); \
_funcspecs bool _C##_range_at_begin(_C##_range_t range, _C##_pos_t pos); \
_funcspecs bool _C##_range_at_end(_C##_range_t range, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_all(_C##_t *list); \
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *list, _C##_pos_t pos); \
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *list, _C##_pos_t pos); \
_funcspecs bool _C##_range_empty(_C##_range_t range); \
_funcspecs _T _C##_front(_C##_t *list); \
_funcspecs _T _C##_back(_C##_t *list); \
_funcspecs _T _C##_get(_C##_pos_t pos); \
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos); \
_funcspecs void _C##_set(_C##_pos_t pos, _T val); \
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *list, _T val); \
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *list, _T val); \
_funcspecs void _C##_remove_front(_C##_t *list); \
_funcspecs void _C##_remove_back(_C##_t *list); \
_funcspecs void _C##_move_front(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos); \
_funcspecs void _C##_move_back(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos); \
_funcspecs void _C##_splice_front(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range); \
_funcspecs void _C##_splice_back(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range);

#define GCL_GENERATE_LIST_LONG_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs void init_##_C(struct _C *list, void (*destroy_elem)(_T)) \
{ \
	*list = (struct _C) { \
		.end = { \
			.next = (struct _C##_node *) &list->end, \
			.prev = (struct _C##_node *) &list->end  \
		}, \
		.destroy_elem = destroy_elem \
	}; \
} \
\
_funcspecs void destroy_##_C(struct _C *list) \
{ \
	struct _C##_node *node, *tmp; \
\
	if (list->destroy_elem) { \
		_gcl_list_for_each_node(node, list, _C) \
			list->destroy_elem(node->elem); \
	} \
\
	_gcl_list_for_each_node_safe(node, tmp, list, _C) \
		free(node); \
} \
\
_funcspecs _C##_pos_t _C##_insert(_C##_t *list, _C##_pos_t pos, _T val) \
{ \
	struct _C##_node *node = gcl_malloc(sizeof(*node)); \
\
	node->elem = val; \
	_C##_link_nodes(pos->prev, node); \
	_C##_link_nodes(node, pos); \
	return node; \
} \
\
_funcspecs _C##_pos_t _C##_release(_C##_t *list, _C##_pos_t pos) \
{ \
	assert(pos != _C##_end(list)); \
\
	_C##_pos_t next = pos->next; \
	_C##_unlink_node(pos); \
	free(pos); \
	return next; \
} \
\
_funcspecs _C##_pos_t _C##_remove(_C##_t *list, _C##_pos_t pos) \
{ \
	assert(pos != _C##_end(list)); \
\
	if (list->destroy_elem) \
		list->destroy_elem(pos->elem); \
\
	return _C##_release(list, pos); \
} \
\
_funcspecs void _C##_clear(_C##_t *list) \
{ \
	struct _C##_node *node, *tmp; \
\
	if (list->destroy_elem) { \
		_gcl_list_for_each_node(node, list, _C) \
			list->destroy_elem(node->elem); \
	} \
\
	_gcl_list_for_each_node_safe(node, tmp, list, _C) \
		free(node); \
\
	init_##_C(list, list->destroy_elem); \
} \
\
_funcspecs void _C##_move(_C##_t *dest_list, _C##_pos_t dest_pos, _C##_t *src_list, _C##_pos_t src_pos) \
{ \
	assert(src_pos != _C##_end(src_list)); \
\
	_C##_unlink_node(src_pos); \
	_C##_link_nodes(dest_pos->prev, src_pos); \
	_C##_link_nodes(src_pos, dest_pos); \
} \
\
_funcspecs void _C##_splice(_C##_t *dest_list, _C##_pos_t pos, _C##_t *src_list, _C##_range_t range) \
{ \
	_C##_link_nodes(range.end->prev, pos); \
	_C##_link_nodes(range.begin->prev, range.end); \
	_C##_link_nodes(pos->prev, range.begin); \
}

#define GCL_GENERATE_LIST_SHORT_FUNCTION_DEFS(_C, _T, _funcspecs) \
\
_funcspecs void _C##_link_nodes(struct _C##_node *prev, struct _C##_node *next) \
{ \
	prev->next = next; \
	next->prev = prev; \
} \
\
_funcspecs void _C##_unlink_node(struct _C##_node* node) \
{ \
	_C##_link_nodes(node->prev, node->next); \
} \
\
_funcspecs bool _C##_empty(_C##_t *list) \
{ \
	return _C##_begin(list) == _C##_end(list); \
} \
\
_funcspecs _C##_pos_t _C##_begin(_C##_t *list) \
{ \
	return list->end.next; \
} \
\
_funcspecs _C##_pos_t _C##_end(_C##_t *list) \
{ \
	return (struct _C##_node *) &list->end; \
} \
\
_funcspecs bool _C##_at_begin(_C##_t *list, _C##_pos_t pos) \
{ \
	return pos == _C##_begin(list); \
} \
\
_funcspecs bool _C##_at_end(_C##_t *list, _C##_pos_t pos) \
{ \
	return pos == _C##_end(list); \
} \
\
_funcspecs _C##_pos_t _C##_next(_C##_pos_t pos) \
{ \
	return pos->next; \
} \
\
_funcspecs _C##_pos_t _C##_prev(_C##_pos_t pos) \
{ \
	return pos->prev; \
} \
\
_funcspecs void _C##_forward(_C##_pos_t *pos) \
{ \
	*pos = (*pos)->next; \
} \
\
_funcspecs void _C##_backward(_C##_pos_t *pos) \
{ \
	*pos = (*pos)->prev; \
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
_funcspecs _C##_range_t _C##_all(_C##_t *list) \
{ \
	return (struct _C##_range) { _C##_begin(list), _C##_end(list) }; \
} \
\
_funcspecs _C##_range_t _C##_range_from_pos(_C##_t *list, _C##_pos_t pos) \
{ \
	return (struct _C##_range) { pos, _C##_end(list) }; \
} \
\
_funcspecs _C##_range_t _C##_range_to_pos(_C##_t *list, _C##_pos_t pos) \
{ \
	return (struct _C##_range) { _C##_begin(list), pos }; \
} \
\
_funcspecs bool _C##_range_empty(_C##_range_t range) \
{ \
	return range.begin == range.end; \
} \
\
_funcspecs _T _C##_front(_C##_t *list) \
{ \
	assert(!_C##_empty(list)); \
	return _C##_begin(list)->elem; \
} \
\
_funcspecs _T _C##_back(_C##_t *list) \
{ \
	assert(!_C##_empty(list)); \
	return _C##_end(list)->prev->elem; \
} \
\
_funcspecs _T _C##_get(_C##_pos_t pos) \
{ \
	return pos->elem; \
} \
\
_funcspecs _T *_C##_get_ptr(_C##_pos_t pos) \
{ \
	return &pos->elem; \
} \
\
_funcspecs void _C##_set(_C##_pos_t pos, _T val) \
{ \
	pos->elem = val; \
} \
\
_funcspecs _C##_pos_t _C##_insert_front(_C##_t *list, _T val) \
{ \
	return _C##_insert(list, _C##_begin(list), val); \
} \
\
_funcspecs _C##_pos_t _C##_insert_back(_C##_t *list, _T val) \
{ \
	return _C##_insert(list, _C##_end(list), val); \
} \
\
_funcspecs void _C##_remove_front(_C##_t *list) \
{ \
	assert(!_C##_empty(list)); \
	_C##_remove(list, _C##_begin(list)); \
} \
\
_funcspecs void _C##_remove_back(_C##_t *list) \
{ \
	assert(!_C##_empty(list)); \
	_C##_remove(list, _C##_end(list)->prev); \
} \
\
_funcspecs void _C##_move_front(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos) \
{ \
	assert(pos != _C##_end(src_list)); \
	_C##_move(dest_list, _C##_begin(dest_list), src_list, pos); \
} \
\
_funcspecs void _C##_move_back(_C##_t *dest_list, _C##_t *src_list, _C##_pos_t pos) \
{ \
	assert(pos != _C##_end(src_list)); \
	_C##_move(dest_list, _C##_end(dest_list), src_list, pos); \
} \
\
_funcspecs void _C##_splice_front(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range) \
{ \
	_C##_splice(dest_list, _C##_begin(dest_list), src_list, range); \
} \
\
_funcspecs void _C##_splice_back(_C##_t *dest_list, _C##_t *src_list, _C##_range_t range) \
{ \
	_C##_splice(dest_list, _C##_end(dest_list), src_list, range); \
}

#endif

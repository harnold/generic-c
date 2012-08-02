/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#ifndef GCL_ALG_H
#define GCL_ALG_H

#define gcl_for_each_pos(_C, pos, range) \
    for ((pos) = _C##_range_begin(range); \
         (pos) != _C##_range_end(range); \
         _C##_forward(&(pos)))

#define gcl_for_each_pos_rev(_C, pos, tmp, range) \
    for ((tmp) = _C##_range_end(range); \
         (tmp) != _C##_range_begin(range) ? (pos) = _C##_prev(tmp), 1 : 0; \
         _C##_backward(&(tmp)))

#define gcl_for_each_pos_indexed(_C, i, pos, range) \
    for ((pos) = _C##_range_begin(range), (i) = 0; \
         (pos) != _C##_range_end(range); \
         _C##_forward(&(pos)), (i)++)

#define gcl_swap(_C, pos1, pos2) \
    do { \
        _C##_elem_t _tmp = _C##_get(pos1); \
        _C##_set(pos1, _C##_get(pos2)); \
        _C##_set(pos2, _tmp); \
    } while (0)

#define gcl_for_each(_C, range, f) \
    do { \
        _C##_pos_t _pos; \
        gcl_for_each_pos(_C, _pos, range) \
            (f)(_C##_get(_pos)); \
    } while (0)

#define gcl_find(_C, range, val, pos) \
    do { \
        gcl_for_each_pos(_C, *(pos), range) { \
            if (_C##_get(*(pos)) == (val)) \
                break; \
        } \
    } while (0)

#define gcl_find_eq(_C, range, eq, val, pos) \
    do { \
        gcl_for_each_pos(_C, *(pos), range) { \
            if ((eq)(_C##_get(*(pos)), (val))) \
                break; \
        } \
    } while (0)

#define gcl_find_if(_C, range, pred, pos) \
    do { \
        gcl_for_each_pos(_C, *(pos), range) { \
            if ((pred)(_C##_get(*(pos)))) \
                break; \
        } \
    } while (0)

#define gcl_count(_C, range, val, n) \
    do { \
        _C##_pos_t _pos; \
        *(n) = 0; \
        gcl_for_each_pos(_C, *(_pos), range) { \
            if (_C##_get(*(_pos)) == (val)) \
                (*(n))++; \
        } \
    } while (0)

#define gcl_count_eq(_C, range, eq, val, n) \
    do { \
        _C##_pos_t _pos; \
        *(n) = 0; \
        gcl_for_each_pos(_C, *(_pos), range) { \
            if ((eq)(_C##_get(*(_pos)), (val))) \
                (*(n))++; \
        } \
    } while (0)

#define gcl_count_if(_C, range, pred, n) \
    do { \
        _C##_pos_t _pos; \
        *(n) = 0; \
        gcl_for_each_pos(_C, *(_pos), range) { \
            if ((pred)(_C##_get(*(_pos)))) \
                (*(n))++; \
        } \
    } while (0)

#define gcl_count_all(_C, range, n) \
    do { \
        _C##_pos_t _pos; \
        *(n) = 0; \
        gcl_for_each_pos(_C, *(_pos), range) \
            (*(n))++; \
    } while (0)

#define gcl_copy_front(_C1, range, _C2, cont) \
    do { \
        _C1##_pos_t _pos; \
        gcl_for_each_pos(_C1, _pos, range) { \
            _C2##_insert_front(cont, _C1##_get(_pos)); \
        } \
    } while (0)

#define gcl_copy_back(_C1, range, _C2, cont) \
    do { \
        _C1##_pos_t _pos; \
        gcl_for_each_pos(_C1, _pos, range) { \
            _C2##_insert_back(cont, _C1##_get(_pos)); \
        } \
    } while (0)

#define gcl_copy(_C1, range, _C2, cont, insert_pos) \
    do { \
        _C1##_pos_t _pos; \
        gcl_for_each_pos(_C1, _pos, range) { \
            insert_pos = _C2##_insert(cont, insert_pos, _C1##_get(_pos)); \
        } \
    } while (0)

#define gcl_fill(_C, range, val) \
    do { \
        _C##_pos_t _pos; \
        gcl_for_each_pos(_C, _pos, range) { \
            _C##_set(_pos, val); \
        } \
    } while(0)

#define gcl_generate(_C, range, generate_elem) \
    do { \
        int _i; \
        _C##_pos_t _pos; \
        gcl_for_each_pos_indexed(_C, _i, _pos, range) { \
            _C##_set(_pos, (generate_elem)(_i)) \
        } \
    } while (0)

#endif

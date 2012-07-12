#ifndef GC_ALG_H
#define GC_ALG_H

#define gc_for_each_pos(C, pos, range) \
    for ((pos) = C##_range_begin(range); \
         (pos) != C##_range_end(range); \
         C##_forward(&(pos)))

#define gc_for_each_pos_rev(C, pos, tmp, range) \
    for ((tmp) = C##_range_end(range); \
         (tmp) != C##_range_begin(range) ? (pos) = C##_prev(tmp), 1 : 0; \
         C##_backward(&(tmp)))

#define gc_for_each_pos_indexed(C, i, pos, range) \
    for ((pos) = C##_range_begin(range), (i) = 0; \
         (pos) != C##_range_end(range); \
         C##_forward(&(pos)), (i)++)

#define gc_for_each_pos_safe(C, pos, tmp, range) \
    for ((pos) = C##_range_begin(range), (tmp) = C##_next(pos); \
         (pos) != C##_range_end(range); \
         (pos) = (tmp), C##_forward(&(tmp)))

#define gc_swap(C, pos1, pos2) \
    do { \
        C##_elem_t tmp = C##_get(pos1); \
        C##_set(pos1, C##_get(pos2)); \
        C##_set(pos2, tmp); \
    } while (0)

#define gc_for_each(C, range, f) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C, pos, range) \
            (f)(C##_get(pos)); \
    } while (0)

#define gc_find(C, range, val, pos) \
    do { \
        gc_for_each_pos(C, *(pos), range) { \
            if (C##_get(*(pos)) == (val)) \
                break; \
        } \
    } while (0)

#define gc_find_eq(C, range, eq, val, pos) \
    do { \
        gc_for_each_pos(C, *(pos), range) { \
            if ((eq)(C##_get(*(pos)), (val))) \
                break; \
        } \
    } while (0)

#define gc_find_if(C, range, pred, pos) \
    do { \
        gc_for_each_pos(C, *(pos), range) { \
            if ((pred)(C##_get(*(pos)))) \
                break; \
        } \
    } while (0)

#define gc_count(C, range, val, n) \
    do { \
        C##_pos_t pos; \
        *(n) = 0; \
        gc_for_each_pos(C, *(pos), range) { \
            if (C##_get(*(pos)) == (val)) \
                (*(n))++; \
        } \
    } while (0)

#define gc_count_eq(C, range, eq, val, n) \
    do { \
        C##_pos_t pos; \
        *(n) = 0; \
        gc_for_each_pos(C, *(pos), range) { \
            if ((eq)(C##_get(*(pos)), (val))) \
                (*(n))++; \
        } \
    } while (0)

#define gc_count_if(C, range, pred, n) \
    do { \
        C##_pos_t pos; \
        *(n) = 0; \
        gc_for_each_pos(C, *(pos), range) { \
            if ((pred)(C##_get(*(pos)))) \
                (*(n))++; \
        } \
    } while (0)

#define gc_copy_front(C1, range, C2, cont, copy_elem) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C1, pos, range) { \
            C2##_insert_front(cont, (copy_elem)(C1##_get(pos))); \
        } \
    } while (0)

#define gc_copy_front(C1, range, C2, cont, copy_elem) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C1, pos, range) { \
            C2##_insert_front(cont, (copy_elem)(C1##_get(pos))); \
        } \
    } while (0)

#define gc_copy_before(C1, range, C2, cont, pos, copy_elem) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C1, pos, range) { \
            C2##_insert_before(cont, pos, (copy_elem)(C1##_get(pos))); \
        } \
    } while (0)

#define gc_copy_after(C1, range, C2, cont, pos, copy_elem) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C1, pos, range) { \
            C2##_insert_after(cont, pos, (copy_elem)(C1##_get(pos))); \
        } \
    } while (0)

#define gc_fill(C, range, val) \
    do { \
        C##_pos_t pos; \
        gc_for_each_pos(C, pos, range) { \
            C##_set(pos, val); \
        } \
    } while(0)

#define gc_generate(C, range, generate_elem) \
    do { \
        int i; \
        C##_pos_t pos; \
        gc_for_each_pos_indexed(C, i, pos, range) { \
            C##_set(pos, (generate_elem)(i)) \
        } \
    } while (0)

#endif

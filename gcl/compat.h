#ifndef GCL_COMPAT_H
#define GCL_COMPAT_H

/* Test compiler features. */

#if defined(__GNUC__)

#ifndef _GNUC_VERSION
#define _GNUC_VERSION   (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

#define _GCL_HAS_FEATURE_STATIC_ASSERT  (_GNUC_VERSION >= 406)
#define _GCL_HAS_ATTRIBUTE_NORETURN     (_GNUC_VERSION >= 205)
#define _GCL_HAS_ATTRIBUTE_FORMAT       1
#define _GCL_HAS_ATTRIBUTE_MALLOC       1

#elif defined(__clang__)

#define _GCL_HAS_FEATURE_STATIC_ASSERT  __has_feature(__c_static_assert__)
#define _GCL_HAS_ATTRIBUTE_NORETURN     __has_attribute(__noreturn__)
#define _GCL_HAS_ATTRIBUTE_FORMAT       __has_attribute(__format__)
#define _GCL_HAS_ATTRIBUTE_MALLOC       __has_attribute(__malloc__)

#else /* Unknown compiler */

#define _GCL_HAS_FEATURE_STATIC_ASSERT  0
#define _GCL_HAS_ATTRIBUTE_NORETURN     0
#define _GCL_HAS_ATTRIBUTE_FORMAT       0
#define _GCL_HAS_ATTRIBUTE_MALLOC       0

#define __attribute__(x)

#endif

/* C99 features not supported by all compilers. */

#if !_GCL_HAS_FEATURE_STATIC_ASSERT
#define _Static_assert(cond, msg) \
    ((void) sizeof(struct { int __static_assert_failure: (cond) ? 1 : -1; }))
#endif

#ifndef static_assert
#define static_assert _Static_assert
#endif

/* Function attributes. */

#if _GCL_HAS_ATTRIBUTE_FORMAT
#define GCL_FORMAT_STRING(pos) \
    __attribute__((__format__ (__printf__, pos, pos + 1)))
#else
#define GCL_FORMAT_STRING(pos)
#endif

#if _GCL_HAS_ATTRIBUTE_NORETURN
#define GCL_NO_RETURN   __attribute__((__noreturn__))
#else
#define GCL_NO_RETURN
#endif

#if _GCL_HAS_ATTRIBUTE_MALLOC
#define GCL_MALLOC_FUNC __attribute__((__malloc__))
#else
#define GCL_MALLOC_FUNC
#endif

#endif

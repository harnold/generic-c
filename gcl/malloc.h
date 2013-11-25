#ifndef GCL_MALLOC_H
#define GCL_MALLOC_H

#include <gcl/compat.h>

#include <stdlib.h>

#ifndef gcl_malloc
#define gcl_malloc(size)        _gcl_malloc(size)
#endif

#ifndef gcl_realloc
#define gcl_realloc(ptr, size)  _gcl_realloc(ptr, size)
#endif

void *_gcl_malloc(size_t size) GCL_MALLOC_FUNC;
void *_gcl_realloc(void * ptr, size_t size);

#endif

/*
 * Copyright 2012 Holger Arnold.
 *
 * Licensed under a modified BSD license.
 * See the accompanying LICENSE file for details.
 */

#include <gcl/malloc.h>
#include <gcl/error.h>

#include <errno.h>

void *_gcl_malloc(size_t size)
{
    if (size == 0)
        return NULL;

    void *ptr = malloc(size);

    if (!ptr)
        gcl_error(errno, "malloc() failed");

    return ptr;
}

void *_gcl_realloc(void *ptr, size_t size)
{
    if (size == 0) {
        free(ptr);
        return NULL;
    }

    void *new_ptr = realloc(ptr, size);

    if (!new_ptr)
        gcl_error(errno, "realloc() failed");

    return new_ptr;
}

#ifndef __MEM_ALLOCATORS__
#define __MEM_ALLOCATORS__

#include <stddef.h>  // для size_t

void *arthur_malloc(size_t size);

void arthur_free(void *p);

void *arthur_calloc(size_t number, size_t size);

void *arthur_realloc(void *p, size_t size);


#endif
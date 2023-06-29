#ifndef __MEM_ALLOCATORS__
#define __MEM_ALLOCATORS__

#include <stddef.h>  // для size_t

void *custom_malloc(size_t size);

void custom_free(void *p);

void *custom_calloc(size_t number, size_t size);

void *custom_realloc(void *p, size_t size);


#endif
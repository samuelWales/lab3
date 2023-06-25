#ifndef __ALLOCS__
#define __ALLOCS__

#include <stddef.h>  // для size_t

void *malloc(size_t size);

void free(void *p);

void *calloc(size_t number, size_t size);

void *realloc(void *p, size_t size);


#endif
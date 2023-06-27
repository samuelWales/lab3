#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <pthread.h>

#include "mem_allocators.h"
#include "allocs.h"


static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;


void *malloc(size_t size)
{
    pthread_mutex_lock(&alloc_mutex);
    void *ans = arthur_malloc(size);
    pthread_mutex_unlock(&alloc_mutex);
    return ans;
}


void *calloc(size_t number, size_t size)
{
    pthread_mutex_lock(&alloc_mutex);
    void *ans = arthur_calloc(number, size);
    pthread_mutex_unlock(&alloc_mutex);
    return ans;
}


void free(void *p)
{
    pthread_mutex_lock(&alloc_mutex);
    arthur_free(p);
    pthread_mutex_unlock(&alloc_mutex);
}


void *realloc(void *p, size_t size){
    pthread_mutex_lock(&alloc_mutex);
    void *res = arthur_realloc(p, size);
    pthread_mutex_unlock(&alloc_mutex);
    return res;
}

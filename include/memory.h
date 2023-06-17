#include <stdint.h>

#ifndef _MEMORY_H
#define _MEMORY_H

void memoryInit(void);

void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void free(void* ptr);

void* memcpy(void* dst, const void* src, size_t size);

#endif
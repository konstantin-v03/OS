#ifndef KALLOC_H
#define KALLOC_H

#include <stdint.h>

void init_kheap(void* heap_start_, uint32_t initial_size_);

void* kalloc(uint32_t size);

void kfree(void* addr);

#endif

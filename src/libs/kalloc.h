#ifndef KALLOC_H
#define KALLOC_H

#include <stdint.h>

#include "../paging/paging.h"

void init_kheap(uint32_t heap_start_, page_directory_t* dir);

void* kalloc(uint32_t size);

void kfree(void* addr);

#endif

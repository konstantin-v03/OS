#ifndef FRAME_ALLOC_H
#define FRAME_ALLOC_H

#include <stdint.h>
#include "paging.h"

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

void init_frames_bit_set(uint32_t nframes, uint32_t* frames);

void set_frame(uint32_t frame_addr);

void clear_frame(uint32_t frame_addr);

uint32_t test_frame(uint32_t frame_addr);

uint32_t first_frame();

void alloc_frame(page_t* page, int is_kernel, int is_writeable);

void free_frame(page_t* page);
	
#endif

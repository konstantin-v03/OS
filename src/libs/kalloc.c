#include <stdint.h>
#include "../paging/frame_alloc.h"
#include "../paging/paging.h"
#include "../libs/shell.h"


#define HEAP_INITIAL_SIZE    0x200000 //heap initial size 2Mb

#define HEAP_EXPAND_ADDITION 0x100000

#define HEAP_MAX_SIZE        0x1000000 //heap max size 16.7Mb

#define NULL ((void*) -1)

uint32_t heap_start;

uint32_t heap_curr_size;

uint32_t heap_curr_pos;

void* brk_val;

page_directory_t* dir;

void init_kheap(uint32_t heap_start_, page_directory_t* dir_) {
    heap_start = heap_start_;
    heap_curr_pos = 0;
    dir = dir_;
    
    for (uint32_t i = heap_start; i < heap_start + HEAP_INITIAL_SIZE; i += 0x1000) {
        alloc_frame(get_page(i, 1, dir), 0, 0);
    } 

    heap_curr_size = HEAP_INITIAL_SIZE;
    brk_val = (void*)heap_start;
    
    return;
}


// temporary variants sbrk() and brk() just for implementation kernel heap

int brk(void* addr) {
    uint32_t addr_ = (uint32_t) addr;

    if (addr_ >= heap_start) {
        if (addr_ < heap_start + heap_curr_size) {
            brk_val = addr;
            return 0;
        } else if (addr_ < heap_start + HEAP_MAX_SIZE) {
            uint32_t heap_new_size = heap_curr_size;
            while (heap_start + heap_new_size < addr_) {
                heap_new_size += HEAP_EXPAND_ADDITION;
            }
            for (uint32_t i = heap_start + heap_curr_size; i < heap_start + heap_new_size; i += 0x1000) {
                alloc_frame(get_page(i, 1, dir), 0, 0);
            }
            brk_val = addr;
            return 0;
        } else {
            shell_print_a("\nHEAP OF THE BOUND(heap max size 16.7Mb)\nStoping the CPU!\n\0", RED_ON_WHITE);
            asm volatile("cli");
            asm volatile("hlt");
        }
    }

    return -1;
}

void* sbrk(uint32_t increment) {
    void* last_brk = brk_val;
    
    uint32_t addr_ = (uint32_t)brk_val + increment;

    if (addr_ >= heap_start) {
        if (addr_ < heap_start + heap_curr_size) {
            brk_val = (void*)addr_;
            return last_brk;
        } else if (addr_ < heap_start + HEAP_MAX_SIZE) {
            uint32_t heap_new_size = heap_curr_size;
            while (heap_start + heap_new_size < addr_) {
                heap_new_size += HEAP_EXPAND_ADDITION;
            }
            for (uint32_t i = heap_start + heap_curr_size; i < heap_start + heap_new_size; i += 0x1000) {
                alloc_frame(get_page(i, 1, dir), 0, 0);
            }
            brk_val = (void*)addr_;
            return last_brk;
        } else {
            shell_print_a("\nHEAP OF THE BOUND(heap max size 16.7Mb)\nStoping the CPU!\n\0", RED_ON_WHITE);
            asm volatile("cli");
            asm volatile("hlt");
        }
    }

    return NULL;
}

#define align4(x) (((((x) - 1) >> 2) << 2) + 4)

#define BLOCK_SIZE (sizeof(struct s_block) - 1)

typedef struct s_block s_block;

struct s_block  {
    uint32_t size;
    s_block* next;
    s_block* prev;
    int free;
    void* ptr;
    char data[1];
} __attribute__((packed));

int valid_block(void* ptr);

void split_block(s_block* b, uint32_t size);

void copy_block(s_block* src, s_block* dest);

s_block* fusion(s_block* b);

s_block* find_block(s_block** last, uint32_t size);

s_block* extend_heap(s_block* last, uint32_t size);

s_block* get_block(void* ptr);

s_block* base = NULL;

void* kalloc(uint32_t size) {
    s_block* b;
    s_block* last;

    size = align4(size);

    if (base != NULL) {
        last = base;

        b = find_block(&last, size);
        
        if (b != NULL) {
            if ((b->size - size) >= BLOCK_SIZE + 4) {
                split_block(b, size);
            }

            b->free = 0;
        } else {
            b = extend_heap(last, size);
            
            if (b == NULL) {  
                return NULL; 
            }
        }
    } else {
        b = extend_heap(NULL, size);
        
        if (b == NULL) {
            return NULL;
        }
    
        base = b;
    }

    return &(b->data);
}

void kfree(void* ptr) {
    s_block* b;

    if (valid_block(ptr)) {
        b = get_block(ptr);
        b->free = 1;
        
        if (b->prev != NULL && b->prev->free) {
            b = fusion(b->prev);    
        }
        
        if (b->next != NULL) {
            fusion(b);
        } else {
            if (b->prev != NULL) {
                b->prev->next = NULL;
            } else {
                base = NULL;
            }
            brk(b);
        }
    }
}

int valid_block(void* ptr){
    if (base != NULL) {
        if (ptr > base && ptr < sbrk(0)) {
            s_block* b = get_block(ptr);
            return (ptr == b->ptr);
        }
    }

    return 0;
}

void split_block(s_block* b, uint32_t size) {
    s_block* new;
    
    new = (void*)(&(b->data) + size);
    new->size = b->size - size - BLOCK_SIZE;
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    new->ptr = new->data;
    
    b->size = size;
    b->next = new;

    if (new->next != NULL) {
        new->next->prev = new;
    }
}

void copy_block(s_block* src, s_block* dest) {
    char* srcdata = src->ptr;
    char* destdata = dest->ptr;

    for (uint32_t i = 0; i < src->size && i < dest->size; i++) {
        destdata[i] = srcdata[i];
    } 
}

s_block* fusion(s_block* b) {
    if (b->next != NULL && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;   

        if (b->next != NULL) {
            b->next->prev = b;            
        }
    }

    return b;
}

s_block* get_block(void* ptr) {
    return (s_block*)((char*)ptr - BLOCK_SIZE);
}

s_block* find_block(s_block** last, uint32_t size) {
    s_block* b = base;   
    
    while(b != NULL && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    } 

    return b;    
}

s_block* extend_heap(s_block* last, uint32_t size) {
    s_block* b = sbrk(0);

    if(sbrk(BLOCK_SIZE + size) == NULL) {
        return NULL;
    }
    
    b->size = size;
    b->next = NULL;
    b->prev = last;
    b->ptr = &(b->data);
    
    if (last != NULL) {
        last->next = b;
    }    
    
    b->free = 0;
    
    return b;
}


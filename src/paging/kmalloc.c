#include <stdint.h>

uint32_t placement_address;

uint32_t kmalloc_a(uint32_t size) {
    if ((placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

uint32_t kmalloc_p(uint32_t size, uint32_t* phys) {
    if (phys) {
        * phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

uint32_t kmalloc_ap(uint32_t size, uint32_t* phys) {
    if ((placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }

    if (phys) {
        * phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

uint32_t kmalloc(uint32_t size) {
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

void set_placement_addr(uint32_t addr) {
    placement_address = addr;
    return;
}

uint32_t get_placement_addr() {
    return placement_address;
}

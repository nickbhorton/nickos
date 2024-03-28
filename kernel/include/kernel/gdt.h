#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#include <stdint.h>

typedef struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint32_t base;
} gdtr_t;

typedef struct __attribute__((packed)) gdt_segment_descriptor {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access_byte;
    uint8_t flags_limit_high;
    uint8_t base_high;
} gdt_segment_descriptor_t;

/*
 * @breif uses the sgdt argument to get the 6 bytes loaded into gdtr_t passed in
 */
int read_gdtr(gdtr_t* gdt);

#endif

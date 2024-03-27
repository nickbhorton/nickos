#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

#include <stdbool.h>
#include <stdint.h>

#define VITRUAL_MEMORY_OFFSET 0xc0000000

typedef struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint32_t base;
} gdtr_t;


void pokel(uint32_t* address, uint32_t value);
void pokew(uint16_t* address, uint16_t value);
void pokeb(uint8_t* address, uint8_t value);

uint32_t peekl(uint32_t* address);
uint16_t peekw(uint16_t* address);
uint8_t peekb(uint8_t* address);

static inline gdtr_t read_gdtr()
{
    gdtr_t result = {0, 0};

    asm("sgdt %0" : "=g"(result));

    return result;
}

#endif

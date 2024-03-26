#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

#include "kernel/serial.h"
#include <stdbool.h>
#include <stdint.h>

#include <kernel/register.h>

#define VITRUAL_MEMORY_OFFSET 0xc0000000

void pokel(uint32_t address, uint32_t value);
void pokew(uint32_t address, uint16_t value);
void pokeb(uint32_t address, uint8_t value);

typedef struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint32_t base;
} gdtr_t;

static inline uint32_t peekl(uint32_t address)
{
    uint32_t ret;
    asm volatile("movl  %1, %%eax\n\t"
                 "movl  (%%eax), %0\n\t"
                 : "=r"(ret)
                 : "g"(address)
                 : "eax");
    return ret;
}

static inline uint8_t peekb(uint32_t address)
{
    uint8_t ret;
    asm volatile("movl  %1, %%eax\n\t"
                 "movb  (%%eax), %0\n\t"
                 : "=r"(ret)
                 : "g"(address)
                 : "eax");
    return ret;
}

// TODO: finish this function
static inline uint16_t peekb_checked(uint16_t page_directory_index,
                                     uint16_t page_table_index,
                                     uint16_t page_offset)
{
    if (page_directory_index & 0b1111110000000000)
    {
        serial_print("peekb_checked() error: page directory index was more than 10 bits\n");
        return 0x0100;
    }
    else if (page_table_index & 0b1111110000000000)
    {
        serial_print("peekb_checked() error: page table index was more than 10 bits\n");
        return 0x0100;
    }
    else if (page_offset & 0b1111000000000000)
    {
        serial_print("peekb_checked() error: page offset was more than 12 bits\n");
        return 0x0100;
    }
    //uint32_t* page_dir_address = (uint32_t*)get_page_directory_address_p();
    //uint32_t page_dir_entry = *(page_dir_address + page_directory_index);
    //serial_print("page_dir_entry: %X\n", page_dir_entry);

    return 0x0;
}

static inline gdtr_t read_gdtr()
{
    gdtr_t result = {0, 0};

    asm("sgdt %0" : "=g"(result));

    return result;
}

#endif

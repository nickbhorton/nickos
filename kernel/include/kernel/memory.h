#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

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

static inline gdtr_t read_gdtr()
{
    gdtr_t result = {0, 0};

    asm("sgdt %0" : "=g"(result));

    return result;
}

static inline gdtr_t read_idtr()
{
    gdtr_t result = {0, 0};

    asm("sidt %0" : "=g"(result));

    return result;
}

static inline uint32_t physical_to_virtual(uint32_t address)
{
    return address | VITRUAL_MEMORY_OFFSET;
}

static inline uint32_t virtual_to_physical(uint32_t address)
{
    return address & ~VITRUAL_MEMORY_OFFSET;
}

static inline uint32_t get_page_directory_address_p()
{
    // register cr3 contains the physical memory address of the page directory
    return read_cr3();
}

static inline uint32_t get_page_directory_entry(uint16_t dir_index)
{
    return peekl(physical_to_virtual(get_page_directory_address_p()) +
                 (dir_index * sizeof(uint32_t)));
}

static inline bool is_page_table_present(uint16_t dir_index)
{
    return get_page_directory_entry(dir_index) & 0x00000001;
}

static inline uint32_t get_page_table_address_p(uint32_t page_directory_entry)
{
    return page_directory_entry & 0xFFFFF000;
}

static inline uint32_t get_page_table_entry(uint16_t dir_index,
                                            uint16_t table_index)
{
    return peekl(physical_to_virtual(
        get_page_table_address_p(get_page_directory_entry(dir_index)) +
        (table_index * sizeof(uint32_t))));
}

static inline uint32_t get_page_address_p(uint32_t page_table_entry)
{
    return page_table_entry & 0xFFFFF000;
}

static inline bool is_page_present(uint16_t dir_index, uint16_t table_index)
{
    return get_page_table_entry(dir_index, table_index) & 0x00000001;
}

#endif

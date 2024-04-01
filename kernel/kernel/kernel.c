#include "kernel/debug_print.h"
#include "kernel/gdt.h"
#include "kernel/paging.h"
#include <kernel/idt.h>
#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/tty.h>

#include <stdio.h>
#include <string.h>

#include <tests/tests.h>

__attribute__((constructor)) void kernel_init(void)
{
    terminal_initialize();
    initialize_idt();
    char buf[80];
    int bytes_written = snprintf(buf, 80, "%d", serial_initialize());
    if (bytes_written > 0)
    {
        terminal_write(buf, bytes_written);
    }
    else
    {
        const char* msg = "snprintf() returned negitive int\n";
        terminal_write(msg, strlen(msg));
    }
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        serial_printf("magic number was not the multiboot magic number\n");
    }
    // identity maping the mbd
    page_tab_entry_t* pte_ptr =
        get_pte_ptr(((uint32_t)mbd) >> 22, ((uint32_t)mbd) >> 12);
    *pte_ptr = ((uint32_t)mbd) | 0x3;

    /* Check bit 6 to see if we have a valid memory map */
    if (!(mbd->flags >> 6 & 0x1))
    {
        serial_printf("invalid memory map given by GRUB bootloader");
    }
    /* Loop through the memory map and display the values */
    uint32_t i;
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*)(mbd->mmap_addr + i);
        serial_printf("size: %x | addr_l %x | addr_h %x | len_l %x | len_h "
                      "%x | type %x\n",
                      mmmt->size, mmmt->addr_l, mmmt->addr_h, mmmt->len_l,
                      mmmt->len_h, mmmt->type);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            /*
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
    }
    serial_printf("boot_page_directory\n");
    for (size_t i = 0; i < 0x0400; i++)
    {
        page_dir_entry_t* boot_page_directory_entry = get_pde_ptr(i);
        // if present flag set
        if (pde_p_test(*boot_page_directory_entry))
        {
            serial_printf("0x%x -> ", (uint32_t)boot_page_directory_entry);
            print_pde(*boot_page_directory_entry);
        }
    }

    serial_printf("boot_page_table1\n");
    for (size_t i = 0; i < 0x0400; i++)
    {
        page_tab_entry_t* boot_page_table_1_entry = get_pte_ptr(0, i);
        if (pte_p_test(*boot_page_table_1_entry))
        {
            serial_printf("0x%x -> ", (uint32_t)boot_page_table_1_entry);
            print_pte(*boot_page_table_1_entry);
        }
    }

    gdtr_t gdt = {};
    read_gdtr(&gdt);
    serial_printf("gdt: %x, %x\n", gdt.base, gdt.limit);
    serial_print_memory((uint8_t*)gdt.base, gdt.limit, 8);
}

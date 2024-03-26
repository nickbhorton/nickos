#include "kernel/paging.h"
#include <kernel/idt.h>
#include <kernel/memory.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/tty.h>

#include <stdio.h>
#include <string.h>

__attribute__((constructor)) void kernel_init(void)
{
    terminal_initialize();
    idt_init();
    char buf[80];
    int bytes_written = snprintf(
        buf, 80, "serial_initialize() return value: %d", serial_initialize());
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

void kernel_main(void)
{
    size_t max_len = 1000;
    char str[max_len];
    int written = pde_summary_string(get_pde(0), str, max_len);
    serial_write(str, written);
    serial_print("\n");
    written = pde_summary_string(get_pde(768), str, max_len);
    serial_write(str, written);
    serial_print("\n");
    for (size_t i = 0; i < 1023; i++)
    {
        written = pte_summary_string(get_pte(768, i), str, max_len);
        serial_write(str, written);
        serial_print(" ");
    }
    /*
    uint32_t page_dir_address =
        physical_to_virtual(get_page_directory_address_p());
    serial_print("page directory\n");
    serial_print_memory(page_dir_address, 0x1000, 8);
    serial_print("\n");

    uint32_t gdt_address = get_page_address_p(get_page_table_entry(768, 1022));
    gdtr_t GDTR = read_gdtr();
    serial_print("GDTR size: %X\nGDTR location: %X\ngdt_address: %X\n",
                 GDTR.limit, GDTR.base, gdt_address);
    idtr_t IDTR = read_idtr();

    uint32_t idt_address = get_page_address_p(get_page_table_entry(768, 1021));
    serial_print("IDTR size: %X\nIDTR location: %X\nidt_address: %X\n\n",
                 IDTR.limit, IDTR.base, idt_address);

    serial_print("### gdt ###\n");
    serial_print_memory(gdt_address, 0x100, 8);
    serial_print("### idt ###\n");
    serial_print_memory(idt_address, 0x100, 8);
    peekb_checked(768, 0, 0);
    */
}

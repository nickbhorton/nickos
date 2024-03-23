#include <kernel/serial.h>
#include <kernel/register.h>
#include <kernel/memory.h>
#include <kernel/tty.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

__attribute__((constructor)) void kernel_init(void)
{
    terminal_initialize();
    char buf[80];
    int bytes_written = snprintf(
        buf, 80, "serial_initialize() return value: %d", serial_initialize());
    if (bytes_written > 0)
    {
        terminal_write(buf, bytes_written);
    }
    else
    {
        const char *msg = "snprintf() returned negitive int\n";
        terminal_write(msg, strlen(msg));
    }
}



void kernel_main(void)
{
    /*
    serial_print_bits_on(read_cr0());
    uint32_t page_dir_address_p = read_cr3();
    uint32_t page_dir_address_v =
        (page_dir_address_p | 0xc0000000);
    serial_print_page(page_dir_address_v);
    uint32_t page_dir_entry_768 =
        peekl((page_dir_address_v + (768 * sizeof(uint32_t))));
    uint32_t page_table1_pysical_address = page_dir_entry_768 & 0xFFFFF000;
    uint32_t page_table1_virtual_address =
        page_table1_pysical_address | 0xc0000000;
    serial_print_page(page_table1_virtual_address);
    */
    serial_print_page(0xC0100000);
}

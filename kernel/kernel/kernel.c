#include <kernel/memory.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/tty.h>

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
    serial_print("ss:%X\n", read_ss());
    serial_print("ds:%X\n", read_ds());
    serial_print("fs:%X\n", read_fs());
    serial_print("gs:%X\n", read_gs());
    serial_print("cs:%X\n", read_cs());
    serial_print("es:%X\n", read_es());
    uint32_t address_of_page_1022_in_page_table_768 = physical_to_virtual(
        get_page_address_p(get_page_table_entry(768, 1022)));
    serial_print("page 1022\n");
    serial_print_page(address_of_page_1022_in_page_table_768);
    gdtr_t GDTR = read_gdtr();
    serial_print("GDTR size: %X\nGDTR location: %X\n", GDTR.limit, GDTR.base);
    serial_print("%d\n", farpeekl(101, (uint32_t *)102));
}

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
    uint32_t gdt_address = physical_to_virtual(
        get_page_address_p(get_page_table_entry(768, 1022)));
    uint32_t idt_address = physical_to_virtual(
        get_page_address_p(get_page_table_entry(768, 1021)));

    pokeb(idt_address, 0xFF);

    serial_print("gdt (page 1022)\n");
    serial_print_page(gdt_address);
    serial_print("idt (page 1021)\n");
    serial_print_page(idt_address);
    gdtr_t IDTR = read_idtr();
    serial_print("IDTR size: %X\nIDTR location: %X\n", IDTR.limit, IDTR.base);
}

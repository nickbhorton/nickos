#include <kernel/memory.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/idt.h>
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

    fill_idt((void *)idt_address, 0 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 1 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 3 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 4 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 5 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 6 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 7 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 8 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 10 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 11 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 12 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 13 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 14 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 16 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 17 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 18 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 19 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 20 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 21 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 28 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 29 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 30 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);
    fill_idt((void *)idt_address, 31 * IDT_GATE_DESCRIPTOR_SIZE, IDT_GATE_TYPE_32_TRAP);

    serial_print("gdt (page 1022)\n");
    serial_print_page(gdt_address);
    serial_print("idt (page 1021)\n");
    serial_print_page(idt_address);
    gdtr_t IDTR = read_idtr();
    serial_print("IDTR size: %X\nIDTR location: %X\n", IDTR.limit, IDTR.base);
    pokel(0x00000001, 0x0);
}

#include "kernel/register.h"
#include "kernel/serial.h"
#include <kernel/idt.h>

idtr_t read_idtr()
{
    idtr_t result = {0, 0};

    asm("sidt %0" : "=g"(result));

    return result;
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idtr_t idtstruct = read_idtr();
    idt_entry_t* idt = (idt_entry_t*)idtstruct.base;
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08; // this value can be whatever offset your
                                  // kernel code selector is in your GDT
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}

void initialize_idt()
{
    for (uint8_t i = 0; i < 32; i++)
    {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
    }
    asm volatile("sti");
}

void handle_page_fault(uint32_t error_code)
{
    serial_print("page fault, code: %X\neax: %d\n", error_code, read_eax());
    if (error_code & (1 << 0))
    {
        serial_print("P ");
    }
    if (error_code & (1 << 1))
    {
        serial_print("W ");
    }
    if (error_code & (1 << 2))
    {
        serial_print("U ");
    }
    if (error_code & (1 << 3))
    {
        serial_print("R ");
    }
    if (error_code & (1 << 4))
    {
        serial_print("i ");
    }
    if (error_code & (1 << 5))
    {
        serial_print("PK ");
    }
    if (error_code & (1 << 6))
    {
        serial_print("SS ");
    }
    if (error_code & (1 << 7))
    {
        serial_print("SGX ");
    }
    serial_print("\n");
    return;
}

#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

#include "kernel/memory.h"
#include "kernel/serial.h"
#include <stdint.h>

#define IDT_GATE_TYPE_32_INTERRUPT 0xE
#define IDT_GATE_TYPE_32_TRAP 0x7

#define IDT_GATE_DESCRIPTOR_SIZE 0x8

typedef struct __attribute__((packed)) idtr {
    uint16_t limit;
    uint32_t base;
} idtr_t;

typedef struct {
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into
                        // CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attributes; see the IDT page
    uint16_t isr_high;  // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

extern void *isr_stub_table[];

static inline idtr_t read_idtr()
{
    idtr_t result = {0, 0};

    asm("sidt %0" : "=g"(result));

    return result;
}

__attribute__((noreturn)) void exception_handler(void);
__attribute__((noreturn)) void isr_stub_14(void);

static inline void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags)
{
    idtr_t idtstruct = read_idtr();
    idt_entry_t *idt = (idt_entry_t *)physical_to_virtual(idtstruct.base);
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08; // this value can be whatever offset your
                                  // kernel code selector is in your GDT
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}

static inline void idt_init()
{
    for (uint8_t i = 0; i < 32; i++)
    {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
    }
    asm volatile ("sti");
}

#endif

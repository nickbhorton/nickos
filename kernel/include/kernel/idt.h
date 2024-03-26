#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

#include <stdint.h>

#define IDT_GATE_TYPE_32_INTERRUPT 0xE
#define IDT_GATE_TYPE_32_TRAP 0x7

#define IDT_GATE_DESCRIPTOR_SIZE 0x8

typedef struct {
    uint16_t isr_low;   // The lower 16 bits of the ISR's address
    uint16_t kernel_cs; // The GDT segment selector that the CPU will load into
                        // CS before calling the ISR
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // Type and attributes; see the IDT page
    uint16_t isr_high;  // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

void fill_idt(void* idt_address, uint16_t offset_into_idt, uint8_t interupt_type);

__attribute__((noreturn))
void exception_handler(void);


#endif

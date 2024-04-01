#include "kernel/debug_print.h"
#include "kernel/gdt.h"
#include "kernel/paging.h"
#include <kernel/idt.h>
#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/pic.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/tty.h>

#include <stdio.h>
#include <string.h>

#include <tests/tests.h>

__attribute__((constructor)) void kernel_init(void)
{
    terminal_initialize();
    idt_initialize();
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

static void wait_input_buffer_clear()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for input buffer to be clear... current status register: %hhx\n", status_reg);
        if (!(status_reg & (1 << 1)))
        {
            //serial_printf("input buffer is clear\n");
            loop = false;
        }
    }
}

static void wait_output_buffer_clear()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for output buffer to be clear... current status register: %hhx\n", status_reg);
        if (!(status_reg & (1 << 0)))
        {
            //serial_printf("output buffer is clear\n");
            loop = false;
        }
    }
}

static void wait_input_buffer_set()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for input buffer to be set... current status register: %hhx\n", status_reg);
        if ((status_reg & (1 << 1)))
        {
            //serial_printf("input buffer is set\n");
            loop = false;
        }
    }
}

static void wait_output_buffer_set()
{
    bool loop = true;
    while (loop)
    {
        uint8_t status_reg = inb(0x64);
        //serial_printf("waiting for output buffer to be set... current status register: %hhx\n", status_reg);
        if ((status_reg & (1 << 0)))
        {
            //serial_printf("output buffer is set\n");
            loop = false;
        }
    }
}

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
    /*
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        serial_printf("magic number was not the multiboot magic number\n");
    }
    // identity maping the mbd
    page_tab_entry_t* pte_ptr =
        get_pte_ptr(((uint32_t)mbd) >> 22, ((uint32_t)mbd) >> 12);
    *pte_ptr = ((uint32_t)mbd) | 0x3;

    // Check bit 6 to see if we have a valid memory map 
    if (!(mbd->flags >> 6 & 0x1))
    {
        serial_printf("invalid memory map given by GRUB bootloader");
    }
    // Loop through the memory map and display the values 
    uint32_t i;
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*)(mbd->mmap_addr + i);
        serial_printf(
            "size: %x | addr_l %x | addr_h %x | len_l %x | len_h "
            "%x | type %x\n",
            mmmt->size, mmmt->addr_l, mmmt->addr_h, mmmt->len_l, mmmt->len_h,
            mmmt->type
        );

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
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

    serial_printf("pic irr: %hx\n", pic_get_irr());
    serial_printf("pic isr: %hx\n", pic_get_isr());
    serial_printf("flags: %x\n", read_flags());
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);
    serial_printf("a1: %hhx\na2: %hhx\n", a1, a2);
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);
    serial_printf("a1: %hhx\na2: %hhx\n", a1, a2);

    irq_clear_mask(1);
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);
    serial_printf("a1: %hhx\na2: %hhx\n", a1, a2);
    serial_printf("flags: %x\n", read_flags());
    */

    
   
    asm("cli");
    pic_disable();
    wait_input_buffer_clear();
    // Disable first PS/2 port
    outb(0x64, 0xAD);

    wait_input_buffer_clear();
    // disable second PS/2 port
    outb(0x64, 0xA7);


    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    uint8_t config_byte = inb(0x60);
    // clear bits 0 1 and 6
    uint8_t new_config_byte = config_byte & 0b10111100;
    serial_printf("config byte ps/2: %hhx\n", config_byte);
    serial_printf("new config byte ps/2: %hhx\n", new_config_byte);

    wait_input_buffer_clear();
    outb(0x64, 0x60);
    wait_input_buffer_clear();
    outb(0x60, new_config_byte);

    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    config_byte = inb(0x60);
    // clear bits 0 1 and 6
    serial_printf("config byte ps/2: %hhx\n", config_byte);

    wait_input_buffer_clear();
    outb(0x64, 0xAA);
    wait_output_buffer_set();
    int result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0x55)
    {
        serial_printf("passed controller test\n");
    }

    wait_input_buffer_clear();
    outb(0x64, 0xAB);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0x00)
    {
        serial_printf("passed first ps/2 port test\n");
    }
    
    // enable first port
    wait_input_buffer_clear();
    outb(0x64, 0xAE);
    new_config_byte = config_byte | 0b00000001;
    wait_input_buffer_clear();
    outb(0x64, 0x60);
    wait_input_buffer_clear();
    outb(0x60, new_config_byte);

    wait_input_buffer_clear();
    // send command to read byte 0
    outb(0x64, 0x20);

    wait_output_buffer_set();
    config_byte = inb(0x60);
    // clear bits 0 1 and 6
    serial_printf("current config byte ps/2: %hhx\n", config_byte);

    wait_input_buffer_clear();
    outb(0x60, 0xff);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device reset success\n");
    }

    wait_input_buffer_clear();
    outb(0x60, 0xf5);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device ack\n");
    }

    wait_input_buffer_clear();
    outb(0x60, 0xf2);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("result: %hhx\n", result);
    if (result == 0xfa) 
    {
        serial_printf("device ack\n");
    }
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("b1: %hhx\n", result);
    wait_output_buffer_set();
    result = inb(0x60);
    serial_printf("b2: %hhx\n", result);

    irq_clear_mask(0x2);
    asm("sti");
    serial_printf("flags %x\n", read_flags());

    /*
    int status = inb(0x64);
    int read = inb(0x60);
    while (true) {
        int next_status = inb(0x64);
        int next_read = inb(0x60);
        if (next_status != status) {
            status = next_status;
            serial_printf("s%hhx ", status);
        }
        if (next_read != read) {
            read = next_read;
            serial_printf("r%hhx ", read);
        }
    }
    */
}

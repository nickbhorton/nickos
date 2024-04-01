#include <kernel/debug_print.h>

#include <kernel/serial.h>
#include <stdio.h>

void print_idt_entry(idt_entry_t idte) {}
void print_gdt_segment_descriptor(gdt_segment_descriptor_t sd) {}
void print_pde(page_dir_entry_t pde)
{
    char str[SERIAL_MAX_STR_LEN];
    int written = pde_summary_string(pde, str, SERIAL_MAX_STR_LEN);
    written += snprintf(str + written, 2, "\n");
    serial_write(str, written);
}
void print_pte(page_tab_entry_t pte)
{
    char str[SERIAL_MAX_STR_LEN];
    int written = pte_summary_string(pte, str, SERIAL_MAX_STR_LEN);
    written += snprintf(str + written, 2, "\n");
    serial_write(str, written);
}

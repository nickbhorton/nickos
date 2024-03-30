/* nickos/kernel/include/kernel/debug_print.h
 *
 * There are a lot of data structures that are used is intro kernel development
 * and this header will provide ways to print said structures to a specific
 * place. The only output stream that is available as of 3/30/24 is the serial
 * output.
 */

#ifndef _KERNEL_DEBUG_PRINT_H_
#define _KERNEL_DEBUG_PRINT_H_

#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/paging.h>

void print_idt_entry(idt_entry_t idte);
void print_gdt_segment_descriptor(gdt_segment_descriptor_t sd);
void print_pde(page_dir_entry_t pde);
void print_pte(page_tab_entry_t pte);

#endif

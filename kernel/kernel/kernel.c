#include "kernel/gdt.h"
#include "kernel/paging.h"
#include <kernel/idt.h>
#include <kernel/memory.h>
#include <kernel/multiboot.h>
#include <kernel/register.h>
#include <kernel/serial.h>
#include <kernel/tty.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

__attribute__((constructor)) void kernel_init(void)
{
    terminal_initialize();
    initialize_idt();
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

void kernel_main(multiboot_info_t* mbd, unsigned int magic)
{
    char str[SERIAL_MAX_STR_LEN];
    const char* error_str = "not implemented";

    // regular print string
    serial_write_str("hello world!");
    serial_write_str(" -> ");
    int written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, world!");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // n less that string given
    serial_write_str("he");
    serial_write_str(" -> ");
    written = snprintf(str, 2, "hello, world!");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // string length of one
    serial_write_str("h");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "h");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // % of string length one with no specifiers need to put this on a page and
    // seg boundry and make sure it does not page or seg fault.
    serial_write_str("<null>");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "%");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    
    // most basic format spec %%
    serial_write_str("%");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "%%");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string format specifier
    serial_write_str("hello, world!");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, %s!", "world");
    if (written >= 0) {
        serial_write(str, written);
    }
    else {
        serial_write_str(error_str);
    }
    serial_write_str("\n");
}

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
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // n less that string given
    serial_write_str("he");
    serial_write_str(" -> ");
    written = snprintf(str, 2, "hello, world!");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // string length of one
    serial_write_str("h");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "h");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // % of string length one with no specifiers need to put this on a page and
    // seg boundry and make sure it does not page or seg fault.
    serial_write_str("<null>");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "%");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // most basic format spec %%
    serial_write_str("%");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "%%");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string format specifier
    serial_write_str("hello, world!");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, %s!", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string is too long
    serial_write_str("hello, wor");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "hello, %s!", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a precision
    serial_write_str("hello, wor!");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, %.3s!", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a precision that is to large
    serial_write_str("hello, world!");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, %.10s!", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a precision that is to large and max n
    // happens in the middle of %s
    serial_write_str("hello, wor");
    serial_write_str(" -> ");
    written = snprintf(str, 10, "hello, %.100s!", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s with a field width larger than string length
    serial_write_str("hello, |     world|");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "hello, |%10s|", "world");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width and precision
    serial_write_str("| nick|horto|");
    serial_write_str(" -> ");
    written =
        snprintf(str, SERIAL_MAX_STR_LEN, "|%5.5s|%5.5s|", "nick", "horton");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width and precision
    // but pecision is less than field width and string length
    serial_write_str("|      nick|     horto|");
    serial_write_str(" -> ");
    written =
        snprintf(str, SERIAL_MAX_STR_LEN, "|%10.5s|%10.5s|", "nick", "horton");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width and precision
    // but precision is larger than field witdth and string length
    serial_write_str("|horton|");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "|%5.10s|", "horton");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width and justify left
    serial_write_str("|nick    |");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "|%-8s|", "nick");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width and justify left but strlen is larger than
    // field width
    serial_write_str("|nickhorton|");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "|%-8s|", "nickhorton");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %s string with a field width, percision and justify left but strlen is
    // larger than field width and precision
    serial_write_str("|nick    |");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "|%-8.4s|", "nickhorton");
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // basic %d
    serial_write_str("127");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%d", 127);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // max %d
    serial_write_str("2147483647");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%d", 2147483647);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // min %d
    serial_write_str("-2147483648");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%d", -2147483648);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %d 0
    serial_write_str("0");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%d", 0);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %d negitive number
    serial_write_str("-127");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%d", -127);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %d positive number with sign always
    serial_write_str("+127");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "%+d", 127);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %d positive number with sign align
    serial_write_str(" 127");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "% d", 127);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");

    // %d positive number with sign align and always
    serial_write_str("+127");
    serial_write_str(" -> ");
    written = snprintf(str, SERIAL_MAX_STR_LEN, "% +d", 127);
    if (written >= 0)
    {
        serial_write(str, written);
    }
    else
    {
        serial_write_str(error_str);
    }
    serial_write_str("\n");
}

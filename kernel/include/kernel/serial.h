#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stddef.h>
#include <stdint.h>

#define SERIAL_MAX_STR_LEN 2048
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

int serial_initialize(void);

// @return number of bytes written to serial line
// returns after 1000 bytes or when \0 is found.
int serial_write_str(const char* str);

// @return number of bytes written to serial line
// returns after 100,000 bytes regardless of len or when \0 is found.
int serial_write(char* str, size_t len);

void serial_printf(const char* format, ...);
void serial_print_memory(uint8_t* address, uint32_t number_of_bytes,
                         uint32_t bytes_each_line);
void serial_print_bits_on(uint32_t val);

#endif

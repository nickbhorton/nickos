#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stddef.h>
#include <stdint.h>

int serial_initialize(void);

// @return number of bytes written to serial line
// returns after 1000 bytes or when \0 is found.
int serial_write_str(const char *str);

// @return number of bytes written to serial line
// returns after 100,000 bytes regardless of len or when \0 is found.
int serial_write(char *str, size_t len);

void serial_print(const char* format, ...);
void serial_print_page(uint32_t address);
void serial_print_bits_on(uint32_t val);

#endif

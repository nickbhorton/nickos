#ifndef ARCH_I386_SERIAL_H
#define ARCH_I386_SERIAL_H

#include <stddef.h>

int serial_initialize(void);

// @return number of bytes written to serial line
// returns after 1000 bytes or when \0 is found.
int serial_write_str(const char *str);

// @return number of bytes written to serial line
// returns after 100,000 bytes regardless of len or when \0 is found.
int serial_write(char *str, size_t len);

#endif

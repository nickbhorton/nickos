#include <stdint.h>

#include <kernel/serial.h>

#define COM1_SERIAL_PORT 0x3f8

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port
     * numbers that fit in 8b. (N constraint). Wider immediate constants would
     * be truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
     * encoding is the only option for all other cases. %1 expands to %dx
     * because  port  is a uint16_t.  %w1 could be used if we had the port
     * number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

int is_transmit_empty() {
   return inb(COM1_SERIAL_PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(COM1_SERIAL_PORT,a);
}

int serial_initialize()
{
    outb(COM1_SERIAL_PORT + 1, 0x00); // Disable all interrupts
    outb(COM1_SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(COM1_SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_SERIAL_PORT + 1, 0x00); //                  (hi byte)
    outb(COM1_SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1_SERIAL_PORT + 2, 0xC7);
    outb(COM1_SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    // Set in loopback mode, test the serial chip
    outb(COM1_SERIAL_PORT + 4, 0x1E);
    outb(COM1_SERIAL_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and
                                      // check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (inb(COM1_SERIAL_PORT + 0) != 0xAE)
    {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(COM1_SERIAL_PORT + 4, 0x0F);
    write_serial('h');
    write_serial('e');
    write_serial('l');
    write_serial('l');
    write_serial('o');
    write_serial(' ');
    write_serial('w');
    write_serial('o');
    write_serial('r');
    write_serial('l');
    write_serial('d');
    return 0;
}
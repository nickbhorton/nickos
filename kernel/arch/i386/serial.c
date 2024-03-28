#include <stdarg.h>
#include <stdio.h>

#include <kernel/memory.h>
#include <kernel/serial.h>
#include <kernel/register.h>

#define COM1_SERIAL_PORT 0x3f8

static const size_t max_serial_message_size = 1000;

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

int serial_received() { return inb(COM1_SERIAL_PORT + 5) & 0x01; }

char read_serial_char()
{
    while (serial_received() == 0)
        ;

    return inb(COM1_SERIAL_PORT);
}

int is_transmit_empty() { return inb(COM1_SERIAL_PORT + 5) & 0x20; }

void write_serial_char(char a)
{
    while (is_transmit_empty() == 0)
        ;

    outb(COM1_SERIAL_PORT, a);
}

int serial_write_str(const char *str)
{
    size_t i = 0;
    for (i = 0; i < 1000; i++)
    {
        // on null byte break
        if (!str[i])
        {
            break;
        }
        write_serial_char(str[i]);
    }
    return i;
}

int serial_write(char *str, size_t len)
{
    size_t i = 0;
    for (i = 0; i < len && i < 100000; i++)
    {
        // on null byte break
        if (!str[i])
        {
            break;
        }
        write_serial_char(str[i]);
    }
    return i;
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
    return 0;
}

void serial_printf(const char *format, ...)
{
    char msg_buf[max_serial_message_size];

    va_list parameters;
    va_start(parameters, format);
    int written =
        _snprintf(msg_buf, max_serial_message_size, format, parameters);
    va_end(parameters);

    serial_write(msg_buf, written);
}

void serial_print_memory(uint8_t* address, uint32_t number_of_bytes, uint32_t bytes_each_line)
{
    for (size_t i = 0; i < number_of_bytes; i += bytes_each_line)
    {
        char line_buf[max_serial_message_size];
        int line_written = 0;
        line_written += snprintf(line_buf, max_serial_message_size - line_written,
                                 "%X %d: ", address + i, i);
        bool found_non_zero_byte = false;
        for (size_t j = 0; j < bytes_each_line; j++)
        {
            uint8_t cb = peekb(address + (i + j));
            if (cb != 0)
            {
                found_non_zero_byte = true;
            }
            line_written += snprintf(line_buf + line_written,
                                     max_serial_message_size, "%X ", cb);
        }
        line_written +=
            snprintf(line_buf + line_written, max_serial_message_size - line_written, "\n");
        if (found_non_zero_byte)
        {
            serial_write(line_buf, line_written);
        }
    }
}

void serial_print_registers() {
    char msg_buf[max_serial_message_size];
    int written = take_register_image_string(msg_buf, max_serial_message_size);
    serial_write(msg_buf, written);
}

void serial_print_bits_on(uint32_t val)
{
    char msg_buf[max_serial_message_size];
    int bw = 0;
    for (uint32_t i = 0; i < 32; i++)
    {
        if (val & 1 << i)
        {
            bw = snprintf(msg_buf, max_serial_message_size, "%d ", i);
            serial_write(msg_buf, bw);
        }
    }
    bw = snprintf(msg_buf, max_serial_message_size, "\n");
    serial_write(msg_buf, bw);
}

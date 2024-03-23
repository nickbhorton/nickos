#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *) 0xC03FF000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

int terminal_get_index(size_t x, size_t y) { return y * VGA_WIDTH + x; }

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
    terminal_buffer[terminal_get_index(x, y)] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    unsigned char uc = c;
    terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

void terminal_newline(void)
{
    // keep if we are at the last row stay on the last row and scroll
    if (++terminal_row == VGA_HEIGHT)
    {
        // scroll
        for (size_t y = 1; y < VGA_HEIGHT; y++)
        {
            memcpy(&terminal_buffer[(y - 1) * VGA_WIDTH],
                   &terminal_buffer[y * VGA_WIDTH],
                   sizeof(uint16_t) * VGA_WIDTH);
        }
        // terminal_row still at bottom
        memset(&terminal_buffer[(VGA_HEIGHT - 1) * (VGA_WIDTH)], 0, sizeof(uint16_t) * VGA_WIDTH);
        --terminal_row;
    }
    terminal_column = 0;
}

void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (data[i] == '\n')
        {
            terminal_newline();
        }
        else
        {
            terminal_putchar(data[i]);
        }
    }
}

void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}

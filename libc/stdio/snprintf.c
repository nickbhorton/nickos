#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LEFT_JUSTIFIED(x) ((x) & 0x01) // 1 for left, 0 for right
#define SIGN_ALWAYS(x) ((x) & 0x02)    // if set + sign on pos, - sign always
#define SIGN_ALIGN(x)                                                          \
    ((x) & 0x04) // if set and SIGN_ALWAYS not set add space to pos nums
#define ALT_FORM(x) ((x) & 0x08)    // see c docs
#define LEADING_ZEROS ((x) & 0x10)) // if set pad with zeros in certain cases

typedef unsigned char fprintf_flags;

static char int_to_digit_char(int digit);
static char uint_to_digit_char(unsigned int digit);
static char uint_to_lc_hexdigit_char(unsigned int digit);
static char uint_to_uc_hexdigit_char(unsigned int digit);

int _snprintf(char* restrict str, size_t n, const char* restrict format,
              va_list parameters);
int snprintf(char* restrict str, size_t n, const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    int written = _snprintf(str, n, format, parameters);

    va_end(parameters);
    return written;
}

// needs to return however many bytes are written to dest
int snprint(char* restrict dest, size_t start, size_t n, size_t max_n,
            const char* restrict src)
{
    size_t src_index = 0;
    for (size_t dest_index = start;
         dest_index < n + start && dest_index < max_n; dest_index++)
    {
        dest[dest_index] = src[src_index];
        ++src_index;
    }
    return src_index;
}

/*
 * @returns [0,INT_MAX] -> number of char written
 *          -1          -> unimplemented
 */
int _snprintf(char* restrict str, size_t n, const char* restrict format,
              va_list parameters)
{
    size_t written = 0;

    while (*format != '\0' && written < n)
    {
        if (format[0] != '%')
        {
            written += snprint(str, written, 1, n, format);
            format++;
            continue;
        }
        fprintf_flags flags = 0;

        const char* conversion_specification_start = format;
        format++;
        bool more_conversion_spec = true;
        while (*format != '\0' && more_conversion_spec)
        {
            switch (*format)
            {
                case '%':
                {
                    written += snprint(str, written, 1, n, "%");
                    format++;
                    more_conversion_spec = false;
                    break;
                }
                default:
                    return -1;
            }
        }
    }
    return written;
}

static char int_to_digit_char(int digit) { return (char)((digit + 48) % 128); }
static char uint_to_digit_char(unsigned int digit)
{
    return (char)((digit + 48) % 128);
}
static char uint_to_lc_hexdigit_char(unsigned int digit)
{
    switch (digit)
    {
        case 1:
            return '1';
            break;
        case 2:
            return '2';
            break;
        case 3:
            return '3';
            break;
        case 4:
            return '4';
            break;
        case 5:
            return '5';
            break;
        case 6:
            return '6';
            break;
        case 7:
            return '7';
            break;
        case 8:
            return '8';
            break;
        case 9:
            return '9';
            break;
        case 10:
            return 'a';
            break;
        case 11:
            return 'b';
            break;
        case 12:
            return 'c';
            break;
        case 13:
            return 'd';
            break;
        case 14:
            return 'e';
            break;
        case 15:
            return 'f';
            break;
        default:
            return '0';
    }
    return '0';
}

static char uint_to_uc_hexdigit_char(unsigned int digit)
{
    switch (digit)
    {
        case 1:
            return '1';
            break;
        case 2:
            return '2';
            break;
        case 3:
            return '3';
            break;
        case 4:
            return '4';
            break;
        case 5:
            return '5';
            break;
        case 6:
            return '6';
            break;
        case 7:
            return '7';
            break;
        case 8:
            return '8';
            break;
        case 9:
            return '9';
            break;
        case 10:
            return 'A';
            break;
        case 11:
            return 'B';
            break;
        case 12:
            return 'C';
            break;
        case 13:
            return 'D';
            break;
        case 14:
            return 'E';
            break;
        case 15:
            return 'F';
            break;
        default:
            return '0';
    }
    return '0';
}

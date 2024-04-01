#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_INT32_STRING_LEN 2048

#define MAX_INT32_DIGIT_LEN  14 // -2,147,483,648 -> 14

#define LEFT_JUSTIFIED(x)    ((x) & 0x01) // 1 for left, 0 for right
#define SIGN_ALWAYS(x)       ((x) & 0x02) // if set + sign on pos, - sign always
#define SIGN_ALIGN(x)        ((x) & 0x04) // if set and add space to pos nums
#define ALT_FORM(x)          ((x) & 0x08) // see c docs
#define LEADING_ZEROS(x)     ((x) & 0x10) // if set pad with zeros in certain cases

typedef unsigned char fprintf_flags;

static char int_to_digit_char(int digit);

// @assumption assuming that char is a digit char!!!!!
static int digit_char_to_int(char end);

// @returns number of char put into dest
static int int32_to_string(
    int n, char* dest, int precision, int min_width, bool plus_sign,
    bool sign_align
);

static char uint_to_lc_hexdigit_char(unsigned int digit);

static char uint_to_uc_hexdigit_char(unsigned int digit);

static int parse_uint(const char* restrict str, int* digit_size);

int _snprintf(
    char* restrict str, size_t n, const char* restrict format,
    va_list parameters
);

int snprintf(char* restrict str, size_t n, const char* restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);
    int written = _snprintf(str, n, format, parameters);
    va_end(parameters);
    return written;
}

// needs to return however many bytes are written to dest
size_t snprint(
    char* restrict dest, size_t start, size_t n, size_t max_n,
    const char* restrict src
)
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
 *          -2          -> overflow
 */
int _snprintf(
    char* restrict str, size_t n, const char* restrict format,
    va_list parameters
)
{
    // space for the \0 char at the end
    n = n - 1;
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

        const char* format_specification_start = format;
        format++;
        bool more_format_spec = true;
        bool before_period = true;
        // -1 means no field_width has been specified
        int field_width = -1;
        int precision = -1;
        while (*format != '\0' && more_format_spec)
        {
            switch (*format)
            {
                case '%':
                {
                    format++;
                    written += snprint(str, written, 1, n, "%");
                    more_format_spec = false;
                    break;
                }
                case '.':
                {
                    format++;
                    before_period = false;
                    break;
                }

                // flags
                case '-':
                {
                    format++;
                    // left justified
                    flags |= 0x1;
                    break;
                }
                case '+':
                {
                    format++;
                    // sign always
                    flags |= 0x2;
                    break;
                }
                case ' ':
                {
                    format++;
                    // align always
                    flags |= 0x4;
                    break;
                }
                case '#':
                {
                    format++;
                    // alternate form
                    flags |= 0x8;
                    break;
                }
                case '0':
                {
                    format++;
                    // leading zeros
                    flags |= 0x10;
                    break;
                }

                // conversion specifier
                // these signal the end of a format specification
                case 's':
                {
                    format++;
                    more_format_spec = false;
                    const char* va_arg_string = va_arg(parameters, const char*);
                    size_t len = strlen(va_arg_string);
                    if (precision > 0 && precision < len)
                    {
                        len = precision;
                    }
                    if (field_width >= 0)
                    {
                        int num_extra_chars = field_width - len;
                        if (!LEFT_JUSTIFIED(flags))
                        {
                            for (int i = 0; i < num_extra_chars; i++)
                            {
                                written += snprint(str, written, 1, n, " ");
                            }
                        }
                        written += snprint(str, written, len, n, va_arg_string);
                        if (LEFT_JUSTIFIED(flags))
                        {
                            for (int i = 0; i < num_extra_chars; i++)
                            {
                                written += snprint(str, written, 1, n, " ");
                            }
                        }
                    }
                    else
                    {
                        written += snprint(str, written, len, n, va_arg_string);
                    }
                    break;
                }

                case 'd':
                case 'i':
                {
                    format++;
                    more_format_spec = false;
                    int va_arg_int = va_arg(parameters, int);
                    // +1 for the sign
                    static char int_string[MAX_INT32_STRING_LEN + 1];
                    int len = 0;
                    if (precision > 0 && precision < 2048)
                    {
                        len = int32_to_string(
                            va_arg_int, int_string, precision, 0,
                            SIGN_ALWAYS(flags), SIGN_ALIGN(flags)
                        );
                    }
                    else if (precision < 0 && field_width > 0 && field_width < 2048 && LEADING_ZEROS(flags) && !LEFT_JUSTIFIED(flags))
                    {
                        len = int32_to_string(
                            va_arg_int, int_string, 1, field_width,
                            SIGN_ALWAYS(flags), SIGN_ALIGN(flags)
                        );
                    }
                    else if (precision < 0)
                    {
                        len = int32_to_string(
                            va_arg_int, int_string, 1, 0, SIGN_ALWAYS(flags),
                            SIGN_ALIGN(flags)
                        );
                    }
                    else
                    {
                        // overflow
                        return -2;
                    }
                    if (field_width >= 0)
                    {
                        int num_extra_chars = field_width - len;
                        if (!LEFT_JUSTIFIED(flags))
                        {
                            for (int i = 0; i < num_extra_chars; i++)
                            {
                                written += snprint(str, written, 1, n, " ");
                            }
                        }
                        written += snprint(str, written, len, n, int_string);
                        if (LEFT_JUSTIFIED(flags))
                        {
                            for (int i = 0; i < num_extra_chars; i++)
                            {
                                written += snprint(str, written, 1, n, " ");
                            }
                        }
                    }
                    else
                    {
                        written += snprint(str, written, len, n, int_string);
                    }
                    break;
                }
                // This does not conform to standard at the moment
                case 'x':
                case 'X':
                {
                    format++;
                    more_format_spec = false;
                    unsigned int va_arg_uint = va_arg(parameters, unsigned int);
                    unsigned char* ptr = (unsigned char*)&va_arg_uint;
                    // backwards because host byte order
                    for (int i = sizeof(unsigned int) - 1; i > -1; i--)
                    {
                        char a[3];
                        a[0] = uint_to_lc_hexdigit_char(ptr[i] >> 4);
                        a[1] = uint_to_lc_hexdigit_char(ptr[i] & 0x0f);
                        a[2] = '\0';
                        written += snprint(str, written, 2, n, a);
                    }
                    break;
                }

                // field width and precision
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    int digit_size = 0;
                    if (before_period)
                    {
                        field_width = parse_uint(format, &digit_size);
                    }
                    else
                    {
                        precision = parse_uint(format, &digit_size);
                    }
                    format += digit_size;
                    break;
                }

                default:
                    return -1;
            }
        }
    }
    // write a \0 at the end
    snprint(str, written, 1, n + 1, "\0");
    // returns length of written, not including \0
    return written;
}

// assuming that the zeroth index of char* str is the first digit
// if zeroth index of char* str is not a digit returns 0
// str is moved to the next non digit character
static int parse_uint(const char* restrict str, int* digit_size)
{
    const char* restrict begining = str;
    const char* restrict end = str;
    while (isdigit(*end))
    {
        ++end;
        ++(*digit_size);
    }
    if (begining == end)
    {
        return 0;
    }
    // str is now pointing at the next non digit char
    // end ptr is now pointing at the first non ditit char
    --end;
    unsigned int result = 0;
    unsigned int mult = 1;
    for (; end >= begining; --end)
    {
        result += digit_char_to_int(*end) * mult;
        mult *= 10;
    }
    return result;
}

static char int_to_digit_char(int digit) { return (char)((digit + 48) % 128); }
static int digit_char_to_int(char end) { return end - 48; }

static int int32_to_string(
    int n, char* dest, int precision, int min_width, bool plus_sign,
    bool sign_align
)
{
    // cases that are easier to solve with string constants
    if (n == -2147483648)
    {
        const char* min_int32 = "-2147483648";
        int len = strlen(min_int32);
        for (int i = 0; i < len; i++)
        {
            dest[i] = min_int32[i];
        }
        return len;
    }

    bool neg = false;
    if (n < 0)
    {
        neg = true;
        n = -n;
    }

    static char backward[MAX_INT32_STRING_LEN + 1];
    int i = 0;
    while (n > 0)
    {
        backward[i] = int_to_digit_char(n % 10);
        n /= 10;
        i++;
    }

    while (i < precision)
    {
        backward[i] = '0';
        i++;
    }
    if (precision == 1 && min_width > 0)
    {
        if (neg || plus_sign || sign_align)
        {
            // for space with sign or sign aligment
            while (i < min_width - 1)
            {
                backward[i] = '0';
                i++;
            }
        }
        else
        {
            while (i < min_width)
            {
                backward[i] = '0';
                i++;
            }
        }
    }

    if (neg)
    {
        backward[i] = '-';
        i++;
    }
    else if (plus_sign)
    {
        backward[i] = '+';
        i++;
    }
    else if (sign_align)
    {
        backward[i] = ' ';
        i++;
    }

    if (i > 0)
    {
        --i;
        int j = 0;
        for (; i >= 0; i--, j++)
        {
            dest[j] = backward[i];
        }
        return j;
    }
    return 0;
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

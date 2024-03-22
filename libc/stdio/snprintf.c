#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
static bool print(const char *data, size_t length)
{
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < length; i++)
    {
        if (putchar(bytes[i]) == EOF)
        {
            return false;
        }
    }
    return true;
}
*/

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

// needs to return however many bytes are written to dest
int snprint(char *restrict dest, size_t start, size_t n, size_t max_n,
            const char *restrict src)
{
    size_t src_index = 0;
    for (size_t dest_index = start; dest_index < n + start && dest_index < max_n;
         dest_index++)
    {
        dest[dest_index] = src[src_index];
        ++src_index;
    }
    return src_index;
}

/*
 * limitations:
 *      flags not implemented
 *      optional field width not implemented
 *      optional precision not implemented
 *      optional length modifier not implemented
 *      only conversion specifier character implemented
 *          c -> char
 *          d -> int 4 bytes hardcoded
 *          s -> const char *
 */
int snprintf(char *restrict str, size_t n, const char *restrict format, ...)
{
    va_list parameters;
    va_start(parameters, format);

    size_t written = 0;

    while (*format != '\0')
    {
        size_t maxrem = INT_MAX - written;

        // print normal character sequence
        if (format[0] != '%' || format[1] == '%')
        {
            if (format[0] == '%')
            {
                format++;
            }
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
            {
                amount++;
            }
            if (maxrem < amount)
            {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            written += snprint(str, written, amount, n, format);
            format += amount;
            continue;
        }

        const char *format_begun_at = format++;

        switch (*format)
        {
            case 'c':
            {
                format++;
                char c =
                    (char)va_arg(parameters, int /* char promotes to int */);
                if (!maxrem)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                written += snprint(str, written, 1, n, &c);
                break;
            }
            case 's':
            {
                format++;
                const char *va_arg_string = va_arg(parameters, const char *);
                size_t len = strlen(va_arg_string);
                if (maxrem < len)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                written += snprint(str, written, len, n, va_arg_string);
                break;
            }
            case 'i':
            case 'd':
            {
                // 2147483647 is 10
                static const size_t max_digits = 10;
                char digit_string[max_digits];
                memset(digit_string, 0, max_digits);
                // index starting at last digit
                int digit_string_index = max_digits - 1;

                format++;
                int number = va_arg(parameters, int);
                if (!maxrem)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                // lol
                if (number == -2147483648)
                {
                    static const char *digit_string = "-2147483648";
                    written += snprint(str, written, strlen(digit_string), n,
                                       digit_string);
                }
                else if (number == 0)
                {
                    static const char *digit_string = "0";
                    written += snprint(str, written, strlen(digit_string), n,
                                       digit_string);
                }
                else
                {
                    if (number < 0)
                    {
                        // TODO: test this
                        number = -number;
                        static const char *digit_string = "-";
                        written += snprint(str, written, strlen(digit_string),
                                           n, digit_string);
                    }
                    while (number != 0)
                    {
                        digit_string[digit_string_index] =
                            int_to_digit_char(number % 10);
                        digit_string_index--;
                        number /= 10;
                    }
                    const int size_of_digit_string =
                        max_digits - digit_string_index - 1;
                    written += snprint(str, written, size_of_digit_string, n,
                                       digit_string + digit_string_index + 1);
                }
                break;
            }
            case 'u':
            {
                // 2147483648 is 10
                static const size_t max_digits = 10;
                char digit_string[max_digits];
                memset(digit_string, 0, max_digits);
                // index starting at last digit
                int digit_string_index = max_digits - 1;

                format++;
                unsigned int number = va_arg(parameters, unsigned int);
                if (!maxrem)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                if (number == 0)
                {
                    static const char *digit_string = "0";
                    written += snprint(str, written, strlen(digit_string), n,
                                       digit_string);
                }
                else
                {
                    while (number != 0)
                    {
                        digit_string[digit_string_index] =
                            uint_to_digit_char(number % 10);
                        digit_string_index--;
                        number /= 10;
                    }
                    const int size_of_digit_string =
                        max_digits - digit_string_index - 1;
                    written += snprint(str, written, size_of_digit_string, n,
                                       digit_string + digit_string_index + 1);
                }
                break;
            }
            case 'x':
            case 'X':
            {
                // 2147483648 is 80000000 is hex so 8 digits
                static const size_t max_digits = 8;
                char digit_string[max_digits];
                memset(digit_string, 0, max_digits);
                // index starting at last digit
                int digit_string_index = max_digits - 1;

                format++;
                unsigned int number = va_arg(parameters, unsigned int);
                if (!maxrem)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                if (number == 0)
                {
                    static const char *digit_string = "0";
                    written += snprint(str, written, strlen(digit_string), n,
                                       digit_string);
                }
                else
                {
                    while (number != 0)
                    {
                        if (*format == 'x')
                        {
                            digit_string[digit_string_index] =
                                uint_to_lc_hexdigit_char(number % 16);
                        }
                        else
                        {
                            digit_string[digit_string_index] =
                                uint_to_uc_hexdigit_char(number % 16);
                        }
                        digit_string_index--;
                        number /= 16;
                    }
                    const int size_of_digit_string =
                        max_digits - digit_string_index - 1;
                    written += snprint(str, written, size_of_digit_string, n,
                                       digit_string + digit_string_index + 1);
                }
                break;
            }
            default:
            {
                format = format_begun_at;
                size_t len = strlen(format);
                if (maxrem < len)
                {
                    // TODO: Set errno to EOVERFLOW.
                    return -1;
                }
                written += snprint(str, written, len, n, format);
                format += len;
                break;
            }
        }
    }

    va_end(parameters);
    return written;
}

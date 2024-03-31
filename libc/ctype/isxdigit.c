#include <ctype.h>
#include <stdbool.h>

int isxdigit(int c)
{
    if (isdigit(c))
        return true;
    switch (c)
    {
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return true;
        default:
            return false;
    }
}

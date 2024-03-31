#include <ctype.h>

int tolower(int c)
{
    if (islower(c)){
        return c - 32;
    }
    return c;
}

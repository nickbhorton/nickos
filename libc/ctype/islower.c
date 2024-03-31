#include <ctype.h>
#include <stdbool.h>

int islower(int c)
{
    if (c >= 'a' && c <= 'z')
        return true;
    return false;
}

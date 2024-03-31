#include <ctype.h>
#include <stdbool.h>

int isupper(int c)
{
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

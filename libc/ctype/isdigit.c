#include <ctype.h>
#include <stdbool.h>

int isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

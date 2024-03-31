#include <ctype.h>
#include <stdbool.h>

int iscntrl(int c)
{
    if (!isprint(c))
        return true;
    return false;
}


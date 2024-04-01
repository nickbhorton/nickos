#include <ctype.h>
#include <stdbool.h>

int isprint(int c)
{
    if (isgraph(c) || c == ' ')
        return true;
    return false;
}

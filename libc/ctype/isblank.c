#include <ctype.h>
#include <stdbool.h>

int isblank(int c)
{
    switch (c)
    {
        case ' ':
        case '\t':
            return true;
        default:
            return false;
    }
}

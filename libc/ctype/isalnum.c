#include <ctype.h>
#include <stdbool.h>

int isalnum(int c)
{
    if (isalpha(c) || isdigit(c))
        return true;
    return false;
}

#include <ctype.h>
#include <stdbool.h>

int isalpha(int c)
{
    if (isupper(c) || islower(c))
        return true;
    return false;
}

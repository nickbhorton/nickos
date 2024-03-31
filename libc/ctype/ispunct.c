#include <ctype.h>
#include <stdbool.h>

int ispunct(int c){
    if (isprint(c) && !isspace(c) && !isalnum(c))
        return true;
    return false;
}

#include <ctype.h>
#include <stdbool.h>

int isspace(int c){
    switch(c)
    {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        return true;
        default:
        return false;
    }
}

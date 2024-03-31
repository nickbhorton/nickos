#include <ctype.h>
#include <stdbool.h>

int isgraph(int c){
    if (isalnum(c))
        return true;
    switch(c)
    {
        case '!':
        case '"':
        case '#':
        case '%':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case '-':
        case '.':
        case '/':
        case ':':
        case ';':
        case '<':
        case '=':
        case '>':
        case '?':
        case '[':
        case '\\':
        case ']':
        case '^':
        case '_':
        case '{':
        case '|':
        case '}':
        case '~':
        return true;
        default:
        return false;
    }
}

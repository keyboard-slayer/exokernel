#include <ctype.h>

int isspace(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

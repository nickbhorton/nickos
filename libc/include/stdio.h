#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include <stddef.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int snprintf(char *restrict, size_t, const char *restrict, ...);
int putchar(int);

#ifdef __cplusplus
}
#endif

#endif

#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

#include <stdint.h>

static inline uint32_t peekl(uint32_t address)
{
    uint32_t ret;
    asm volatile("movl  %1, %%eax\n\t"
                 "movl  (%%eax), %0\n\t"
                 : "=r"(ret)
                 : "g"(address)
                 : "eax");
    return ret;
}

#endif

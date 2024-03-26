#ifndef _KERNEL_REGISTER_H
#define _KERNEL_REGISTER_H

#include <stdint.h>

static inline uint32_t read_cr0(void)
{
    unsigned long val;
    asm volatile("mov %%cr0, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_cs(void)
{
    unsigned long val;
    asm volatile("mov %%cs, %0" : "=r"(val));
    return val;
}
static inline uint32_t read_ds(void)
{
    unsigned long val;
    asm volatile("mov %%ds, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_es(void)
{
    unsigned long val;
    asm volatile("mov %%es, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_fs(void)
{
    unsigned long val;
    asm volatile("mov %%fs, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_gs(void)
{
    unsigned long val;
    asm volatile("mov %%gs, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_ss(void)
{
    unsigned long val;
    asm volatile("mov %%ss, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_flags(void)
{
    unsigned long flags;
    asm volatile("pushf\n\t"
                 "pop %0"
                 : "=g"(flags));
    return flags;
}

#endif

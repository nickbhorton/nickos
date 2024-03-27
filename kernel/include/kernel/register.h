#ifndef _KERNEL_REGISTER_H
#define _KERNEL_REGISTER_H

#include <stddef.h>
#include <stdint.h>

typedef struct register_image {
    uint32_t cr0;
    uint32_t cr2;
    uint32_t cr3;
    uint32_t cr4;

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    uint16_t cs;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
    uint16_t es;

    uint32_t flags;

    uint32_t esi;
    uint32_t edi;

    uint32_t ebp;
    uint32_t esp;
} register_image_t;

/* reads all registers into ri. Useful for exceptions. returns zero on success
 */
int take_register_image(register_image_t* ri);

int register_image_string(const register_image_t* ri, char* restrict str,
                          size_t max_str_len);
int take_register_image_string(char* restrict str, size_t max_str_len);

// control registers
uint32_t read_cr0(void);
uint32_t read_cr2(void);
uint32_t read_cr3(void);
uint32_t read_cr4(void);

// segment registers
uint16_t read_cs(void);
uint16_t read_ds(void);
uint16_t read_fs(void);
uint16_t read_gs(void);
uint16_t read_ss(void);
uint16_t read_es(void);

// general registers
uint32_t read_eax(void);
uint32_t read_ebx(void);
uint32_t read_ecx(void);
uint32_t read_edx(void);

// indicator registers
uint32_t read_flags(void);

// index registers
uint32_t read_esi(void);
uint32_t read_edi(void);

// pointer registers
uint32_t read_ebp(void);
uint32_t read_esp(void);

#endif

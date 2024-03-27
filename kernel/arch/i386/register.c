#include <kernel/register.h>
#include <stdio.h>

int take_register_image(register_image_t* ri)
{
    // eax has to go first as funtions return using eax
    ri->eax = read_eax();

    ri->cr0 = read_cr0();
    ri->cr2 = read_cr2();
    ri->cr3 = read_cr3();
    ri->cr4 = read_cr4();

    ri->ebx = read_ebx();
    ri->ecx = read_ecx();
    ri->edx = read_edx();

    ri->cs = read_cs();
    ri->ds = read_ds();
    ri->fs = read_fs();
    ri->gs = read_gs();
    ri->ss = read_ss();
    ri->es = read_es();

    ri->flags = read_flags();

    ri->esi = read_esi();
    ri->edi = read_edi();

    ri->ebp = read_ebp();
    ri->esp = read_esp();
    return 0;
}

int register_image_string(const register_image_t* ri, char* restrict str,
                          size_t max_str_len)
{
    int written = 0;
    written +=
        snprintf(str + written, max_str_len - written, "registers:\n");
    written +=
        snprintf(str + written, max_str_len - written, "\tcr0: 0x%X\n", ri->cr0);
    written +=
        snprintf(str + written, max_str_len - written, "\tcr2: 0x%X\n", ri->cr2);
    written +=
        snprintf(str + written, max_str_len - written, "\tcr3: 0x%X\n", ri->cr3);
    written +=
        snprintf(str + written, max_str_len - written, "\tcr4: 0x%X\n", ri->cr4);

    written +=
        snprintf(str + written, max_str_len - written, "\teax: 0x%X\n", ri->eax);
    written +=
        snprintf(str + written, max_str_len - written, "\tebx: 0x%X\n", ri->ebx);
    written +=
        snprintf(str + written, max_str_len - written, "\tecx: 0x%X\n", ri->ecx);
    written +=
        snprintf(str + written, max_str_len - written, "\tedx: 0x%X\n", ri->edx);

    written +=
        snprintf(str + written, max_str_len - written, "\tcs: 0x%X\n", ri->cs);
    written +=
        snprintf(str + written, max_str_len - written, "\tds: 0x%X\n", ri->ds);
    written +=
        snprintf(str + written, max_str_len - written, "\tfs: 0x%X\n", ri->fs);
    written +=
        snprintf(str + written, max_str_len - written, "\tgs: 0x%X\n", ri->gs);
    written +=
        snprintf(str + written, max_str_len - written, "\tss: 0x%X\n", ri->ss);
    written +=
        snprintf(str + written, max_str_len - written, "\tes: 0x%X\n", ri->es);

    written += snprintf(str + written, max_str_len - written, "\tflags: 0x%X\n",
                        ri->flags);

    written +=
        snprintf(str + written, max_str_len - written, "\tesi: 0x%X\n", ri->esi);
    written +=
        snprintf(str + written, max_str_len - written, "\tedi: 0x%X\n", ri->edi);

    written +=
        snprintf(str + written, max_str_len - written, "\tebp: 0x%X\n", ri->ebp);
    written +=
        snprintf(str + written, max_str_len - written, "\tesp: 0x%X\n", ri->esp);
    return written;
}

int take_register_image_string(char * restrict str, size_t max_str_len) {
    register_image_t ri;
    take_register_image(&ri);
    return register_image_string(&ri, str, max_str_len);
}

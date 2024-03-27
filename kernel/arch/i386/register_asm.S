.section .text

# contol registers
.global read_cr0
read_cr0:
    movl %cr0, %eax
    ret
.global read_cr2
read_cr2:
    movl %cr2, %eax
    ret
.global read_cr3
read_cr3:
    movl %cr3, %eax
    ret
.global read_cr4
read_cr4:
    movl %cr4, %eax
    ret

# segment registers
.global read_ss
read_ss:
    movw %ss, %ax
    ret
.global read_ds
read_ds:
    movw %ds, %ax
    ret
.global read_fs
read_fs:
    movw %fs, %ax
    ret
.global read_es
read_es:
    movw %es, %ax
    ret
.global read_cs
read_cs:
    movw %cs, %ax
    ret
.global read_gs
read_gs:
    movw %gs, %ax
    ret

# general reigsters
.global read_eax
read_eax:
    ret
.global read_ebx
read_ebx:
    movl %ebx, %eax
    ret
.global read_ecx
read_ecx:
    movl %ecx, %eax
    ret
.global read_edx
read_edx:
    movl %edx, %eax
    ret

# indicator registers
.global read_flags
read_flags:
    pushf
    pop %eax
    ret

# index registers
.global read_esi
read_esi:
    movl %esi, %eax
    ret
.global read_edi
read_edi:
    movl %edi, %eax
    ret

# pointer registers
.global read_esp
read_esp:
    movl %esp, %eax
    ret
.global read_ebp
read_ebp:
    movl %ebp, %eax
    ret



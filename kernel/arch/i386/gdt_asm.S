.section .text
.global read_gdtr
read_gdtr:
    # move gdtr_t* from args into eax
    movl 4(%esp), %eax
    sgdt (%eax);
    movl $0, %eax
    ret
        

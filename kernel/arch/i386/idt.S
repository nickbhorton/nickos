.text

.global page_fault_handle
.type page_fault_handle, @function
page_fault_handle:
    # PF adds error code, this removes it
    cli
    hlt
    addl $4, %esp
    push $message_pf
    call serial_print
    addl $4, %esp
    iret 

exception_handler:
    cli
    hlt

.macro isr_err_stub num
isr_stub_\num:
    call exception_handler
    iret
.endm

.macro isr_no_err_stub num
isr_stub_\num:
    call exception_handler
    iret
.endm

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

.global isr_stub_table
isr_stub_table:
.macro generate_isr_stub_table from=0, to=31
    .long   \from
        .if     \to-\from
        generate_isr_stub_table "(\from+1)",\to
        .endif
.endm
generate_isr_stub_table


# i want one argument that is a ptr to idt
.global fill_idt
.type fill_idt, @function
fill_idt:
    movl 4(%esp), %eax
    movl 8(%esp), %ecx
    addl %ecx, %eax

    movl $page_fault_handle, %ecx
    movb %cl, 0(%eax)
    movb %ch, 1(%eax)
    shrl $0x10, %ecx
    movb %cl, 6(%eax)
    movb %ch, 7(%eax)
    
    movb $0x08, 2(%eax)
    movb $0x0,  3(%eax)
    # present gate
    movb $0b10000000, 5(%eax)
    movl 12(%esp), %ecx
    orb %cl, 5(%eax)
    sti
    ret


message_pf:
    .ascii "#PF\n\0"
    

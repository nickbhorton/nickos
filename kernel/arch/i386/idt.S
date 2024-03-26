.text

/*
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
*/

.global exception_handler
exception_handler:
    pushl $message1
    call serial_print
    addl $4, %esp
    cli
    hlt

message1:
    .ascii "exception\n\0"

.macro isr_err_stub num
.global isr_stub_\num
isr_stub_\num:
    call exception_handler
    iret
.endm

.macro isr_no_err_stub num
.global isr_stub_\num
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
    .long   isr_stub_0
    .long   isr_stub_1
    .long   isr_stub_2
    .long   isr_stub_3
    .long   isr_stub_4
    .long   isr_stub_5
    .long   isr_stub_6
    .long   isr_stub_7
    .long   isr_stub_8
    .long   isr_stub_9
    .long   isr_stub_10
    .long   isr_stub_11
    .long   isr_stub_12
    .long   isr_stub_13
    .long   isr_stub_14
    .long   isr_stub_15
    .long   isr_stub_16
    .long   isr_stub_17
    .long   isr_stub_18
    .long   isr_stub_19
    .long   isr_stub_20
    .long   isr_stub_21
    .long   isr_stub_22
    .long   isr_stub_23
    .long   isr_stub_24
    .long   isr_stub_25
    .long   isr_stub_26
    .long   isr_stub_27
    .long   isr_stub_28
    .long   isr_stub_29
    .long   isr_stub_30
    .long   isr_stub_31


/*
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
*/

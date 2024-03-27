.global pokel 
.type pokel, @function
pokel:
    # first arg: address
    movl 4(%esp), %eax
    # second arg: val
    movl 8(%esp), %ecx
    movl %ecx, (%eax)
    ret

.global pokew 
.type pokew, @function
pokew:
    # first arg: address
    movl 4(%esp), %eax
    # second arg: val
    movl 8(%esp), %ecx
    movw %cx, (%eax)
    ret

.global pokeb 
.type pokeb, @function
pokeb:
    # first arg: address
    movl 4(%esp), %eax
    # second arg: val
    movl 8(%esp), %ecx
    movb %cl, (%eax)
    ret
    
.global peekb
.type peekb, @function
peekb:
    movl 4(%esp), %eax
    movb (%eax), %al
    ret

.global peekw
.type peekw, @function
peekw:
    movl 4(%esp), %eax
    movw (%eax), %ax
    ret

.global peekl
.type peekl, @function
peekl:
    movl 4(%esp), %eax
    movl (%eax), %eax
    ret

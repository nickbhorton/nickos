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
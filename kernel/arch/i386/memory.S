.global farpeekl 
.type farpeekl, @function
farpeekl:
    # first arg
    movl 4(%esp), %eax
    # second arg
    movl 8(%esp), %ecx
    ret

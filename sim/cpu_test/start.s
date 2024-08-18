.section entry, "ax"
.globl _start,halt
.type _start, @function
.type halt, @function

_start:
    la      sp,     _stack_pointer
    jal     main
halt:
    j       halt

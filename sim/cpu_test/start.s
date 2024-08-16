.section entry, "ax"
.global _start,halt
.type _start, @function

_start:
    la      sp,     _stack_pointer
    jal     main
halt:
    j       halt

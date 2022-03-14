.section .text.init
.globl _start
_start:
    addi sp,sp,0x100
    jal ra,main

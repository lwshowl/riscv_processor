.section .text.init
.globl _start
_start:
    addi sp,sp,0x500
    jal ra,main

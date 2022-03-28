.section .text.init
.globl _start
.globl _exit_exceution
_start:
    li sp,0x2000
    call main

_hlt:
    j _hlt

_exit_exceution:
    ecall
   
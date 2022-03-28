.section .text
.equ FREQ , 50000000
.globl delay_1s

delay_1s:
    li t0,(FREQ/32)/2
    li t1,0
    addi t1,t1,1
    bltu t1,t0,delay_1s+0xc
    ret
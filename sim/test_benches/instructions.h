#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//instructions
#define   i_bubble  0
#define   i_lui     1
#define   i_auipc   2
#define   i_jal     3
#define   i_jalr    4
#define   i_beq     5
#define   i_bne     6
#define   i_blt     7
#define   i_bge     8
#define   i_bltu    9
#define   i_bgeu    10
#define   i_lb      11
#define   i_lh      12
#define   i_lw      13
#define   i_lbu     14
#define   i_lhu     15
#define   i_sb      16
#define   i_sh      17
#define   i_sw      18
#define   i_addi    19
#define   i_slti    20
#define   i_sltiu   21
#define   i_xori    22
#define   i_ori     23
#define   i_andi    24
#define   i_slli    25
#define   i_srli    26
#define   i_srai    27
#define   i_add     28
#define   i_sub     29
#define   i_sll     30
#define   i_slt     31
#define   i_sltu    32
#define   i_xor     33
#define   i_srl     34
#define   i_sra     35
#define   i_or      36
#define   i_and     37
#define   i_ecall   38
#define   i_ebreak  39
#define   i_csrrw   40
#define   i_csrrs   41
#define   i_csrrc   42
#define   i_csrrwi  43
#define   i_csrrsi  44
#define   i_csrrci  45

// RV64I instructions
#define i_sd 46
#define i_ld 47
#define i_addw 48
#define i_subw 49
#define i_srlw 50
#define i_sraw 51
#define i_slliw 52
#define i_sllw 53
#define i_addiw 54
#define i_sraiw 55
#define i_srliw 56

// RV32M instructions
#define i_mul 57
#define i_mulh 58
#define i_mulhsu 59
#define i_mulhu 60
#define i_div 61
#define i_divu 62
#define i_rem 63
#define i_remu 64

// RV64M instrctions
#define i_mulw 65
#define i_divw 66
#define i_divuw 67
#define i_remw   68
#define i_remuw 69
#define i_lwu 70
#define i_mret 71


#define   i_invalid 127
#endif

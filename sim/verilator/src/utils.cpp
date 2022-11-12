#include "include/utils.h"
#include "instructions.h"
#include <iostream>
std::string enum_to_ins(int id)
{
    std::string instr;
    switch (id)
    {
    case i_lui:
        instr = "i_lui";
        break;
    case i_auipc:
        instr = "i_auipc";
        break;
    case i_jal:
        instr = "i_jal";
        break;
    case i_jalr:
        instr = "i_jalr";
        break;
    case i_beq:
        instr = "i_beq";
        break;
    case i_bne:
        instr = "i_bne";
        break;
    case i_blt:
        instr = "i_blt";
        break;
    case i_bge:
        instr = "i_bge";
        break;
    case i_bltu:
        instr = "i_bltu";
        break;
    case i_bgeu:
        instr = "i_bgeu";
        break;
    case i_lb:
        instr = "i_lb";
        break;
    case i_lh:
        instr = "i_lh";
        break;
    case i_lw:
        instr = "i_lw";
        break;
    case i_lbu:
        instr = "i_lbu";
        break;
    case i_lhu:
        instr = "i_lhu";
        break;
    case i_sb:
        instr = "i_sb";
        break;
    case i_sh:
        instr = "i_sh";
        break;
    case i_sw:
        instr = "i_sw";
        break;
    case i_addi:
        instr = "i_addi";
        break;
    case i_slti:
        instr = "i_slti";
        break;
    case i_sltiu:
        instr = "i_sltiu";
        break;
    case i_xori:
        instr = "i_xori";
        break;
    case i_ori:
        instr = "i_ori";
        break;
    case i_andi:
        instr = "i_andi";
        break;
    case i_slli:
        instr = "i_slli";
        break;
    case i_srli:
        instr = "i_srli";
        break;
    case i_srai:
        instr = "i_srai";
        break;
    case i_add:
        instr = "i_add";
        break;
    case i_sub:
        instr = "i_sub";
        break;
    case i_sll:
        instr = "i_sll";
        break;
    case i_slt:
        instr = "i_slt";
        break;
    case i_sltu:
        instr = "i_sltu";
        break;
    case i_xor:
        instr = "i_xor";
        break;
    case i_srl:
        instr = "i_srl";
        break;
    case i_sra:
        instr = "i_sra";
        break;
    case i_or:
        instr = "i_or";
        break;
    case i_and:
        instr = "i_and";
        break;
    case i_ecall:
        instr = "i_ecall";
        break;
    case i_ebreak:
        instr = "i_ebreak";
        break;
    case i_csrrw:
        instr = "i_csrrw";
        break;
    case i_csrrs:
        instr = "i_csrrs";
        break;
    case i_csrrc:
        instr = "i_csrrc";
        break;
    case i_csrrwi:
        instr = "i_csrrwi";
        break;
    case i_csrrsi:
        instr = "i_csrrsi";
        break;
    case i_csrrci:
        instr = "i_csrrci";
        break;
    case i_invalid:
        instr = "i_invalid";
        break;
    }
    return instr;
}

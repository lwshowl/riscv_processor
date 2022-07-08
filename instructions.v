`ifndef INSTRUCTIONS_V
`define INSTRUCTIONS_V

//instructions
`define   i_bubble  7'd0
`define   i_lui     7'd1
`define   i_auipc   7'd2
`define   i_jal     7'd3
`define   i_jalr    7'd4
`define   i_beq     7'd5
`define   i_bne     7'd6
`define   i_blt     7'd7
`define   i_bge     7'd8
`define   i_bltu    7'd9
`define   i_bgeu    7'd10
`define   i_lb      7'd11
`define   i_lh      7'd12
`define   i_lw      7'd13
`define   i_lbu     7'd14
`define   i_lhu     7'd15
`define   i_sb      7'd16
`define   i_sh      7'd17
`define   i_sw      7'd18
`define   i_addi    7'd19
`define   i_slti    7'd20
`define   i_sltiu   7'd21
`define   i_xori    7'd22
`define   i_ori     7'd23
`define   i_andi    7'd24
`define   i_slli    7'd25
`define   i_srli    7'd26
`define   i_srai    7'd27
`define   i_add     7'd28
`define   i_sub     7'd29
`define   i_sll     7'd30
`define   i_slt     7'd31
`define   i_sltu    7'd32
`define   i_xor     7'd33
`define   i_srl     7'd34
`define   i_sra     7'd35
`define   i_or      7'd36
`define   i_and     7'd37
`define   i_ecall   7'd38
`define   i_ebreak  7'd39
`define   i_csrrw   7'd40
`define   i_csrrs   7'd41
`define   i_csrrc   7'd42
`define   i_csrrwi  7'd43
`define   i_csrrsi  7'd44
`define   i_csrrci  7'd45

// RV64I instructions
`define i_sd 7'd46
`define i_ld 7'd47
`define i_addw 7'd48
`define i_subw 7'd49
`define i_srlw 7'd50
`define i_sraw 7'd51
`define i_slliw 7'd52
`define i_sllw 7'd53
`define i_addiw 7'd54
`define i_sraiw 7'd55
`define i_srliw 7'd56

// RV32M instructions
`define i_mul 7'd57
`define i_mulh 7'd58
`define i_mulhsu 7'd59
`define i_mulhu 7'd60
`define i_div 7'd61
`define i_divu 7'd62
`define i_rem 7'd63
`define i_remu 7'd64

// RV64M instrctions
`define i_mulw 7'd65
`define i_divw 7'd66
`define i_divuw 7'd67
`define i_remw   7'd68
`define i_remuw 7'd69
`define i_lwu 7'd70
`define i_mret 7'd71


`define   i_invalid 7'd127
`endif

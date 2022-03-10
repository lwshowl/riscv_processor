`ifndef INSTRUCTIONS_V
`define INSTRUCTIONS_V

//instructions
parameter   i_lui    = 0;
parameter   i_auipc  = 1;
parameter   i_jal    = 2;
parameter   i_jalr   = 3;
parameter   i_beq    = 4;
parameter   i_bne    = 5;
parameter   i_blt    = 6;
parameter   i_bge    = 7;
parameter   i_bltu   = 8;
parameter   i_bgeu   = 9;
parameter   i_lb     = 10;
parameter   i_lh     = 11;
parameter   i_lw     = 12;
parameter   i_lbu    = 13;
parameter   i_lhu    = 14;
parameter   i_sb     = 15;
parameter   i_sh     = 16;
parameter   i_sw     = 17;
parameter   i_addi   = 18;
parameter   i_slti   = 19;
parameter   i_sltiu  = 20;
parameter   i_xori   = 21;
parameter   i_ori    = 22;
parameter   i_andi   = 23;
parameter   i_slli   = 24;
parameter   i_srli   = 25;
parameter   i_srai   = 26;
parameter   i_add    = 27;
parameter   i_sub    = 28;
parameter   i_sll    = 29;
parameter   i_slt    = 30;
parameter   i_sltu   = 31;
parameter   i_xor    = 32;
parameter   i_srl    = 33;
parameter   i_sra    = 34;
parameter   i_or     = 35;
parameter   i_and    = 36;
parameter   i_ecall  = 37;
parameter   i_ebreak = 38;
parameter   i_csrrw  = 39;
parameter   i_csrrs  = 40;
parameter   i_csrrc  = 41;
parameter   i_csrrwi = 42;
parameter   i_csrrsi = 43;
parameter   i_csrrci = 44;

`endif

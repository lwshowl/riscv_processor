module reg_dec_exe (
    input clk,
    input reset,
    input enable,
    input [4:0] rs2,
    input [4:0] rd,    
    input [63:0] rs1val,
    input [63:0] rs2val,
    input [63:0] pc_from_decode,
    input [63:0] imm64_from_decode,
    input [5:0] shamt_from_decode,
    input [6:0] opcode_from_decode,
    input [6:0] instrId_from_decode,
    input branch_from_decode,
    input regw_from_decode,
    input memr_from_decode,
    input memw_from_decode,
    output [4:0] alu_zimm,
    output [4:0] alu_rd,
    output [63:0] alu_rs1val,
    output [63:0] alu_rs2val,
    output [63:0] alu_pc,
    output [63:0] alu_imm64,
    output [5:0] alu_shamt,
    output [6:0] alu_opcode,
    output [6:0] alu_instrId,
    output alu_branch,
    output alu_regw,
    output alu_memr,
    output alu_memw
);

Reg #(5,0)  reg_alu_zimm (clk, reset, rs2, alu_zimm, enable);
Reg #(64,0) reg_alu_rs1val (clk, reset, rs1val, alu_rs1val, enable);
Reg #(64,0) reg_alu_rs2val (clk, reset, rs2val, alu_rs2val, enable);
Reg #(5,0) reg_alu_rd      (clk, reset, rd, alu_rd, enable);
Reg #(64,0) reg_alu_pc      (clk, reset, pc_from_decode, alu_pc, enable);
Reg #(64,0) reg_alu_imm64 (clk, reset, imm64_from_decode, alu_imm64, enable);
Reg #(6,0) reg_alu_shamt (clk, reset, shamt_from_decode, alu_shamt, enable);
Reg #(7,0) reg_alu_opcode (clk, reset, opcode_from_decode, alu_opcode, enable);
Reg #(7,0) reg_alu_instrId (clk, reset, instrId_from_decode, alu_instrId, enable);
Reg #(1,0) reg_alu_branch (clk, reset, branch_from_decode, alu_branch, enable);
Reg #(1,0) reg_alu_regw (clk, reset, regw_from_decode, alu_regw, enable);
Reg #(1,0) reg_alu_memr (clk, reset, memr_from_decode, alu_memr, enable);
Reg #(1,0) reg_alu_memw (clk, reset, memw_from_decode, alu_memw, enable);


endmodule
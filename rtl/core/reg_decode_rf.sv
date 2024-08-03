module reg_decode_rf(
    input clk,
    input enable,
    input rst,
    input [63:0] rs1_from_decode,
    input [63:0] rs2_from_decode,
    input [63:0] rd_from_decode,
    input [63:0] pc_from_decode,
    input [31:0] imm32_from_decode,
    input [5:0] shamt_from_decode,
    input [6:0] opcode_from_decode,
    input [7:0] instrId_from_decode,
    input branch_from_decode,
    input regw_from_decode,
    input memr_from_decode,
    input memw_from_decode,
    output [63:0] rs1_to_rf,
    output [63:0] rs2_to_rf,
    output [63:0] rd_to_rf,
    output [63:0] pc_to_rf,
    output [31:0] imm_to_rf,
    output [5:0] shamt_to_rf,
    output [6:0] opcode_to_rf,
    output [7:0] instrId_to_rf,
    output branch_to_rf,
    output regw_to_rf,
    output memr_to_rf,
    output memw_to_rf
);
    Reg #(5,0) regfile_rs1 (clk, rst, rs1_from_decode, rs1_to_rf, enable);
    Reg #(5,0) regfile_rs2 (clk, rst, rs2_from_decode, rs2_to_rf, enable);
    Reg #(5,0) regfile_rd (clk, rst,rd, rd_from_decode, rd_to_rf, enable);
    Reg #(64,0) regfile_pc (clk, rst, pc_from_decode, pc_to_rf, enable);
    Reg #(64,0) regfile_imm64 (clk, rst, {{32{imm32_from_decode[31]}}, imm32_from_decode}, imm_to_rf, enable);
    Reg #(6,0) regfile_shamt (clk, rst, shamt_from_decode, shamt_to_rf, enable);
    Reg #(7,0) regfile_opcode (clk, rst, opcode_from_decode, opcode_to_rf, enable);
    Reg #(7,0) regfile_instrId (clk, rst, instrId_from_decode, instrId_to_rf, enable);
    Reg #(1,0) regfile_branch (clk, rst, branch_from_decode, branch_to_rf, enable);
    Reg #(1,0) regfile_regw (clk, rst, regw_from_decode, regw_to_rf, enable);
    Reg #(1,0) regfile_memr (clk, rst, memr_from_decode, memr_to_rf, enable);
    Reg #(1,0) regfile_memw (clk, rst, memw_from_decode, memw_to_rf, enable);
    
endmodule
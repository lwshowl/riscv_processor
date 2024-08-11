module reg_alu_dmem(
    input clk,
    input rst,
    input enable,
    input [4:0] zimm_from_alu,
    input [4:0] rd_from_alu,
    input [63:0] pc_from_alu,
    input [63:0] rs1val_from_alu,
    input [63:0] rs2val_from_alu,
    input [63:0] imm_from_alu,
    input [6:0]  instrId_from_alu,
    input [6:0]  opcode_from_alu,
    input [63:0] alu_result,
    input regw_from_alu,
    input memw_from_alu,
    input memr_from_alu,
    input branch_from_alu,
    output [4:0] zimm_to_dmem, 
    output [4:0] rd_to_dmem,
    output [63:0] pc_to_dmem,
    output [63:0] rs1val_to_dmem,
    output [63:0] rs2val_to_dmem,
    output [63:0] imm_to_dmem,
    output [63:0] alu_result_to_dmem,
    output [6:0] instrId_to_dmem,
    output [6:0] opcode_to_dmem,
    output regw_to_dmem,
    output memw_to_dmem,
    output memr_to_dmem,
    output branch_to_dmem
);

Reg #(5,0) reg_dmem_zimm(clk, rst, zimm_from_alu, zimm_to_dmem, enable);
Reg #(5,0) reg_dmem_rd (clk,  rst, rd_from_alu, rd_to_dmem, enable);
Reg #(64,0) reg_dmem_pc (clk, rst, pc_from_alu, pc_to_dmem, enable);
Reg #(64,0) reg_dmem_result (clk, rst, alu_result, alu_result_to_dmem, enable);
Reg #(64,0) reg_dmem_rs1val (clk, rst, rs1val_from_alu, rs1val_to_dmem, enable);
Reg #(64,0) reg_dmem_rs2val (clk, rst, rs2val_from_alu, rs2val_to_dmem, enable);
Reg #(64,0) reg_dmem_imm64 (clk,  rst, imm_from_alu,  imm_to_dmem, enable);
Reg #(7,0) reg_dmem_instrId (clk, rst, instrId_from_alu,  instrId_to_dmem, enable);
Reg #(7,0) reg_dmem_opcode (clk,  rst, opcode_from_alu, opcode_to_dmem, enable);
Reg #(1,0) reg_dmem_regw (clk,  rst, regw_from_alu, regw_to_dmem, enable);
Reg #(1,0) reg_dmem_memw (clk,  rst, memw_from_alu, memw_to_dmem, enable);
Reg #(1,0) reg_dmem_memr (clk,  rst, memr_from_alu, memr_to_dmem, enable);
Reg #(1,0) reg_dmem_branch (clk,  rst, branch_from_alu, branch_to_dmem, enable);

endmodule
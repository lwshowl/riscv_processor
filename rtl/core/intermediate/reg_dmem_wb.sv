module reg_dmem_wb(
    input clk,
    input rst,
    input enable,
    input [63:0] csrval,
    input [4:0] rd_from_dmem,
    input [63:0] pc_from_dmem,
    input [63:0] result_from_dmem,
    input [6:0] instrId_from_dmem,
    input [63:0] dmem_data,
    input [63:0] rs1val_from_dmem,
    input [63:0] rs2val_from_dmem,
    input regw_from_dmem,
    input memw_from_dmem,
    input memr_from_dmem,
    input branch_from_dmem,
    input [6:0] opcode_from_dmem,
    output [63:0] csrval_to_wb,
    output [4:0] rd_to_wb,
    output [63:0] pc_to_wb,
    output [63:0] result_to_wb,
    output [6:0] instrId_to_wb,
    output [63:0] dmem_data_to_wb,
    output [63:0] rs1val_to_wb,
    output [63:0] rs2val_to_wb,
    output regw_to_wb,
    output memw_to_wb,
    output memr_to_wb,
    output branch_to_wb,
    output [6:0] opcode_to_wb
);

Reg #(64,0) wb_csrval (clk, rst, csrval, csrval_to_wb, enable);
Reg #(5,0) wb_rd (clk, rst, rd_from_dmem, rd_to_wb, enable);
Reg #(64,0) wb_pc (clk, rst, pc_from_dmem, pc_to_wb, enable);
Reg #(64,0) wb_result (clk, rst, result_from_dmem, result_to_wb, enable);
Reg #(7,0) wb_instrId (clk, rst, instrId_from_dmem, instrId_to_wb, enable);
Reg #(64,0) wb_memdata (clk, rst, dmem_data, dmem_data_to_wb, enable);
Reg #(64,0) wb_rs1val (clk, rst, rs1val_from_dmem, rs1val_to_wb, enable);
Reg #(64,0) wb_rs2val (clk, rst, rs2val_from_dmem, rs2val_to_wb, enable);
Reg #(1,0) wb_regw (clk, rst, regw_from_dmem, regw_to_wb, enable);
Reg #(1,0) wb_memw (clk, rst, memw_from_dmem, memw_to_wb, enable);
Reg #(1,0) wb_memr (clk, rst, memr_from_dmem, memr_to_wb, enable);
Reg #(1,0) wb_branch (clk, rst, branch_from_dmem, branch_to_wb, enable);
Reg #(7,0) wb_opcode (clk, rst, opcode_from_dmem, opcode_to_wb, enable);

endmodule
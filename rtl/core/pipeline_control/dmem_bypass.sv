module dmem_bypass(
    input [4:0] alu_rs1,
    input [4:0] alu_rs2,
    input [4:0] wb_rd,
    input wb_regw,
    output rs1_dmem_bypass,
    output rs2_dmem_bypass
);

assign rs1_dmem_bypass = (alu_rs1 == wb_rd && alu_rs1 != 0 && wb_regw) ? 1:0;
assign rs2_dmem_bypass = (alu_rs2 == wb_rd && alu_rs2 != 0 && wb_regw) ? 1:0;

endmodule
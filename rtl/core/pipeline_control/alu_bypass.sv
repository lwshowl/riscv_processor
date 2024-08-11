module alu_bypass(
    input [4:0] alu_rs1,
    input [4:0] alu_rs2,
    input [4:0] dmem_rd,
    input dmem_regw,
    output rs1_alu_bypass,
    output rs2_alu_bypass
);

assign rs1_alu_bypass = (alu_rs1 == dmem_rd && alu_rs1 != 0 && dmem_regw ) ? 1 :0;
assign rs2_alu_bypass = (alu_rs2 == dmem_rd && alu_rs2 != 0 && dmem_regw ) ? 1 :0;

endmodule
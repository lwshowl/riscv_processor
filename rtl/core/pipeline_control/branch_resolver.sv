module branch_resolver(
    input alu_branch,
    input [6:0] alu_instrId,
    input [63:0] alu_pc,
    input [63:0] alu_imm64,
    input [63:0] alu_result,
    input [63:0] mepc_val,
    output pc_rel_branch,
    output pc_abs_branch,
    output [63:0] branch_base_pc,
    output [63:0] pc_imm_in
);
  

  assign pc_rel_branch = alu_branch & alu_result[0] & ~pc_abs_branch;
  assign pc_abs_branch = (alu_instrId == `i_jalr) | (alu_instrId == `i_mret);

  assign pc_imm_in = (alu_instrId == `i_jalr) ? {alu_result[63:1],1'b0} :
         (alu_instrId == `i_mret) ? mepc_val + 64'd4 :
         alu_imm64;
  assign branch_base_pc = alu_pc;

endmodule
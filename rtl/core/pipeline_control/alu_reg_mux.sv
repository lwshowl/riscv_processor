module alu_reg_mux(
    input alu_bypass_rs1,
    input alu_bypass_rs2,
    input dmem_bypass_rs1,
    input dmem_bypass_rs2,
    input [63:0] dmem_result,
    input [63:0] alu_rs1val,
    input [63:0] alu_rs2val,
    input [63:0] wb_result,
    output [63:0] rs1val,
    output [63:0] rs2val
);

wire [63:0] alubypass_rs1val = alu_bypass_rs1 ? dmem_result : alu_rs1val;
wire [63:0] alubypass_rs2val = alu_bypass_rs2 ? dmem_result : alu_rs2val;

wire [63:0] dmembypass_rs1val = dmem_bypass_rs1 ? wb_result : alu_rs1val;
wire [63:0] dmembypass_rs2val = dmem_bypass_rs2 ? wb_result : alu_rs2val;

assign rs1val = (alu_bypass_rs1) ? alubypass_rs1val : 
                 dmem_bypass_rs1 ? dmembypass_rs1val :
                                   alu_rs1val;
assign rs2val = (alu_bypass_rs2) ? alubypass_rs2val :
                dmem_bypass_rs2 ? dmembypass_rs2val :
                alu_rs2val;
endmodule
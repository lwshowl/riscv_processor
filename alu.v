module alu #(parameter WIDTH = 32)
            (input clk,
             input rst,
             input [5:0] instr_in,
             input [4:0] op_in,
             input [WIDTH-1:0] rs1,
             input [WIDTH-1:0] rs2,
             input [WIDTH-1:0] imm,
             input [4:0] shamt,
             output [WIDTH-1:0] out);
    
    `include "instructions.v"
    
    reg  [WIDTH-1:0]  rs1_r;
    reg  [WIDTH-1:0]  rs2_r;
    reg  [WIDTH-1:0]  imm_r;
    reg               in_valid_r;
    reg  [WIDTH-1:0]  result;
    reg  [4:0] op_in_r;
    reg  [5:0] instr_in_r;
    reg  [4:0] shamt_r;
    
    
    
    // Register all inputs
    always @ (posedge clk, posedge rst)
    begin
        op_in_r    <= op_in;
        rs1_r      <= rs1;
        rs2_r      <= rs2;
        imm_r      <= imm;
        instr_in_r <= instr_in;
        shamt_r    <= shamt;
        result     <= 0;
    end
    
    always @(*) begin
        case (instr_in_r)
            i_lb:       result    <= rs1_r + imm_r;
            i_lh:       result    <= rs1_r + imm_r;
            i_lw:       result    <= rs1_r + imm_r;
            i_lbu:       result   <= rs1_r + imm_r;
            i_lhu:       result   <= rs1_r + imm_r;
            i_sb:       result    <= rs1_r + imm_r;
            i_sh:       result    <= rs1_r + imm_r;
            i_sw:       result    <= rs1_r + imm_r;
            i_addi:     result    <= rs1_r + imm_r;
            i_slti:     result[0] <= $signed(rs1_r) < $signed(imm_r);
            i_sltiu:    result[0] <= rs1_r < imm_r;
            i_xori:     result    <= rs1_r ^ imm_r;
            i_ori:      result    <= rs1_r | imm_r;
            i_andi:     result    <= rs1_r & imm_r;
            i_slli:     result    <= rs1_r << shamt_r;
            i_srli:     result    <= rs1_r >> shamt_r;
            i_srai:     result    <= rs1_r >>> shamt_r;
            i_add:      result    <= rs1_r + rs2_r;
            i_sub:      result    <= rs1_r + rs2_r;
            i_sll:      result    <= rs1_r << rs2_r;
            i_slt:      result[0] <= $signed(rs1_r) < $signed(imm_r);
            i_sltu:     result[0] <= rs1_r < rs2_r;
            i_xor:      result    <= rs1_r ^ rs2_r;
            i_srl:      result    <= rs1_r >> rs2_r[4:0];
            i_sra:      result    <= rs1_r >>> rs2_r[4:0];
            i_or:       result    <= rs1_r | rs2_r;
            i_and:      result    <= rs1_r & rs2_r;
            
            default: result <= '0;
        endcase
    end
    
    assign out = result;
    
    endmodule;


`include "instructions.v"

module alu #(parameter WIDTH = 32)
            (input clk,
             input rst,
             input [5:0] instr_in,
             input [WIDTH-1:0] rs1,
             input [WIDTH-1:0] rs2,
             input [WIDTH-1:0] imm,
             input [4:0] shamt,
             input [31:0] pc,
             output [WIDTH-1:0] out);
    
    reg  [WIDTH-1:0]  rs1_r;
    reg  [WIDTH-1:0]  rs2_r;
    reg  [WIDTH-1:0]  imm_r;
    reg  [WIDTH-1:0]  result;
    reg  [5:0] instr_in_r;
    reg  [4:0] shamt_r;
    reg  [31:0] pc_r;
    
    
    
    // Register all inputs
    always @ (posedge clk, posedge rst)
    begin
        rs1_r      <= rs1;
        rs2_r      <= rs2;
        imm_r      <= imm;
        instr_in_r <= instr_in;
        shamt_r    <= shamt;
        pc_r       <= pc;
    end
    
    always @(*) begin
        case (instr_in_r)
            i_lui:      result = imm_r;
            i_auipc:    result = pc_r + imm_r;
            i_jal:      result = 32'd1;
            i_jalr:     result = rs1_r + imm_r;
            i_beq:      result = (rs1_r == rs2_r) ? 32'd1 : 32'd0;
            i_bge:      result = ($signed(rs1_r) >= $signed(rs2_r)) ? 32'd1 : 32'd0;
            i_bgeu:     result = (rs1_r >= rs2_r) ? 32'd1 : 32'd0;
            i_blt:      result = ($signed(rs1_r) < $signed(rs2_r)) ?  32'd1 : 32'd0;
            i_bltu:     result = (rs1_r < rs2_r) ? 32'd1 : 32'd0;
            i_bne:      result = (rs1_r != rs2_r) ? 32'd1 : 32'd0;
            i_lb:       result = rs1_r + imm_r;
            i_lh:       result = rs1_r + imm_r;
            i_lw:       result = rs1_r + imm_r;
            i_lbu:      result = rs1_r + imm_r;
            i_lhu:      result = rs1_r + imm_r;
            i_sb:       result = rs1_r + imm_r;
            i_sh:       result = rs1_r + imm_r;
            i_sw:       result = rs1_r + imm_r;
            i_addi:     result = rs1_r + imm_r;
            i_slti:     result = ($signed(rs1_r) < $signed(imm_r)) ? 32'd1 : 32'd0;
            i_sltiu:    result = (rs1_r < imm_r) ? 32'd1 : 32'd0 ;
            i_xori:     result = rs1_r ^ imm_r;
            i_ori:      result = rs1_r | imm_r;
            i_andi:     result = rs1_r & imm_r;
            i_slli:     result = rs1_r << shamt_r;
            i_srli:     result = rs1_r >> shamt_r;
            i_srai:     result = rs1_r >>> shamt_r;
            i_add:      result = rs1_r + rs2_r;
            i_sub:      result = (~rs2_r +1'b1) + rs1_r;
            i_sll:      result = rs1_r << rs2_r[5:0];
            i_slt:      result = ($signed(rs1_r) < $signed(imm_r)) ? 32'd1 : 32'd0;
            i_sltu:     result = (rs1_r < rs2_r) ? 32'd1 : 32'd0;
            i_xor:      result = rs1_r ^ rs2_r;
            i_srl:      result = rs1_r >> rs2_r[4:0];
            i_sra:      result = rs1_r >>> rs2_r[4:0];
            i_and:      result = rs1_r & rs2_r;
            i_or:       result = rs1_r | rs2_r;
            
            default: result = 0;
        endcase
    end
    
    assign out = result;
    
    endmodule;

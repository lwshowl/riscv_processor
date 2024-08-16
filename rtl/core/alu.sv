`include "instructions.sv"

module alu (/* verilator lint_off UNUSED */
            /* verilator lint_off UNUSED */
             input [6:0] instr_in,
             input [63:0] rs1,
             input [63:0] rs2,
             input [63:0] imm,
             input [5:0] shamt,
             input [63:0] pc,
             output reg [63:0] result);
    
    /* verilator lint_off UNOPTFLAT */
    reg [63:0] result_64;
    /* verilator lint_off UNOPTFLAT */
    reg [127:0] result_128;
    wire sign32;

    assign sign32 = result_64[31];

    always_comb begin
        case (instr_in)
            `i_lui:      result = imm;
            `i_auipc:    result = pc + imm;
            `i_jal:      result = 64'd1;
            `i_mret:     result = 64'd1;
            `i_jalr:     result = rs1 + imm;
            `i_beq:      result = (rs1 == rs2) ? 64'd1 : 64'd0;
            `i_bge:      result = ($signed(rs1) >= $signed(rs2)) ? 64'd1 : 64'd0;
            `i_bgeu:     result = (rs1 >= rs2) ? 64'd1 : 64'd0;
            `i_blt:      result = ($signed(rs1) < $signed(rs2)) ?  64'd1 : 64'd0;
            `i_bltu:     result = (rs1 < rs2) ? 64'd1 : 64'd0;
            `i_bne:      result = (rs1 != rs2) ? 64'd1 : 64'd0;
            `i_lb:       result = rs1 + imm;
            `i_lh:       result = rs1 + imm;
            `i_lw:       result = rs1 + imm;
            `i_ld:       result = rs1 + imm;
            `i_lbu:      result = rs1 + imm;
            `i_lhu:      result = rs1 + imm;
            `i_sb:       result = rs1 + imm;
            `i_sh:       result = rs1 + imm;
            `i_sw:       result = rs1 + imm;
            `i_sd:       result = rs1 + imm;
            `i_addi:     result = rs1 + imm;
            `i_slti:     result = ($signed(rs1) < $signed(imm)) ? 64'd1 : 64'd0;
            `i_sltiu:    result = (rs1 < imm) ? 64'd1 : 64'd0 ;
            `i_xori:     result = rs1 ^ imm;
            `i_ori:      result = rs1 | imm;
            `i_andi:     result = rs1 & imm;
            `i_slli:     result = rs1 << shamt;
            `i_srli:     result = rs1 >> shamt;
            `i_srai:     result = rs1 >>> shamt;
            `i_add:      result = rs1 + rs2;
            `i_sub:      result = (~rs2 +1'b1) + rs1;
            `i_sll:      result = rs1 << rs2[5:0];
            `i_slt:      result = ($signed(rs1) < $signed(rs2)) ? 64'd1 : 64'd0;
            `i_sltu:     result = (rs1 < rs2) ? 64'd1 : 64'd0;
            `i_xor:      result = rs1 ^ rs2;
            `i_srl:      result = rs1 >> rs2[4:0];
            `i_sra:      result = rs1 >>> rs2[4:0];
            `i_and:      result = rs1 & rs2;
            `i_or:       result = rs1 | rs2;
            //rv64i
            `i_lwu:      result = rs1 + imm;

            `i_addw:    begin
                result_64 = rs1 + rs2;
                result = {{32{sign32}},result_64[31:0]};
            end 
            `i_subw: begin
                result_64 = rs1 - rs2;
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_sllw:  begin 
                result_64 = rs1 << rs2[4:0];
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_srlw:  begin 
                result_64 = {{32{1'd0}},rs1[31:0] >> rs2[4:0]};
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_sraw:  begin 
                result_64 = {{32{1'd0}},$signed(rs1[31:0]) >>> rs2[4:0]};
                result = {{32{sign32}},result_64[31:0]};
            end
            
            `i_addiw: begin
                result_64 = rs1 + imm;
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_slliw: begin
                result_64 = rs1 << imm[4:0];
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_sraiw: begin
                result_64 ={{32{1'd0}},$signed(rs1[31:0]) >>> imm[4:0]};
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_srliw: begin
                result_64 ={{32{1'd0}},rs1[31:0] >> imm[4:0]};
                result = {{32{sign32}},result_64[31:0]};
            end
            //rv64M
            
            `i_mul : result = $signed(rs1) * $signed(rs2);

            `i_mulh: begin
              result_128 = $signed(rs1) * $signed(rs2);
              result = result_128[127:64];
            end
            `i_mulhsu: begin
                result_128 = $signed(rs1) * $unsigned(rs2);
                result = result_128[127:64];
            end
            `i_mulhu: begin
                result_128 = $unsigned(rs1) * $unsigned(rs2);
                result = result_128[127:64];
            end

            `i_div  : result = $signed(rs1) / $signed(rs2);
            `i_divu : result = $unsigned(rs1) / $unsigned(rs2);
            `i_rem : result = $unsigned(rs1) % $signed(rs2);
            `i_remu : result = $unsigned(rs1) % $unsigned(rs2);

            `i_mulw : begin
                result_64 = $signed(rs1) * $signed(rs2);
                result = {{32{sign32}},result_64[31:0]};
            end

            `i_divw : begin
                result_64 = $signed(rs1) / $signed(rs2);
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_remw : begin
                result_64 = $signed(rs1) % $signed(rs2);
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_divuw : begin
                result_64 = $signed(rs1) / $unsigned(rs2);
                result = {{32{sign32}},result_64[31:0]};
            end
            `i_remuw : begin
                result_64 = $signed(rs1) % $unsigned(rs2);
                result = {{32{sign32}},result_64[31:0]};
            end

            default: result = 64'd1;
        endcase
    end
endmodule

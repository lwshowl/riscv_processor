`include "instructions.v"

module decoder(input clk,
               input [31:0] instr,
               output [4:0] rs1,
               output [4:0] rs2,
               output [4:0] rd,
               output [2:0] func3,
               output [6:0] func7,
               output [31:0] imm,
               output [4:0] shamt,
               output [5:0] instr_id);
    
    
    // i type opcode
    parameter [6:0] i_opcode [0:3] = '{
    7'b0000011, //lb lh lw lbu lhu
    7'b0010011, //addi slti sltiu xori ori andi slli srli srai
    7'b1100111, //jalr
    7'b1110011 //csrrw csrrs csrrc csrrwi csrrsi csrrci
    };
    
    //u type opcode
    parameter [6:0] u_opcode[0:1] = '{
    7'b0110111, // lui
    7'b0010111 // auipc
    };
    
    //j type opcode
    parameter [6:0] j_opcode = 7'b1101111;
    
    //b type opcode
    parameter [6:0] b_opcode = 7'b1100011;
    
    //s type opcode
    parameter [6:0] s_opcode = 7'b0100011;
    
    // r type opcode
    parameter [6:0] r_opcode = 7'b0110011;
    
    // ebreak
    parameter [31:0] ebreak = 32'h00000073;
    
    // ecall
    parameter [31:0] ecall = 32'h001000073;
    
    reg  [31:0] instr_r;
    reg  [6:0]  opcode_r;
    reg  [2:0]  func3_r;
    reg  [6:0]  func7_r;
    /* verilator lint_off UNDRIVEN */
    reg  [31:0] imm_r;
    /* verilator lint_off UNDRIVEN */
    reg  [5:0]  instr_id_r;
    reg  [4:0]  rs1_r;
    reg  [4:0]  rs2_r;
    reg  [4:0]  rd_r;
    
    //register input instruction
    always @(posedge clk) begin
        instr_r  <= instr;
        opcode_r <= instr[6:0];
        rd_r     <= instr[11:7];
        func3_r  <= instr[14:12];
        rs1_r    <= instr[19:15];
        rs2_r    <= instr[24:20];
        func7_r  <= instr[31:25];
    end
    
    //i type decode
    always @(*) begin
        case(opcode_r)
            // 7'b0000011, //lb lh lw lbu lhu
            i_opcode[0]: begin
            imm_r = {{20{instr_r[31]}},instr_r[31:20]};   //sign extend
            //compare fun3 to figure out what instruction
            case(func3_r)
                3'b000:  instr_id_r = i_lb;
                3'b001:  instr_id_r = i_lh;
                3'b010:  instr_id_r = i_lw;
                3'b100:  instr_id_r = i_lbu;
                3'b101:  instr_id_r = i_lhu;
                default: instr_id_r = 63;
            endcase
        end
        
        //7'b0010011, //addi slti sltiu xori ori andi slli srli srai
        i_opcode[1]: begin
            imm_r = {{20{instr_r[31]}},instr_r[31:20]};
            //compare fun3 to figure out what instruction
            case(func3_r)
                3'b000:  instr_id_r = i_addi;
                3'b010:  instr_id_r = i_slti;
                3'b011:  instr_id_r = i_sltiu;
                3'b100:  instr_id_r = i_xori;
                3'b110:  instr_id_r = i_ori;
                3'b111:  instr_id_r = i_andi;
                3'b001:  instr_id_r = i_slli;
                3'b101:  begin
                    if (func7_r == 7'b0100000)begin
                        instr_id_r = i_srai;
                    end
                    else begin
                        instr_id_r = i_srli;
                    end
                    
                end
                default: ;
            endcase
        end
        
        //jalr
        i_opcode[2]: begin
            imm_r      = {{20{instr_r[31]}},instr_r[31:20]};
            instr_id_r = i_jalr;
        end
        
        //7'b1110011 //csrrw csrrs csrrc csrrwi csrrsi csrrci
        i_opcode[3]: begin
            imm_r = {{20{instr_r[31]}},instr_r[31:20]};
            //compare fun3 to figure out what instruction
            case(func3_r)
                3'b001:  instr_id_r = i_csrrw;
                3'b010:  instr_id_r = i_csrrs;
                3'b011:  instr_id_r = i_csrrc;
                3'b101:  instr_id_r = i_csrrwi;
                3'b110:  instr_id_r = i_csrrsi;
                3'b111:  instr_id_r = i_csrrci;
                default: instr_id_r = 63;
            endcase
        end
        
        default: ;
        endcase
    end
    
    
    //u type decode
    always @(*) begin
        case(opcode_r)
            //7'b0110111, // lui
            u_opcode[0]: begin
                imm_r      = {instr_r[31:12],{12{1'b0}}};
                instr_id_r = i_lui;
            end
            //7'b0010111 // auipc
            u_opcode[1]: begin
                imm_r      = {{12{instr_r[31]}},instr_r[31:12]};
                instr_id_r = i_auipc;
            end
            default: ;
        endcase
    end
    
    //j type decode
    always @(*) begin
        case(opcode_r)
            //j_opcode = 7'b1101111;
            j_opcode: begin
                imm_r      = {{11{instr_r[31]}},instr_r[31],instr_r[19:12],instr_r[20],instr_r[30:21],1'b0};
                instr_id_r = i_jal;
            end
            
            default: ;
        endcase
    end
    
    //b type decode
    always @(*) begin
        case(opcode_r)
            //b_opcode = 7'b1100011 ;
            b_opcode: begin
                // imm_r[12]   = instr_r[31];
                // imm_r[10:5] = instr_r[30:25];
                // imm_r[4:1]  = instr_r[11:8];
                // imm_r[11]   = instr_r[7];
                
                imm_r = {{19{instr_r[31]}},instr_r[31],instr_r[7],instr_r[30:25],instr_r[11:8],1'b0};
                //compare fun3 to figure out what instruction
                case(func3_r)
                    3'b000:  instr_id_r = i_beq;
                    3'b001:  instr_id_r = i_bne;
                    3'b100:  instr_id_r = i_blt;
                    3'b101:  instr_id_r = i_bge;
                    3'b110:  instr_id_r = i_bltu;
                    3'b111:  instr_id_r = i_bgeu;
                    default: instr_id_r = 63;
                endcase
            end
            default: ;
        endcase
    end
    
    //s type opcode
    //s_opcode = 7'b0100011;
    always @(*) begin
        case(opcode_r)
            s_opcode: begin
                // imm_r[11:5] = instr_r[31:25];
                // imm_r[4:0]  = instr_r[11:7];
                
                imm_r = {{20{instr_r[31]}},instr_r[31:25],instr_r[11:7]};
                case(func3_r)
                    3'b000: instr_id_r  = i_sb;
                    3'b001: instr_id_r  = i_sh;
                    3'b010: instr_id_r  = i_sw;
                    default: instr_id_r = 63;
                endcase
            end
            
            default: ;
        endcase
    end
    
    // r type opcode
    //r_opcode = 7'b0110011;
    always @(*) begin
        case(opcode_r)
            r_opcode: begin
                imm_r = 0;
                case(func3_r)
                    3'b000: begin
                        if (func7_r == 0)begin
                            instr_id_r = i_add;
                        end
                        else begin
                            instr_id_r = i_sub;
                        end
                    end
                    3'b001: instr_id_r = i_sll;
                    3'b010: instr_id_r = i_slt;
                    3'b011: instr_id_r = i_sltu;
                    3'b100: instr_id_r = i_xor;
                    3'b101: begin
                        if (func7_r == 0)begin
                            instr_id_r = i_srl;
                        end
                        else begin
                            instr_id_r = i_sra;
                        end
                    end
                    3'b110: instr_id_r  = i_or;
                    3'b111: instr_id_r  = i_and;
                    default: instr_id_r = 63;
                endcase
            end
            
            default: ;
        endcase
    end
    
    
    //ebreak = 32'h00000073;
    //ecall  = 32'h001000073;
    always @(*) begin
        case(instr_r)
            ebreak: instr_id_r  = i_ebreak;
            ecall: instr_id_r   = i_ecall;
            default: instr_id_r = 63;
        endcase
    end
    
    assign shamt    = rs2_r;
    assign rs1      = rs1_r;
    assign rs2      = rs2_r;
    assign rd       = rd_r;
    assign func3    = func3_r;
    assign func7    = func7_r;
    assign instr_id = instr_id_r;
    assign imm      = imm_r;
    
endmodule

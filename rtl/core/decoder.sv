`include "instructions.sv"

module decoder(/* verilator lint_off UNUSED */
                input clk,
                /* verilator lint_off UNUSED */
               input [31:0] instr,
               output reg [4:0] rs1,
               output reg [4:0] rs2,
               output reg [4:0] rd,
               output reg [63:0] imm,
               output reg [5:0] shamt,
               output reg [6:0] opcode,
               output reg branch,
               output reg reg_w,
               output reg mem_r,
               output reg mem_w,
               output reg [6:0] instr_id);
    
    // i type opcode
    localparam [6:0] i_opcode1 = 7'b0000011; //lb lh lw lbu lhu
    localparam [6:0] i_opcode2 = 7'b0010011; //addi slti sltiu xori ori andi slli srli srai
    localparam [6:0] i_opcode3 = 7'b1100111; //jalr
    localparam [6:0] i_opcode4 = 7'b1110011; //csrrw csrrs csrrc csrrwi csrrsi csrrci ebreak ecall
    //u type opcode
    localparam [6:0] u_opcode1 = 7'b0110111; // lui
    localparam [6:0] u_opcode2 = 7'b0010111; // auipc
    //j type opcode
    localparam [6:0] j_opcode = 7'b1101111;
    //b type opcode
    localparam [6:0] b_opcode = 7'b1100011;
    //s type opcode
    localparam [6:0] s_opcode = 7'b0100011;
    // r type opcode
    localparam [6:0] r_opcode = 7'b0110011;
    // ebreak
    localparam [31:0] ebreak = 32'h00000073;
    //rv64i R
    localparam [6:0] rv64ir_opcode = 7'b0111011;
    //rv64i I
    localparam [6:0] rv64ii_opcode = 7'b0011011;
    // ecall
    localparam [31:0] ecall = 32'h001000073;
    
    localparam [6:0] bubble_opcode = 7'b0000000;
    
    wire [2:0] func3;
    wire [6:0] func7;
    
    assign opcode = instr[6:0];
    assign rd     = (opcode == 7'b1100011 || opcode == 7'b0100011 ) ? 5'd0 :instr[11:7];
    assign func3  = instr[14:12];
    assign rs1    = instr[19:15];
    assign rs2    = instr[24:20];
    assign func7  = instr[31:25];
    assign shamt  = instr[25:20];
    
    //i type decode
    always_comb begin
        case(opcode)
            // 7'b0000011, //lb lh lw lbu lhu
            i_opcode1: begin
            imm    = {{52{instr[31]}}, instr[31:20]};   // sign extend
            mem_r  = 1;
            mem_w  = 0;
            reg_w  = 1;
            branch = 0;
            // compare fun3 to figure out what instruction
            case(func3)
                3'b000:  instr_id = `i_lb;
                3'b001:  instr_id = `i_lh;
                3'b010:  instr_id = `i_lw;
                3'b011:  instr_id = `i_ld;
                3'b100:  instr_id = `i_lbu;
                3'b101:  instr_id = `i_lhu;
                3'b110:  instr_id = `i_lwu;
                default: instr_id = `i_invalid;
            endcase
        end
        
        //7'b0010011, //addi slti sltiu xori ori andi slli srli srai
        i_opcode2: begin
            imm    = {{52{instr[31]}},instr[31:20]};
            mem_r  = 0;
            mem_w  = 0;
            reg_w  = 1;
            branch = 0;
            //compare fun3 to figure out what instruction
            case(func3)
                3'b000:  instr_id = `i_addi;
                3'b010:  instr_id = `i_slti;
                3'b011:  instr_id = `i_sltiu;
                3'b100:  instr_id = `i_xori;
                3'b110:  instr_id = `i_ori;
                3'b111:  instr_id = `i_andi;
                3'b001:  begin
                    casez(func7)
                        7'b000000?: instr_id = `i_slli;
                        default: instr_id = `i_invalid;
                    endcase
                end
                3'b101:  begin
                    casez(func7)
                        7'b010000?: instr_id = `i_srai;
                        7'b000000?: instr_id = `i_srli;
                        default: instr_id    = `i_invalid;
                    endcase
                end
                default:instr_id = `i_invalid ;
            endcase
        end
        
        //jalr
        i_opcode3: begin
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 1;
            branch   = 1;
            imm      = {{52{instr[31]}},instr[31:20]};
            instr_id = `i_jalr;
        end
        
        //7'b1110011 //csrrw csrrs csrrc csrrwi csrrsi csrrci
        i_opcode4: begin
            mem_r  = 0;
            mem_w  = 0; 
            reg_w  = func3[0] | func3[1] | func3[2];
            branch = 0;
            imm    = {{52{1'd0}},instr[31:20]};
            //compare fun3 to figure out what instruction
            case(func3)
                3'b001:  instr_id = `i_csrrw;
                3'b010:  instr_id = `i_csrrs;
                3'b011:  instr_id = `i_csrrc;
                3'b101:  instr_id = `i_csrrwi;
                3'b110:  instr_id = `i_csrrsi;
                3'b111:  instr_id = `i_csrrci;
                3'b000:  begin
                    case(instr[31:7])
                        25'd0 : instr_id   = `i_ecall;
                        25'd8192 :instr_id = `i_ebreak;
                        25'h604000: instr_id = `i_mret;
                        default : instr_id = `i_invalid;
                    endcase
                end
                default: instr_id = `i_invalid;
            endcase
        end
        
        //7'b0110111, // lui
        u_opcode1: begin
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 1;
            branch   = 0;
            imm      = {{32{instr[31]}},instr[31:12],{12{1'b0}}};
            instr_id = `i_lui;
        end

        
        //7'b0010111 // auipc
        u_opcode2: begin
            imm      = {{32{instr[31]}},instr[31:12],{12{1'b0}}};
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 1;
            branch   = 0;
            instr_id = `i_auipc;
        end
        
        //j_opcode = 7'b1101111;
        j_opcode: begin
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 1;
            branch   = 1;
            imm      = {{43{instr[31]}},instr[31],instr[19:12],instr[20],instr[30:21],1'b0};
            instr_id = `i_jal;
        end
        
        //b_opcode = 7'b1100011 ;
        b_opcode: begin
            mem_r  = 0;
            mem_w  = 0;
            reg_w  = 1;
            branch = 1;
            imm    = {{51{instr[31]}},instr[31],instr[7],instr[30:25],instr[11:8],1'b0};
            //compare fun3 to figure out what instruction
            case(func3)
                3'b000:  instr_id = `i_beq;
                3'b001:  instr_id = `i_bne;
                3'b100:  instr_id = `i_blt;
                3'b101:  instr_id = `i_bge;
                3'b110:  instr_id = `i_bltu;
                3'b111:  instr_id = `i_bgeu;
                default: instr_id = `i_invalid;
            endcase
        end
        
        s_opcode: begin
            mem_r  = 0;
            mem_w  = 1;
            reg_w  = 0;
            branch = 0;
            imm    = {{52{instr[31]}},instr[31:25],instr[11:7]};
            case(func3)
                3'b000: instr_id  = `i_sb;
                3'b001: instr_id  = `i_sh;
                3'b010: instr_id  = `i_sw;
                3'b011: instr_id  = `i_sd;
                default: instr_id = `i_invalid;
            endcase
        end
        
        r_opcode: begin
            imm    = 0;
            mem_r  = 0;
            mem_w  = 0;
            reg_w  = 1;
            branch = 0;
            case(func3)
                3'b000: begin
                    case (func7)
                        7'b0000000: instr_id = `i_add;
                        7'b0100000: instr_id = `i_sub;
                        7'b0000001: instr_id = `i_mul;
                        default: instr_id    = `i_invalid;
                    endcase
                end
                3'b001: begin
                    case (func7)
                        7'b0000000:instr_id = `i_sll;
                        7'b0000001: instr_id = `i_mulh;
                        default : instr_id = `i_invalid;
                    endcase
                end
                3'b010: begin
                    case (func7)
                        7'b0000000: instr_id = `i_slt;
                        7'b0000001: instr_id = `i_mulhsu;
                        default: instr_id = `i_invalid;
                    endcase
                end
                3'b011: begin
                  case (func7)
                    7'b0000000: instr_id = `i_sltu;
                    7'b0000001: instr_id = `i_mulhu;
                    default: instr_id = `i_invalid;
                  endcase
                end 
                3'b100: begin
                  case (func7)
                    7'b0000000: instr_id = `i_xor;
                    7'b0000001: instr_id = `i_div;
                    default: instr_id = `i_invalid;
                  endcase
                end
                3'b101: begin
                    case (func7)
                        7'b0000000: instr_id = `i_srl;
                        7'b0100000: instr_id = `i_sra;
                        7'b0000001: instr_id = `i_divu;
                        default: instr_id    = `i_invalid;
                    endcase
                end
                3'b110: begin
                    case (func7)
                        7'b0000000: instr_id = `i_or;
                        7'b0000001: instr_id = `i_rem;
                        default: instr_id    = `i_invalid;
                    endcase                
                end
                3'b111: begin
                    case (func7)
                        7'b0000000: instr_id = `i_and;
                        7'b0000001: instr_id = `i_remu;
                        default: instr_id    = `i_invalid;
                    endcase                
                end
                default: instr_id = `i_invalid;
            endcase
        end
        
        bubble_opcode: begin
            imm      = 0;
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 0;
            branch   = 0;
            instr_id = `i_bubble;
        end
        
        //7'b0111011
        rv64ir_opcode: begin
            imm    = 0;
            mem_r  = 0;
            mem_w  = 0;
            reg_w  = 1;
            branch = 0;
            case (func3)
                3'b000: begin
                    case(func7)
                        7'b0000000: instr_id = `i_addw;
                        7'b0100000: instr_id = `i_subw;
                        7'b0000001: instr_id = `i_mulw;
                        default: instr_id    = `i_invalid;
                    endcase
                end
                3'b001: begin
                    case (func7)
                        7'b0000000: instr_id = `i_sllw;
                        default:    instr_id = `i_invalid;
                    endcase
                end
                3'b100: begin
                    case (func7)
                        7'b0000001: instr_id = `i_divw;
                        default: instr_id = `i_invalid;
                    endcase
                end
                3'b101: begin
                    case(func7)
                        7'b0000000: instr_id = `i_srlw;
                        7'b0100000: instr_id = `i_sraw;
                        7'b0000001: instr_id = `i_divuw;
                        default: instr_id = `i_invalid;
                    endcase
                end
                3'b110: begin
                    case(func7)
                        7'b0000001: instr_id = `i_remw;
                        default: instr_id = `i_invalid;
                    endcase
                end
                3'b111: begin
                    case(func7)
                        7'b0000001: instr_id = `i_remuw;
                        default: instr_id = `i_invalid;
                    endcase
                end
                default: instr_id = `i_invalid;
            endcase
        end

        rv64ii_opcode: begin
            imm    = 0;
            mem_r  = 0;
            mem_w  = 0;
            reg_w  = 1;
            branch = 0;
            imm    = {{52{instr[31]}},instr[31:20]};
            case (func3)
                3'b000: instr_id = `i_addiw;
                3'b001: instr_id = `i_slliw;
                3'b101: begin
                  case (func7)
                  7'b0100000: instr_id = `i_sraiw;
                  7'b0000000: instr_id = `i_srliw;
                  default : instr_id = `i_invalid; 
                  endcase
                end 
                default: instr_id = `i_invalid;
            endcase
        end
        
        default: begin
            imm      = 0;
            mem_r    = 0;
            mem_w    = 0;
            reg_w    = 0;
            branch   = 0;
            instr_id = `i_invalid;
        end
        endcase
    end
endmodule

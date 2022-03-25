`include "instructions.v"

module tinyrv32(input clk,
                input rst,
                output reg [31:0] m_addr,
                output reg [31:0] m_data,
                output reg wea
                );
    
    
    //???????? pc ,??????
   (*mark_debug="true"*)wire fetch_clk;
   (*mark_debug="true"*)wire write_clk;
    wire b_enable;
    wire [31:0] pc_imm_in;
    //pc ????31 ?????
    (*mark_debug="true"*)wire [31:0] pc_out;
    wire pc_abs_branch;
    
    //????
    clock_divider #(.divider (64),.ratio1(1/2),.ratio2 (3/4)) clk_div (
    .clk_in (clk),
    .clk_out1 (fetch_clk),
    .clk_out2 (write_clk)
    );
    
    pc pc32(
    .clk (clk),
    .r_clk (fetch_clk),
    .w_clk (write_clk),
    .rst (rst),
    .branch (b_enable),
    .abs_branch (pc_abs_branch),
    .immediate (pc_imm_in),
    .pc_out_reg (pc_out)
    );
    /********************************************/
    
    //??????
    (*mark_debug="true"*)wire [3:0] mem_w_enable;
    wire [31:0] mem_data_in;
    //?????????
    (*mark_debug="true"*)wire [31:0] instr;
    wire [31:0] w_addr_select;
    wire [31:0] r_addr_select;
    (*mark_debug="true"*)wire [31:0] mem_data_out;
    
    
  blk_mem mem32 (
  .clka(clk),               // input wire clka
  .wea(0),                  // input wire [3 : 0] wea
  .addra(pc_out),           // input wire [14 : 0] addra
  .dina(0),                 // input wire [31 : 0] dina
  .douta(instr),            // output wire [31 : 0] douta
  .clkb(clk),               // input wire clkb
  .web(mem_w_enable),       // input wire [3 : 0] web
  .addrb(r_addr_select),    // input wire [14 : 0] addrb
  .dinb(mem_data_in),       // input wire [31 : 0] dinb
  .doutb(mem_data_out),  // output wire [31 : 0] doutb
  .rsta_busy(),
  .rstb_busy(),
  .rsta(0),
  .rstb(0)
  );
  
  

    /*********************************************/
    
    //??,??????????????
    (*mark_debug="true"*)wire [4:0] rs1;
    (*mark_debug="true"*)wire [4:0] rs2;
    (*mark_debug="true"*)wire [4:0] rd;
    wire [2:0] func3;
    /* verilator lint_off UNUSED */
    wire [6:0] func7;
    /* verilator lint_off UNUSED */
    wire [31:0] imm32;
    wire [4:0] shamt;
    (*mark_debug="true"*)wire [5:0] instr_id;
    wire [6:0] opcode;
        
    decoder dec(
    .clk (fetch_clk),
    .instr (instr), //input
    .rs1 (rs1),
    .rs2 (rs2),
    .rd (rd),
    .func3 (func3),
    .func7 (func7),
    .imm (imm32),
    .shamt (shamt),
    .instr_id (instr_id),
    .opcode (opcode)
    );
    /*********************************************/
        
    //?????
    wire reg_file_read;     //????  >-_->
    (*mark_debug="true"*)wire reg_file_write;   //???
    reg [31:0] rf_write_val;
    //???
    (*mark_debug="true"*)wire [31:0] rs1_val;
    (*mark_debug="true"*)wire [31:0] rs2_val;
    
    assign reg_file_read = 1; //???????????????
    
    regfile rfile32(
    .r_clk (clk),
    .w_clk (write_clk),
    .r_enable (reg_file_read),
    .w_enable (reg_file_write),
    .rs1_select (rs1),
    .rs2_select (rs2),
    .rd_select (rd),
    .w_val (rf_write_val),
    .rs1_out (rs1_val),
    .rs2_out (rs2_val)
    );
    /************************************************/
    
    //ALU ???????
    (*mark_debug="true"*)wire [31:0] alu_out;
    
    alu alu32(
    .clk (clk),
    .rst (rst),
    .instr_in (instr_id),
    .rs1 (rs1_val),
    .rs2 (rs2_val),
    .imm (imm32),
    .shamt (shamt),
    .pc (pc_out),
    .out (alu_out)
    );
    //???????????????
    //btype                   //jal
    assign b_enable      = ((opcode === 7'b1100011) | (opcode === 7'b1101111)) & (instr_id != `i_invalid) & alu_out[0];
    assign pc_abs_branch = (instr_id == `i_jalr);
    //???????????????? ?
    //jalr ??????PC???????????? alu ?????????????????? ALU ????
    assign pc_imm_in = (instr_id == `i_jalr) ? (rs1_val + imm32) : imm32;
    
    /****************************************************/
    
    //??
    // wire dmem_r_enable = 1;
    // wire dmem_w_enable;
    // wire [31:0] dmem_addr;
    // // wire [31:0] dmem_in;
    // wire [31:0] dmem_out;
    
    assign mem_w_enable  = {4{(((instr_id == `i_sb) | (instr_id == `i_sh) | (instr_id == `i_sw)) & !write_clk)}};                          //s type opcode
    assign w_addr_select = alu_out;                                         //?????ALU ????
    assign mem_data_in   = rs2_val & {{16{func3[1]}},{8{func3[0]|func3[1]}},8'hff};  //? store ???rs2 ? word half byte
    assign r_addr_select = alu_out;
    
    always @(negedge write_clk) begin
        m_addr <= alu_out;
        m_data <= mem_data_in;
        wea    <= ((instr_id == `i_sb) | (instr_id == `i_sh) | (instr_id == `i_sw));
    end
    
    /**************************************************/
    
    //?????
    assign reg_file_write = ((opcode == 7'b0110111) |        //lui
    (opcode == 7'b0010111) |                               //auipc
    (opcode == 7'b0000011) |                                //lb lh lw
    (opcode == 7'b0010011) |                                //addi slti ……
    (opcode == 7'b0110011) |                                //add sub ……
    (opcode == 7'b1101111) |                                //jal
    (opcode == 7'b1100111))&                                //jalr
    (instr_id != `i_invalid);                              //is a valid instruction ?
    
    always @(*) begin
        case(opcode)
            7'b0000011: begin                                                           // lb lh lw lbu lhu
                case(func3)
                    3'b000: rf_write_val = {{24{mem_data_out[7]}},mem_data_out[7:0]};   //lb
                    3'b001: rf_write_val = {{16{mem_data_out[7]}},mem_data_out[15:0]};  //lh
                    3'b010: rf_write_val = mem_data_out;                                //lw
                    3'b100: rf_write_val = {{24{1'b0}},mem_data_out[7:0]};              //lbu
                    3'b101: rf_write_val = {{16{1'b0}},mem_data_out[15:0]};             //lhu
                    default : ;
                endcase
            end
            7'b1101111: rf_write_val = pc_out + 4;                                  //jal
            7'b1100111: rf_write_val = pc_out + 4;                                  //jalr
            default : rf_write_val   = alu_out;                                     // ????
        endcase
    end
endmodule

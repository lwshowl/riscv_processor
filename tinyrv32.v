module tinyrv32(input clk,
                input rst);
    
    
    
    wire [31:0] pc_imm;
    wire b_enable;
    wire [31:0] pc_out;
    wire imem_w_enable;
    wire imem_r_enable;
    wire [31:0] imem_data_in;
    wire [31:0] imem_data_out;
    wire [31:0] imm_select;
    wire [4:0] rs1;
    wire [4:0] rs2;
    wire [4:0] rd;
    wire [2:0] func3;
    wire [6:0] func7;
    wire [31:0] imm;
    wire [5:0] instr_id;
    
    
    memory imem(
    .clk (clk),
    .w_enable (imem_w_enable),
    .r_enable (imem_r_enable),
    .addr_select(imm_select),
    .data_in (imem_data_in),
    .data_out (imem_data_out)
    );
    
    pc pc32(
    .clk (clk),
    .rst (rst),
    .branch (b_enable),
    .immediate (pc_imm),
    .pc_out_reg (pc_out)
    );
    
    decoder dec(
    .clk (clk),
    .instr (imem_data_out),
    .rs1 (rs1),
    .rs2 (rs2),
    .rd (rd),
    .func3 (func3),
    .func7 (func7),
    .imm (imm),
    .instr_id (instr_id)
    );
    
    
    
    
    
    
    
endmodule

module tinyrv32(input clk,
                input rst);
    
    //将时钟分频后输入 pc ,控制取指频率
    wire fetch_clk;
    wire write_clk;
    wire b_enable;
    wire [31:0] pc_imm_in;
    //pc 的输出是31 位内存地址
    wire [31:0] pc_out;
    
    //分频时钟
    clock_divider #(.divider (8),.ratio1(1/2),.ratio2 (3/4)) clk_div (
    .clk_in (clk),
    .clk_out1 (fetch_clk),
    .clk_out2 (write_clk)
    );
    
    pc pc32(
    .clk (fetch_clk),
    .rst (rst),
    .branch (b_enable),
    .immediate (pc_imm_in),
    .pc_out_reg (pc_out)
    );
    /********************************************/
    
    //访问指令内存
    wire imem_w_enable;
    wire [31:0] imem_data_in;
    wire imem_r_enable;
    //指令内存输出的指令
    wire [31:0] imem_data_out;
    
    assign imem_data_in  = 0;     //用不上 指令内存写死
    assign imem_r_enable = 1;    //无所谓
    assign imem_w_enable = 0;    //指令内存不可写
    
    memory imem(
    .clk (clk),
    .w_enable (imem_w_enable),
    .r_enable (imem_r_enable),
    .addr_select(pc_out),
    .data_in (imem_data_in),
    .data_out (imem_data_out)
    );
    /*********************************************/
    
    //译码,译码器输出的指令各种需要的值
    wire [4:0] rs1;
    wire [4:0] rs2;
    wire [4:0] rd;
    wire [2:0] func3;
    /* verilator lint_off UNUSED */
    wire [6:0] func7;
    /* verilator lint_off UNUSED */
    wire [31:0] imm32;
    wire [4:0] shamt;
    wire [5:0] instr_id;
    wire [6:0] opcode;
    
    assign opcode = imem_data_out[6:0];
    
    decoder dec(
    .clk (clk),
    .instr (imem_data_out), //input
    .rs1 (rs1),
    .rs2 (rs2),
    .rd (rd),
    .func3 (func3),
    .func7 (func7),
    .imm (imm32),
    .shamt (shamt),
    .instr_id (instr_id)
    );
    /*********************************************/
    
    //访问寄存器
    wire reg_file_read;     //无用属性  >-_->
    wire reg_file_write;   //先声明
    reg [31:0] rf_write_val;
    //输出值
    wire [31:0] rs1_val;
    wire [31:0] rs2_val;
    
    assign reg_file_read = 1; //寄存器堆始终可读，无论设置与否
    
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
    
    //ALU 计算地址或者值
    wire [31:0] alu_out;
    
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
    //此时是否分支应该也已经计算完毕
    //btype                   //jal                  //jalr
    assign b_enable  = (opcode == 7'b1100011) | (opcode == 7'b1101111) | (opcode == 7'b1100111) & alu_out[0];
    assign pc_imm_in = (instr_id == i_jalr)? (imm32 + rs1_val) : imm32;
    
    /****************************************************/
    
    //访存
    wire dmem_r_enable = 1;
    wire dmem_w_enable;
    wire [31:0] dmem_addr;
    wire [31:0] dmem_in;
    wire [31:0] dmem_out;
    
    assign dmem_w_enable = opcode == 7'b0100011 ;//s type opcode
    assign dmem_addr     = alu_out; //地址已经在ALU 里计算好
    assign dmem_in       = rs2_val & {{16{func3[1]}},{8{func3[0]}},8'hff}; //要 store 的值为rs2 的 word half byte
    
    memory dmem(
    .clk (clk),
    .w_enable (dmem_w_enable),
    .r_enable (dmem_r_enable),
    .addr_select(dmem_addr),
    .data_in (dmem_in),
    .data_out (dmem_out)
    );
    /**************************************************/
    
    //回写寄存器
    assign reg_file_write = (opcode == 7'b0110111) |   //lui
    (opcode == 7'b0010111) |   //auipc
    (opcode == 7'b0000011) |   //lb lh lw
    (opcode == 7'b0010011) |   //addi slti ……
    (opcode == 7'b0110011);    //add sub ……
    
    wire lbu;
    wire lhu;
    
    // lb lh lw lbu lhu
    always @(*) begin
        case(opcode)
            7'b0000011: begin
                case(func3)
                    3'b000: rf_write_val = {{24{dmem_out[7]}},dmem_out[7:0]};  //lb
                    3'b001: rf_write_val = {{16{dmem_out[7]}},dmem_out[15:0]}; //lh
                    3'b010: rf_write_val = dmem_out;                           //lw
                    3'b100: rf_write_val = {{24{1'b0}},dmem_out[7:0]};           //lbu
                    3'b101: rf_write_val = {{16{1'b0}},dmem_out[15:0]};          //lhu
                    default : ;
                endcase
            end
            default : rf_write_val = alu_out; // 其他指令
        endcase
    end
endmodule

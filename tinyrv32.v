module tinyrv32(input clk,
                input rst);
    
    //将时钟分频后输入 pc ,控制取指频率
    wire fetch_clk;
    wire write_clk;
    wire b_enable;
    wire [31:0] pc_imm_in;
    //pc 的输出是31 位内存地址
    wire [31:0] pc_out;
    wire pc_abs_branch;
    
    //分频时钟
    clock_divider #(.divider (8),.ratio1(1/2),.ratio2 (3/4)) clk_div (
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
    
    //访问指令内存
    wire mem_w_enable;
    wire [31:0] mem_data_in;
    wire mem_r_enable;
    //指令内存输出的指令
    wire [31:0] instr;
    wire [31:0] w_addr_select;
    wire [31:0] r_addr_select;
    wire [31:0] mem_data_out;
    
    assign mem_r_enable = 1;    //无所谓
    
    memory mem(
    .clk (clk),
    .w_clk (write_clk),
    .w_enable (mem_w_enable),
    .r_enable (mem_r_enable),
    .r_addr_select1(pc_out),
    .r_addr_select2(r_addr_select),
    .w_addr_select(w_addr_select),
    .data_in (mem_data_in),
    .data_out1 (instr),
    .data_out2 (mem_data_out)
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
    
    assign opcode = instr[6:0];
    
    decoder dec(
    .clk (clk),
    .instr (instr), //input
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
    //btype                   //jal
    assign b_enable      = ((opcode === 7'b1100011) | (opcode === 7'b1101111)) & alu_out[0];
    assign pc_abs_branch = (instr_id == i_jalr);
    //绝大多数转移地址由立即数直接给出 ，
    //jalr 需要进行一次地址计算，原则上来说 alu 只进行内存地址计算，最好是在 ALU 外部计算
    assign pc_imm_in = (instr_id == i_jalr) ? (rs1_val + imm32) : imm32;
    
    /****************************************************/
    
    //访存
    // wire dmem_r_enable = 1;
    // wire dmem_w_enable;
    // wire [31:0] dmem_addr;
    // // wire [31:0] dmem_in;
    // wire [31:0] dmem_out;
    
    assign mem_w_enable  = (instr_id == i_sb) | (instr_id == i_sh) | (instr_id == i_sw) ;                           //s type opcode
    assign w_addr_select = alu_out;                                         //地址已经在ALU 里计算好
    assign mem_data_in   = rs2_val & {{16{func3[1]}},{8{func3[0]|func3[1]}},8'hff};  //要 store 的值为rs2 的 word half byte
    
    assign r_addr_select = alu_out;
    
    // memory imem(
    // .clk (clk),
    // .w_enable (imem_w_enable),
    // .r_enable (imem_r_enable),
    // .addr_select(pc_out),
    // .data_in (imem_data_in),
    // .data_out (iinstr)
    // );
    
    
    // memory dmem(
    // .clk (clk),
    // .w_enable (dmem_w_enable),
    // .r_enable (dmem_r_enable),
    // .addr_select(dmem_addr),
    // .data_in (dmem_in),
    // .data_out (dmem_out)
    // );
    
    /**************************************************/
    
    //回写寄存器
    assign reg_file_write = (opcode == 7'b0110111) |    //lui
    ((opcode == 7'b0010111) |                            //auipc
    (opcode == 7'b0000011) |                            //lb lh lw
    (opcode == 7'b0010011) |                            //addi slti ……
    (opcode == 7'b0110011) |                            //add sub ……
    (opcode == 7'b1101111) |                            //jal
    (opcode == 7'b1100111))                            //jalr
    &(instr_id != i_invalid);                            //is a valid instruction ?
    
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
            default : rf_write_val   = alu_out;                                     // 其他指令
        endcase
    end
endmodule

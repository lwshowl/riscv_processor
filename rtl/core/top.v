`include "instructions.v"
`include "exceptions.v"

module top(input clk,
           input rst);
    
    wire w_clk;
    wire r_clk;
    
    assign w_clk = clk;
    assign r_clk = ~clk;
    
    //pc 的输入
    //两种分支模式，直接分支，和伪直接分支
    wire pc_rel_branch;
    wire pc_abs_branch;
    wire pc_exception;
    //分支立即数为32位宽
    wire [63:0] pc_ref_pc;
    wire [63:0] pc_imm_in;
    wire [63:0] mepc_val;
    //pc 的输出是 指令指针
    wire [63:0] pc_out;
    
    pc pc64(
    .clk (w_clk),
    .rst (rst),
    .bubble(memory_bubble),
    .exception(pc_exception),
    .rel_branch (pc_rel_branch),
    .abs_branch (pc_abs_branch),
    .immediate (pc_imm_in),
    .mtvec(mtvec_val),
    .ref_pc (pc_ref_pc),
    .pc_out_reg (pc_out)
    );
    
    /********************取指***************************/
    // 指令内存输出的值，既为指令
    /* verilator lint_off UNUSED */
    wire [63:0] instr64;
    /* verilator lint_off UNUSED */
    reg [31:0] instr32;
    
    // DPI-C接口，内存在外部编程语言中定义
    import "DPI-C" function void npc_mem_read(
    input longint raddr,output longint rdata);
    // 访存
    always @(r_clk) begin
        if(!rst)
            npc_mem_read(pc_out,instr64);
    end
    assign instr32 = instr64[31:0];
    wire [15:0] fetch_exception = 0;
    /**************译码，生成立即数*************************/
    //译码阶段过程寄存器
    wire [31:0] decode_instr_out;
    wire [63:0] decode_pc_out;
    wire decode_rst;
    wire decode_wen;
    assign decode_rst = pc_rel_branch | pc_abs_branch | fetch_exception > 0 | wb_exception > 0;
    assign decode_wen = ~memory_bubble;
    
    Reg #(32,0) decode_instr (w_clk,decode_rst,instr32,decode_instr_out,decode_wen);
    Reg #(64,0) decode_pc (w_clk,decode_rst,pc_out,decode_pc_out,decode_wen);
    // 异常寄存器
    wire [15:0] decode_exception;
    wire [15:0] fetch_excep_out;
    wire decode_excep_rst = pc_abs_branch | pc_rel_branch | wb_exception >0;
    Reg #(16,0) decode_excep(w_clk,decode_excep_rst,fetch_exception,fetch_excep_out,decode_wen);
    assign decode_exception = 0 | fetch_excep_out;

    //译码器的输出
    wire [4:0] rs1;
    wire [4:0] rs2;
    wire [4:0] rd;
    wire [31:0] imm32;
    wire [5:0] shamt;
    wire [6:0] instr_id;
    wire branch;
    
    wire reg_w;
    wire mem_r;
    wire mem_w;
    wire [6:0] opcode;
    
    decoder dec(
    .clk (r_clk),
    .instr (decode_instr_out),
    .rs1 (rs1),
    .rs2 (rs2),
    .rd (rd),
    .branch(branch),
    .reg_w(reg_w),
    .mem_r(mem_r),
    .mem_w(mem_w),
    .imm (imm32),
    .shamt (shamt),
    .opcode (opcode),
    .instr_id (instr_id)
    );
    /*****************访问寄存器***********************/
    //寄存器阶段 过程寄存器
    wire regfile_rst;
    wire regfile_wen;
    wire [4:0] regfile_rs1_out;
    wire [4:0] regfile_rs2_out;
    wire [4:0] regfile_rd_out;
    wire [63:0] regfile_pc_out;
    wire [63:0] regfile_imm64_out;
    wire [5:0] regfile_shamt_out;
    wire [6:0] regfile_opcode_out;
    wire [6:0] regfile_instrId_out;
    wire regfile_branch_out;
    wire regfile_regw_out;
    wire regfile_memr_out;
    wire regfile_memw_out;
    
    assign regfile_rst = pc_rel_branch | pc_abs_branch | decode_exception > 0 | wb_exception > 0;
    assign regfile_wen = ~memory_bubble;
    
    Reg #(5,0) regfile_rs1 (w_clk,regfile_rst,rs1,regfile_rs1_out,regfile_wen);
    Reg #(5,0) regfile_rs2 (w_clk,regfile_rst,rs2,regfile_rs2_out,regfile_wen);
    Reg #(5,0) regfile_rd (w_clk,regfile_rst,rd,regfile_rd_out,regfile_wen);
    Reg #(64,0) regfile_pc (w_clk,regfile_rst,decode_pc_out,regfile_pc_out,regfile_wen);
    Reg #(64,0) regfile_imm64 (w_clk,regfile_rst,{{32{imm32[31]}},imm32},regfile_imm64_out,regfile_wen);
    Reg #(6,0) regfile_shamt (w_clk,regfile_rst,shamt,regfile_shamt_out,regfile_wen);
    Reg #(7,0) regfile_opcode (w_clk,regfile_rst,opcode,regfile_opcode_out,regfile_wen);
    Reg #(7,0) regfile_instrId (w_clk,regfile_rst,instr_id,regfile_instrId_out,regfile_wen);
    Reg #(1,0) regfile_branch (w_clk,regfile_rst,branch,regfile_branch_out,regfile_wen);
    Reg #(1,0) regfile_regw (w_clk,regfile_rst,reg_w,regfile_regw_out,regfile_wen);
    Reg #(1,0) regfile_memr (w_clk,regfile_rst,mem_r,regfile_memr_out,regfile_wen);
    Reg #(1,0) regfile_memw (w_clk,regfile_rst,mem_w,regfile_memw_out,regfile_wen);
    // 访存异常寄存器
    wire [15:0] decode_excep_out;
    wire [15:0] regfile_exception;
    wire regfile_excep_rst = pc_rel_branch | pc_abs_branch | wb_exception >0;
    Reg #(16,0) regfile_excep(w_clk,regfile_excep_rst,decode_excep_out,decode_excep_out,regfile_wen);
    assign regfile_exception = 0 | decode_excep_out;
    //检查是否有相邻的 ld / st
    reg  ldst_bubbule;
    reg  regld_bubble;
    wire memory_bubble;
    
    assign ldst_bubbule = ((regfile_opcode_out == 7'b0000011) &  //load
                                        (alu_opcode_out == 7'b0100011)) ? 1 : 0; // store
                                        
    assign regld_bubble = ((regfile_regw_out == 1 | regfile_memw_out ==1) &
                                    (alu_opcode_out == 7'b0000011) &     //load
                                            (regfile_rs2_out == alu_rd_out) | (regfile_rs1_out == alu_rd_out) &
                                                                                                (alu_rd_out != 0));
                                                                                                
    assign memory_bubble = ldst_bubbule | regld_bubble;
    
    //输出值
    wire [63:0]  regfile_rs1valout;
    wire [63:0]  regfile_rs2valout;
    wire wb_rf_wen;
    wire [63:0] rf_write_val;
    
    regfile registerFile(
    .clk(r_clk),
    .wdata(rf_write_val),
    .waddr(wb_rd_out),
    .wen(wb_rf_wen),
    .r_addr1(regfile_rs1_out),
    .r_addr2(regfile_rs2_out),
    .r_out1(regfile_rs1valout),
    .r_out2(regfile_rs2valout)
    );
    
    /*********************RAW旁路***********************/
    // 旁路alu级的值
    reg regfile_alubypass_rs1 = 0;
    reg regfile_alubypass_rs2 = 0;

    assign regfile_alubypass_rs1 = (regfile_rs1_out == alu_rd_out &&
                                            regfile_rs1_out!=0) ? 1 :0;

    assign regfile_alubypass_rs2 = (regfile_rs2_out == alu_rd_out &&
                                            regfile_rs2_out!=0) ? 1 :0;
    
    
    // 旁路dmem级的值
    reg regfile_dmembypass_rs1 = 0;
    reg regfile_dmembypass_rs2 = 0;
    
    assign regfile_dmembypass_rs1 = (regfile_rs1_out == dmem_rd_out &&
                                                regfile_rs1_out!=0) ? 1 :0;

    assign regfile_dmembypass_rs2 = (regfile_rs2_out == dmem_rd_out &&
                                                regfile_rs2_out!=0) ? 1 :0;                                            
    
    //两路旁路
    wire [63:0] alubypass_rs1val;
    wire [63:0] alubypass_rs2val;
    
    wire [63:0] dmembypass_rs1val;
    wire [63:0] dmembypass_rs2val;
    
    wire [63:0] regfile_rs1val;
    wire [63:0] regfile_rs2val;
    
    assign alubypass_rs1val = regfile_alubypass_rs1 ? alu_result : regfile_rs1valout;
    assign alubypass_rs2val = regfile_alubypass_rs2 ? alu_result : regfile_rs2valout;
    
    assign dmembypass_rs1val = regfile_dmembypass_rs1 ? dmem_data_result : regfile_rs1valout;
    assign dmembypass_rs2val = regfile_dmembypass_rs2 ? dmem_data_result : regfile_rs2valout;
    
    assign regfile_rs1val = (regfile_alubypass_rs1) ?  alubypass_rs1val :
                                (regfile_dmembypass_rs1) ? dmembypass_rs1val :
                                                                regfile_rs1valout;
    
    assign regfile_rs2val = (regfile_alubypass_rs2) ?  alubypass_rs2val :
                                (regfile_dmembypass_rs2) ? dmembypass_rs2val :
                                                                regfile_rs2valout;
    
    /***************ALU(算数,分支和地址计算)**************************/
    // ALU阶段过程寄存器
    wire alu_rst;
    wire alu_wen;
    wire [63:0] alu_rs1val_out;
    wire [63:0] alu_rs2val_out;
    wire [4:0] alu_rd_out;
    wire [63:0] alu_pc_out;
    wire [63:0] alu_imm64_out;
    wire [5:0] alu_shamt_out;
    wire [6:0] alu_opcode_out;
    wire [6:0] alu_instrId_out;
    wire  alu_branch_out;
    wire  alu_regw_out;
    wire  alu_memr_out;
    wire  alu_memw_out;
    wire [63:0] alu_result;
    wire [4:0] alu_zimm_out;

    assign alu_rst = pc_rel_branch | pc_abs_branch | memory_bubble | regfile_exception >0 | wb_exception > 0;
    assign alu_wen = ~memory_bubble;
    Reg #(5,0)  alu_zimm (w_clk,alu_rst,regfile_rs2_out,alu_zimm_out,alu_wen);
    Reg #(64,0) alu_rs1val (w_clk,alu_rst,regfile_rs1val,alu_rs1val_out,alu_wen);
    Reg #(64,0) alu_rs2val (w_clk,alu_rst,regfile_rs2val,alu_rs2val_out,alu_wen);
    Reg #(5,0) alu_rd      (w_clk,alu_rst,regfile_rd_out,alu_rd_out,alu_wen);
    Reg #(64,0) alu_pc      (w_clk,alu_rst,regfile_pc_out,alu_pc_out,alu_wen);
    Reg #(64,0) alu_imm64 (w_clk,alu_rst,regfile_imm64_out,alu_imm64_out,alu_wen);
    Reg #(6,0) alu_shamt (w_clk,alu_rst,regfile_shamt_out,alu_shamt_out,alu_wen);
    Reg #(7,0) alu_opcode (w_clk,alu_rst,regfile_opcode_out,alu_opcode_out,alu_wen);
    Reg #(7,0) alu_instrId (w_clk,alu_rst,regfile_instrId_out,alu_instrId_out,alu_wen);
    Reg #(1,0) alu_branch (w_clk,alu_rst,regfile_branch_out,alu_branch_out,alu_wen);
    Reg #(1,0) alu_regw (w_clk,alu_rst,regfile_regw_out,alu_regw_out,alu_wen);
    Reg #(1,0) alu_memr (w_clk,alu_rst,regfile_memr_out,alu_memr_out,alu_wen);
    Reg #(1,0) alu_memw (w_clk,alu_rst,regfile_memw_out,alu_memw_out,alu_wen);
    // 异常
    wire [15:0] regfile_excep_out;
    wire [15:0] alu_exception;
    wire alu_exception_rst = pc_abs_branch | pc_rel_branch | wb_exception > 0;
    Reg #(16,0) alu_excep(w_clk,alu_exception_rst,regfile_exception,regfile_excep_out,alu_wen);
    assign alu_exception = 16'd0 | regfile_excep_out;
    
    alu alu64(
    .clk (r_clk),
    .instr_in (alu_instrId_out),
    .rs1 (alu_rs1val_out),
    .rs2 (alu_rs2val_out),
    .imm (alu_imm64_out),
    .shamt (alu_shamt_out),
    .pc (alu_pc_out),
    .result (alu_result)
    );

    //分支预测 branch 不会发生
    //此时是否分支应该也已经计算完毕
    assign pc_rel_branch = alu_branch_out & alu_result[0] & ~pc_abs_branch;
    assign pc_abs_branch = (alu_instrId_out == `i_jalr) | (alu_instrId_out == `i_mret);
    //绝大多数转移地址由立即数直接给出,jalr 为立即数与寄存器的和
    assign pc_imm_in = (alu_instrId_out == `i_jalr) ? {alu_result[63:1],1'b0} : 
                                    (alu_instrId_out == `i_mret) ? mepc_val + 64'd4 : 
                                                                            alu_imm64_out;

    assign pc_ref_pc = alu_pc_out;
    
    /**********************访问数据内存******************************/
    wire dmem_rst;
    wire dmem_wen;
    wire [4:0] dmem_rd_out;
    wire [63:0] dmem_pc_out;
    wire [63:0] dmem_result_out;
    wire [63:0] dmem_rs1val_out;
    wire [63:0] dmem_rs2val_out;
    wire [63:0] dmem_imm64_out;
    wire [6:0] dmem_instrId_out;
    wire [6:0] dmem_opcode_out;
    wire dmem_regw_out;
    wire dmem_memw_out;
    wire dmem_memr_out;
    wire dmem_branch_out;
    wire [4:0] dmem_zimm_out;

    assign dmem_rst = alu_exception > 0 | wb_exception >0;
    assign dmem_wen = 1;
    Reg #(5,0) dmem_zimm(w_clk,dmem_rst,alu_zimm_out,dmem_zimm_out,dmem_wen);
    Reg #(5,0) dmem_rd (w_clk,dmem_rst,alu_rd_out,dmem_rd_out,dmem_wen);
    Reg #(64,0) dmem_pc (w_clk,dmem_rst,alu_pc_out,dmem_pc_out,dmem_wen);
    Reg #(64,0) dmem_result (w_clk,dmem_rst,alu_result,dmem_result_out,dmem_wen);
    Reg #(64,0) dmem_rs1val (w_clk,dmem_rst,alu_rs1val_out,dmem_rs1val_out,dmem_wen);
    Reg #(64,0) dmem_rs2val (w_clk,dmem_rst,alu_rs2val_out,dmem_rs2val_out,dmem_wen);
    Reg #(64,0) dmem_imm64 (w_clk,dmem_rst,alu_imm64_out,dmem_imm64_out,dmem_wen);
    Reg #(7,0) dmem_instrId (w_clk,dmem_rst,alu_instrId_out,dmem_instrId_out,dmem_wen);
    Reg #(7,0) dmem_opcode (w_clk,dmem_rst,alu_opcode_out,dmem_opcode_out,dmem_wen);
    Reg #(1,0) dmem_regw (w_clk,dmem_rst,alu_regw_out,dmem_regw_out,dmem_wen);
    Reg #(1,0) dmem_memw (w_clk,dmem_rst,alu_memw_out,dmem_memw_out,dmem_wen);
    Reg #(1,0) dmem_memr (w_clk,dmem_rst,alu_memr_out,dmem_memr_out,dmem_wen);
    Reg #(1,0) dmem_branch (w_clk,dmem_rst,alu_branch_out,dmem_branch_out,dmem_wen);
    
    //异常
    wire [15:0] alu_excep_out;
    wire [15:0] dmem_exception;
    wire dmem_excep_rst = wb_exception > 0;
    Reg #(16,0) dmem_excep(w_clk,dmem_excep_rst,alu_exception,alu_excep_out,dmem_wen);
    assign dmem_exception = 0 | alu_excep_out;
    
    //访存
    import "DPI-C" function void npc_mem_write(
    input longint addr,input longint wdata,input byte wmask);
    
    reg [63:0] dmem_data_out;
    wire [63:0] dmem_sextdata_out;
    wire [63:0] dmem_data_result;

    // load instructions
    assign dmem_data_result = (dmem_opcode_out == 7'b0000011) ? dmem_sextdata_out : dmem_result_out;

    assign dmem_sextdata_out = (dmem_instrId_out == `i_lb) ? {{56{dmem_data_out[7]}},dmem_data_out[7:0]} :
                                (dmem_instrId_out == `i_lh) ?  {{48{dmem_data_out[15]}},dmem_data_out[15:0]} :
                                (dmem_instrId_out == `i_lw) ? {{32{dmem_data_out[31]}},dmem_data_out[31:0]} :
                                (dmem_instrId_out == `i_ld ) ? dmem_data_out :
                                (dmem_instrId_out == `i_lbu) ? {{56{1'b0}},dmem_data_out[7:0]} :
                                (dmem_instrId_out == `i_lhu) ? {{48{1'b0}},dmem_data_out[15:0]} :
                                (dmem_instrId_out == `i_lwu) ? {{32{1'b0}},dmem_data_out[31:0]} : 64'hdeadbeef;

    always @(posedge r_clk) begin
        if (dmem_memr_out) begin
            npc_mem_read(dmem_result_out,dmem_data_out);
        end
    end

    wire [63:0] csrval;
    wire [63:0] mepc_overri;
    wire [63:0] mcause_overri;
    wire [63:0] mtvec_val;
    wire excep_wen;
    wire csr_wen = dmem_instrId_out == `i_csrrw || dmem_instrId_out == `i_csrrs || dmem_instrId_out == `i_csrrc || dmem_instrId_out == `i_csrrwi || dmem_instrId_out == `i_csrrsi || dmem_instrId_out == `i_csrrci;
    
    // 如果是csr 指令，则在此读csr
    CSR csr(
        .clk(r_clk),
        .rst(rst),
        .wen(csr_wen),
        .excep_wen(excep_wen),
        .rs1(dmem_rs1val_out),
        .addr(dmem_imm64_out[11:0]),
        .instr(dmem_instrId_out),
        .imm(dmem_zimm_out),
        .mepc_val(mepc_val),
        .mtvec_val(mtvec_val),
        .mcause_overri(mcause_overri),
        .mepc_overri(mepc_overri),
        .csrVal(csrval)
    );

    /********************回写****************************/
    wire wb_rst;
    wire wb_wen;
    wire [4:0] wb_rd_out;
    wire [63:0] wb_pc_out;
    wire [63:0] wb_result_out;
    wire [6:0] wb_instrId_out;
    wire [63:0] wb_memdata_out;
    wire [63:0] wb_rs1val_out;
    wire [63:0] wb_rs2val_out;
    wire [6:0] wb_opcode_out;
    wire [63:0] wb_csrval_out;
    wire wb_regw_out;
    wire wb_memw_out;
    wire wb_branch_out;
    
    assign wb_rst = dmem_exception > 0 | wb_exception > 0;
    assign wb_wen = 1;
    Reg #(64,0) wb_csrval (w_clk,wb_rst,csrval,wb_csrval_out,wb_wen);
    Reg #(5,0) wb_rd (w_clk,wb_rst,dmem_rd_out,wb_rd_out,wb_wen);
    Reg #(64,0) wb_pc (w_clk,wb_rst,dmem_pc_out,wb_pc_out,wb_wen);
    Reg #(64,0) wb_result (w_clk,wb_rst,dmem_result_out,wb_result_out,wb_wen);
    Reg #(7,0) wb_instrId (w_clk,wb_rst,dmem_instrId_out,wb_instrId_out,wb_wen);
    Reg #(64,0) wb_memdata (w_clk,wb_rst,dmem_sextdata_out,wb_memdata_out,wb_wen);
    Reg #(64,0) wb_rs1val (w_clk,wb_rst,dmem_rs1val_out,wb_rs1val_out,wb_wen);
    Reg #(64,0) wb_rs2val (w_clk,wb_rst,dmem_rs2val_out,wb_rs2val_out,wb_wen);
    Reg #(1,0) wb_regw (w_clk,wb_rst,dmem_regw_out,wb_regw_out,wb_wen);
    Reg #(1,0) wb_memw (w_clk,wb_rst,dmem_memw_out,wb_memw_out,wb_wen);
    Reg #(1,0) wb_branch (w_clk,wb_rst,dmem_branch_out,wb_branch_out,wb_wen);
    Reg #(7,0) wb_opcode (w_clk,wb_rst,dmem_opcode_out,wb_opcode_out,wb_wen);
    //异常
    wire [15:0] dmem_excep_out;
    wire [15:0] wb_exception;
    Reg #(16,0) wb_excep(w_clk,0,dmem_exception,dmem_excep_out,wb_wen);
    assign wb_exception = wb_instrId_out == `i_ecall ? `e_ecall | dmem_excep_out : 0 | dmem_exception;
    
    wire [7:0] wmask;
    assign wmask = (wb_instrId_out == `i_sb) ? 1 :
                        (wb_instrId_out == `i_sh) ? 2 :
                            (wb_instrId_out == `i_sw) ? 4 : 
                                (wb_instrId_out == `i_sd) ? 8 : 0;
    
    always @(posedge r_clk) begin
        if (wb_memw_out) begin
            npc_mem_write(wb_result_out,wb_rs2val_out,wmask);
        end
    end

    assign pc_exception = wb_exception > 0;
    assign excep_wen = wb_exception > 0;
    assign mepc_overri = wb_pc_out;
    assign mcause_overri = wb_instrId_out == `i_ecall ? 64'd11 : 64'd0;
    
    // 0号寄存器不能被写
    assign wb_rf_wen = wb_regw_out & (wb_rd_out!= 0) & (wb_opcode_out != 7'b1100011);

    assign rf_write_val = (wb_opcode_out == 7'b0000011) ? wb_memdata_out : 
                                (wb_opcode_out == 7'b1110011) ? wb_csrval_out :
                                        (wb_instrId_out == `i_jal) ? wb_pc_out + 4 :
                                                (wb_instrId_out == `i_jalr) ? wb_pc_out + 4 :
                                                                                    wb_result_out;

endmodule
    
    

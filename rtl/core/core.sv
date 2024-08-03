`include "instructions.v"
`include "exceptions.v"

module core # (
    parameter RW_DATA_WIDTH     = 64,
    parameter RW_ADDR_WIDTH     = 64,
    parameter AXI_DATA_WIDTH    = 64,
    parameter AXI_ADDR_WIDTH    = 64,
    parameter AXI_ID_WIDTH      = 4,
    parameter AXI_STRB_WIDTH    = AXI_DATA_WIDTH/8,
    parameter AXI_USER_WIDTH    = 1
)(
	// input clk
	input clk,
	// reset
	input rst,
	// Advanced eXtensible Interfaces
	input                               axi_aw_ready,
	output                              axi_aw_valid,
	output [AXI_ADDR_WIDTH-1:0]         axi_aw_addr,
	output [2:0]                        axi_aw_prot,
	output [AXI_ID_WIDTH-1:0]           axi_aw_id,
	output [AXI_USER_WIDTH-1:0]         axi_aw_user,
	output [7:0]                        axi_aw_len,
	output [2:0]                        axi_aw_size,
	output [1:0]                        axi_aw_burst,
	output                              axi_aw_lock,
	output [3:0]                        axi_aw_cache,
	output [3:0]                        axi_aw_qos,
	output [3:0]                        axi_aw_region,

	input                               axi_w_ready,
	output                              axi_w_valid,
	output [AXI_DATA_WIDTH-1:0]         axi_w_data,
	output [AXI_DATA_WIDTH/8-1:0]       axi_w_strb,
	output                              axi_w_last,
	output [AXI_USER_WIDTH-1:0]         axi_w_user,

	output                              axi_b_ready,
	input                               axi_b_valid,
	/* verilator lint_off UNUSED */
	input  [1:0]                        axi_b_resp,
	/* verilator lint_off UNUSED */
	input  [AXI_ID_WIDTH-1:0]           axi_b_id,
	input  [AXI_USER_WIDTH-1:0]         axi_b_user,

	input                               axi_ar_ready,
	output                              axi_ar_valid,
	output [AXI_ADDR_WIDTH-1:0]         axi_ar_addr,
	output [2:0]                        axi_ar_prot,
	output [AXI_ID_WIDTH-1:0]           axi_ar_id,
	output [AXI_USER_WIDTH-1:0]         axi_ar_user,
	output [7:0]                        axi_ar_len,
	output [2:0]                        axi_ar_size,
	output [1:0]                        axi_ar_burst,
	output                              axi_ar_lock,
	output [3:0]                        axi_ar_cache,
	output [3:0]                        axi_ar_qos,
	output [3:0]                        axi_ar_region,

	output                              axi_r_ready,
	input                               axi_r_valid,
	input  [1:0]                        axi_r_resp,
	input  [AXI_DATA_WIDTH-1:0]         axi_r_data,
	input                               axi_r_last,
	input  [AXI_ID_WIDTH-1:0]           axi_r_id,
	input  [AXI_USER_WIDTH-1:0]         axi_r_user
);


    wire axi_fifo_wen;
    wire [8:0] axi_fifo_idx;
    wire axi_fifo_done;
    wire [63:0] axi_fifo_data_i;
    /* verilator lint_off UNUSED */
    wire [63:0] data_out1;
    wire trans_done1_o;
    /* verilator lint_off UNUSED*/

    /* verilator lint_off UNUSED */
    axi_ctl ac0(
      .clk(clk),
      .rst(rst),

      .axi_req1(dmem_axi_req),
      .rw_req1(dmem_axi_rw),
      .addr1(dmem_axi_addr),
      .data1(dmem_axi_data_o),
      .rw_len1(dmem_axi_len),
      .data_o1(dmem_axi_data_i),
      .axi_done1(dmem_axi_done),
      
      .axi_req2(if_axi_req),
      .rw_req2(if_axi_rw),
      .addr2(if_axi_addr),
      .data2(64'd0),
      .rw_len2(if_axi_len),
      .data_o2(if_axi_data),
      .axi_done2(if_axi_done),

      .cache_fifo_idx(axi_fifo_idx),
      .cache_fifo_data_i(axi_fifo_data_i),
      .cache_fifo_done(axi_fifo_done),
      .cache_fifo_wen(axi_fifo_wen),

      .axi_aw_ready (axi_aw_ready ),
      .axi_aw_valid (axi_aw_valid ),
      .axi_aw_addr  (axi_aw_addr  ),
      .axi_aw_prot  (axi_aw_prot  ),
      .axi_aw_id    (axi_aw_id    ),
      .axi_aw_user  (axi_aw_user  ),
      .axi_aw_len   (axi_aw_len   ),
      .axi_aw_size  (axi_aw_size  ),
      .axi_aw_burst (axi_aw_burst ),
      .axi_aw_lock  (axi_aw_lock  ),
      .axi_aw_cache (axi_aw_cache ),
      .axi_aw_qos   (axi_aw_qos   ),
      .axi_aw_region(axi_aw_region),

      .axi_w_ready  (axi_w_ready  ),
      .axi_w_valid  (axi_w_valid  ),
      .axi_w_data   (axi_w_data   ),
      .axi_w_strb   (axi_w_strb   ),
      .axi_w_last   (axi_w_last   ),
      .axi_w_user   (axi_w_user   ),

      .axi_b_ready  (axi_b_ready  ),
      .axi_b_valid  (axi_b_valid  ),
      .axi_b_resp   (axi_b_resp   ),
      .axi_b_id     (axi_b_id     ),
      .axi_b_user   (axi_b_user   ),

      .axi_ar_ready (axi_ar_ready ),
      .axi_ar_valid (axi_ar_valid ),
      .axi_ar_addr  (axi_ar_addr  ),
      .axi_ar_prot  (axi_ar_prot  ),
      .axi_ar_id    (axi_ar_id    ),
      .axi_ar_user  (axi_ar_user  ),
      .axi_ar_len   (axi_ar_len   ),
      .axi_ar_size  (axi_ar_size  ),
      .axi_ar_burst (axi_ar_burst ),
      .axi_ar_lock  (axi_ar_lock  ),
      .axi_ar_cache (axi_ar_cache ),
      .axi_ar_qos   (axi_ar_qos   ),
      .axi_ar_region(axi_ar_region),

      .axi_r_ready  (axi_r_ready  ),
      .axi_r_valid  (axi_r_valid  ),
      .axi_r_resp   (axi_r_resp   ),
      .axi_r_data   (axi_r_data   ),
      .axi_r_last   (axi_r_last   ),
      .axi_r_id     (axi_r_id     ),
      .axi_r_user   (axi_r_user   )
    );

    wire pc_rel_branch;
    wire pc_abs_branch;
    wire pc_exception;
    wire [63:0] pc_out;
    wire [63:0] branch_base_pc;
    wire [63:0] pc_imm_in;
    wire [63:0] mepc_val;
    wire pc_bubble;
    assign pc_bubble = memory_bubble | ~ic_valid | dcache_hold;

    wire if_axi_req;
    wire if_axi_done;
    wire ic_valid;
    wire if_axi_rw = 0;
    wire [7:0] if_axi_len = 8'd8;
    wire [31:0] ic_data;
    wire [63:0] if_axi_data;
    wire [63:0] instr_fetched;
    wire [63:0] if_axi_addr;

    wire [15:0] fetch_exception = 0;
    wire [31:0] decode_instr;
    wire [63:0] decode_pc;
    wire [15:0] exception_to_decode;
    wire decode_rst = pc_rel_branch | pc_abs_branch | fetch_exception > 0 | wb_exception > 0;
    wire decode_wen = ~memory_bubble & ~dcache_hold;
    wire [15:0] decode_exception = exception_to_decode;
    wire decode_excep_rst = pc_abs_branch | pc_rel_branch | wb_exception >0;

    wire [4:0] rs1;
    wire [4:0] rs2;
    wire [4:0] rd;
    wire [63:0] imm64;
    wire [5:0] shamt;
    wire [6:0] instr_id;
    wire branch;
    wire reg_w;
    wire mem_r;
    wire mem_w;
    wire [6:0] opcode;

    wire rf_rst;
    wire rf_enable;
    wire [4:0] rf_rs1;
    wire [4:0] rf_rs2;
    wire [4:0] rf_rd;
    wire [63:0] rf_pc;
    wire [63:0] rf_imm64;
    wire [5:0] rf_shamt;
    wire [6:0] rf_opcode;
    wire [6:0] rf_instrId;
    wire rf_branch;
    wire rf_regw;
    wire rf_memr;
    wire rf_memw;

    pc pc64 (
      .clk (clk),
      .rst (rst),
      .bubble(pc_bubble),
      .exception(pc_exception),
      .rel_branch (pc_rel_branch),
      .abs_branch (pc_abs_branch),
      .immediate (pc_imm_in),
      .mtvec(mtvec_val),
      .branch_base(branch_base_pc),
      .pc_out_reg(pc_out)
    );

    icache #(.WAY_NUMBER(8)) ic0(
      .clk(clk),
      .rst(rst),

      .core_addr_i(pc_out),
      .valid_o(ic_valid),
      .data_o(ic_data),

      .axi_done(if_axi_done),
      .axi_req(if_axi_req),
      .axi_data_i(if_axi_data),
      .axi_req_addr(if_axi_addr),
      .axi_fifo_idx(axi_fifo_idx),
      .axi_fifo_done(axi_fifo_done)
    );

    reg_fetch_decode reg_fd(
      .clk(clk),
      .rst(decode_rst),
      .enable(decode_wen),
      .pc_from_fetch(pc_out),
      .instr_from_fetch(ic_data),
      .exception_from_fetch(fetch_exception),
      .pc_to_decode(decode_pc),
      .instr_to_decode(decode_instr),
      .exception_to_decode(exception_to_decode)
    );

    decoder dec(
		.clk (clk),
		.instr (decode_instr),
		.rs1 (rs1),
		.rs2 (rs2),
		.rd (rd),
		.branch(branch),
		.reg_w(reg_w),
		.mem_r(mem_r),
		.mem_w(mem_w),
		.imm(imm64),
		.shamt (shamt),
		.opcode (opcode),
		.instr_id (instr_id)
    );

    wire regfile_rst = pc_rel_branch | pc_abs_branch | decode_exception > 0 | wb_exception > 0;
    wire regfile_wen = ~memory_bubble & ~dcache_hold;

    wire [15:0] decode_excep_out;
    wire [15:0] regfile_exception;
    wire regfile_excep_rst = pc_rel_branch | pc_abs_branch | wb_exception >0;
    Reg #(16,0) regfile_excep(clk,regfile_excep_rst,decode_excep_out,decode_excep_out,regfile_wen);
    assign regfile_exception = 0 | decode_excep_out;

    wire ldst_bubbule;
    wire memory_bubble;

    assign ldst_bubbule = ((opcode == 7'b0000011) &                 //load
                           (alu_opcode_out == 7'b0100011)) ? 1 : 0; // store

    assign memory_bubble = ldst_bubbule;

    //输出值
    wire [63:0]  regfile_rs1valout;
    wire [63:0]  regfile_rs2valout;
    wire wb_rf_wen;
    wire [63:0] rf_write_val;

    regfile registerFile(
      .clk(clk),
      .wdata(rf_write_val),
      .waddr(wb_rd_out),
      .wen(wb_rf_wen),
      .r_addr1(rs1),
      .r_addr2(rs2),
      .r_out1(regfile_rs1valout),
      .r_out2(regfile_rs2valout)
    );

    /*********************RAW旁路***********************/
    // 旁路alu级的值
    reg regfile_alubypass_rs1 = 0;
    reg regfile_alubypass_rs2 = 0;

    assign regfile_alubypass_rs1 = (rs1 == alu_rd_out &&
                                    rs1 != 0) ? 1 :0;

    assign regfile_alubypass_rs2 = (rs2 == alu_rd_out &&
                                    rs2 != 0) ? 1 :0;


    // 旁路dmem级的值
    reg regfile_dmembypass_rs1 = 0;
    reg regfile_dmembypass_rs2 = 0;

    assign regfile_dmembypass_rs1 = (rs1 == dmem_rd_out &&
                                     rs1 != 0) ? 1 :0;

    assign regfile_dmembypass_rs2 = (rs2 == dmem_rd_out &&
                                     rs2 != 0) ? 1 :0;

    //合并两路旁路
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

    /***************ALU(算数,分支和地址 计算)**************************/
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

    assign alu_rst = pc_rel_branch | pc_abs_branch | (memory_bubble & ~dcache_hold) | regfile_exception >0 | wb_exception > 0;
    assign alu_wen = ~memory_bubble & ~dcache_hold;
    Reg #(5,0)  alu_zimm (clk, alu_rst, rs2, alu_zimm_out, alu_wen);
    Reg #(64,0) alu_rs1val (clk, alu_rst, regfile_rs1val, alu_rs1val_out, alu_wen);
    Reg #(64,0) alu_rs2val (clk, alu_rst, regfile_rs2val, alu_rs2val_out, alu_wen);
    Reg #(5,0) alu_rd      (clk, alu_rst, rd, alu_rd_out, alu_wen);
    Reg #(64,0) alu_pc      (clk, alu_rst, decode_pc, alu_pc_out, alu_wen);
    Reg #(64,0) alu_imm64 (clk, alu_rst, imm64, alu_imm64_out, alu_wen);
    Reg #(6,0) alu_shamt (clk, alu_rst, shamt, alu_shamt_out, alu_wen);
    Reg #(7,0) alu_opcode (clk, alu_rst, opcode, alu_opcode_out, alu_wen);
    Reg #(7,0) alu_instrId (clk, alu_rst, instr_id, alu_instrId_out, alu_wen);
    Reg #(1,0) alu_branch (clk, alu_rst, branch, alu_branch_out, alu_wen);
    Reg #(1,0) alu_regw (clk, alu_rst, reg_w, alu_regw_out, alu_wen);
    Reg #(1,0) alu_memr (clk, alu_rst, mem_r, alu_memr_out, alu_wen);
    Reg #(1,0) alu_memw (clk, alu_rst, mem_w, alu_memw_out, alu_wen);
    // 异常
    wire [15:0] regfile_excep_out;
    wire [15:0] alu_exception;
    wire alu_exception_rst = pc_abs_branch | pc_rel_branch | wb_exception > 0;
    Reg #(16,0) alu_excep(clk,alu_exception_rst,regfile_exception,regfile_excep_out,alu_wen);
    assign alu_exception = 16'd0 | regfile_excep_out;

    alu alu64(
		.clk (clk),
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

    assign branch_base_pc = alu_pc_out;

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
    assign dmem_wen = ~dcache_hold;
    Reg #(5,0) dmem_zimm(clk,dmem_rst,alu_zimm_out,dmem_zimm_out,dmem_wen);
    Reg #(5,0) dmem_rd (clk,dmem_rst,alu_rd_out,dmem_rd_out,dmem_wen);
    Reg #(64,0) dmem_pc (clk,dmem_rst,alu_pc_out,dmem_pc_out,dmem_wen);
    Reg #(64,0) dmem_result (clk,dmem_rst,alu_result,dmem_result_out,dmem_wen);
    Reg #(64,0) dmem_rs1val (clk,dmem_rst,alu_rs1val_out,dmem_rs1val_out,dmem_wen);
    Reg #(64,0) dmem_rs2val (clk,dmem_rst,alu_rs2val_out,dmem_rs2val_out,dmem_wen);
    Reg #(64,0) dmem_imm64 (clk,dmem_rst,alu_imm64_out,dmem_imm64_out,dmem_wen);
    Reg #(7,0) dmem_instrId (clk,dmem_rst,alu_instrId_out,dmem_instrId_out,dmem_wen);
    Reg #(7,0) dmem_opcode (clk,dmem_rst,alu_opcode_out,dmem_opcode_out,dmem_wen);
    Reg #(1,0) dmem_regw (clk,dmem_rst,alu_regw_out,dmem_regw_out,dmem_wen);
    Reg #(1,0) dmem_memw (clk,dmem_rst,alu_memw_out,dmem_memw_out,dmem_wen);
    Reg #(1,0) dmem_memr (clk,dmem_rst,alu_memr_out,dmem_memr_out,dmem_wen);
    Reg #(1,0) dmem_branch (clk,dmem_rst,alu_branch_out,dmem_branch_out,dmem_wen);

    //异常
    wire [15:0] alu_excep_out;
    wire [15:0] dmem_exception;
    wire dmem_excep_rst = wb_exception > 0;
    Reg #(16,0) dmem_excep(clk,dmem_excep_rst,alu_exception,alu_excep_out,dmem_wen);
    assign dmem_exception = 0 | alu_excep_out;


    /* verilator lint_off UNDRIVEN */
    reg [63:0] dmem_data_out;
    wire [63:0] dmem_sextdata_out;
    wire [63:0] dmem_data_result;
    /* verilator lint_off UNDRIVEN */


    // data cache ,
    // write to data cache will only be valid in the next cycle ,
    // so that , if dmem and wb stage is commiting an instruction at the same time , dmem will be a cycle slower ,
    // which means theses two instructions commit one by one
    wire [63:0]  dmem_axi_data_o;
    wire [63:0]  dmem_axi_data_i;
    wire dmem_axi_done;
    wire dmem_axi_req;
    wire dmem_axi_rw;
    wire [63:0] dmem_axi_addr;
    wire [7:0] dmem_axi_len;
    wire [63:0] dmem_data_out;
    wire dmem_r_valid;
    wire dmem_w_valid;
    wire dmem_cache_rw;
    wire dmem_cache_req;
    wire [63:0] dmem_rw_addr;
    wire [63:0] dmem_rw_data;
    wire mmio_rw;
    wire [63:0] mmio_addr;
    wire [63:0] mmio_data_o;
    wire [3:0] mmio_burst_len;

    assign dmem_axi_len = 8'd8;
    assign dmem_cache_req = dmem_memr_out | dmem_memw_out;
    assign dmem_cache_rw = dmem_memw_out;
    assign dmem_rw_addr = dmem_result_out;
    assign dmem_rw_data = dmem_rs2val_out;

    wire dcache_hold = (dmem_memr_out & ~dmem_r_valid) | (dmem_memw_out & ~dmem_w_valid);

    // how many bytes should be written ?
    wire [3:0] wmask;
    assign wmask = (dmem_instrId_out == `i_sb) ? 1 :
           (dmem_instrId_out == `i_sh) ? 2 :
           (dmem_instrId_out == `i_sw) ? 4 :
           (dmem_instrId_out == `i_sd) ? 8 : 0;
    
    assign mmio_rw = dmem_cache_req & dmem_rw_addr > 64'h0000000010000000 & dmem_rw_addr < 64'h000000001fffffff; 
    assign mmio_addr = dmem_rw_addr;
    assign mmio_burst_len = 0;

    dcache #(.WAY_NUMBER(8)) dc0
		(.clk(clk),
		.rst(rst),
		// core interfaces
		.dcache_req(dmem_cache_req),
		.cache_rw(dmem_cache_rw),
		.core_addr_i(dmem_result_out),
		.core_data_i(dmem_rs2val_out), // riscv only writes to ram with the data in rs2
		.write_mask(wmask),
		.w_valid_o(dmem_w_valid),
		.r_valid_o(dmem_r_valid),
		.data_o(dmem_data_out),
		// axi interfaces
		.axi_data_i(dmem_axi_data_i),
		.axi_done(dmem_axi_done),
		.axi_req(dmem_axi_req),
		.axi_rw(dmem_axi_rw),
		.axi_req_addr(dmem_axi_addr),
		.axi_fifo_data_o(axi_fifo_data_i),
		.axi_fifo_idx(axi_fifo_idx),
		.axi_fifo_wen(axi_fifo_wen),
		.axi_fifo_done(axi_fifo_done)
    );

    // solve width and signedness of the read data
    assign dmem_sextdata_out = (dmem_instrId_out == `i_lb) ? {{56{dmem_data_out[7]}},dmem_data_out[7:0]} :
           (dmem_instrId_out == `i_lh) ?  {{48{dmem_data_out[15]}},dmem_data_out[15:0]} :
           (dmem_instrId_out == `i_lw) ? {{32{dmem_data_out[31]}},dmem_data_out[31:0]} :
           (dmem_instrId_out == `i_ld ) ? dmem_data_out :
           (dmem_instrId_out == `i_lbu) ? {{56{1'b0}},dmem_data_out[7:0]} :
           (dmem_instrId_out == `i_lhu) ? {{48{1'b0}},dmem_data_out[15:0]} :
           (dmem_instrId_out == `i_lwu) ? {{32{1'b0}},dmem_data_out[31:0]} : 64'hdeadbeef;

    // the data passed to next stage
    // if the instruction is a load instruction , then pass the read data to next stage
    // other wise pass the result of the alu
    assign dmem_data_result = (dmem_opcode_out == 7'b0000011) ? dmem_sextdata_out : dmem_result_out;

    // CSR instructions
    // since CSR never reads memory, we can simply put csr instrutions to be executed at mem stage
    wire [63:0] csrval;
    wire [63:0] mepc_overri;
    wire [63:0] mcause_overri;
    wire [63:0] mtvec_val;
    wire excep_wen;
    wire csr_wen = dmem_instrId_out == `i_csrrw || dmem_instrId_out == `i_csrrs ||
         dmem_instrId_out == `i_csrrc || dmem_instrId_out == `i_csrrwi ||
         dmem_instrId_out == `i_csrrsi || dmem_instrId_out == `i_csrrci;

    CSR csr(
		.clk(clk),
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
    assign wb_rst = dcache_hold | dmem_exception > 0 | wb_exception > 0;
    assign wb_wen = ~dcache_hold;

    Reg #(64,0) wb_csrval (clk,wb_rst,csrval,wb_csrval_out,wb_wen);
    Reg #(5,0) wb_rd (clk,wb_rst,dmem_rd_out,wb_rd_out,wb_wen);
    Reg #(64,0) wb_pc (clk,wb_rst,dmem_pc_out,wb_pc_out,wb_wen);
    Reg #(64,0) wb_result (clk,wb_rst,dmem_result_out,wb_result_out,wb_wen);
    Reg #(7,0) wb_instrId (clk,wb_rst,dmem_instrId_out,wb_instrId_out,wb_wen);
    Reg #(64,0) wb_memdata (clk,wb_rst,dmem_sextdata_out,wb_memdata_out,wb_wen);
    Reg #(64,0) wb_rs1val (clk,wb_rst,dmem_rs1val_out,wb_rs1val_out,wb_wen);
    Reg #(64,0) wb_rs2val (clk,wb_rst,dmem_rs2val_out,wb_rs2val_out,wb_wen);
    Reg #(1,0) wb_regw (clk,wb_rst,dmem_regw_out,wb_regw_out,wb_wen);
    Reg #(1,0) wb_memw (clk,wb_rst,dmem_memw_out,wb_memw_out,wb_wen);
    Reg #(1,0) wb_branch (clk,wb_rst,dmem_branch_out,wb_branch_out,wb_wen);
    Reg #(7,0) wb_opcode (clk,wb_rst,dmem_opcode_out,wb_opcode_out,wb_wen);
    //异常
    wire [15:0] dmem_excep_out;
    wire [15:0] wb_exception;
    Reg #(16,0) wb_excep(clk,0,dmem_exception,dmem_excep_out,wb_wen);
    assign wb_exception = wb_instrId_out == `i_ecall ? `e_ecall | dmem_excep_out : 0 | dmem_exception;
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
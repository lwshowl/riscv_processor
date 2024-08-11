`include "instructions.sv"
`include "exceptions.sv"

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

  wire pc_rel_branch;
  wire pc_abs_branch;
  wire pc_exception = 1'b0;
  wire [63:0] pc;
  wire [63:0] branch_base_pc;
  wire [63:0] pc_imm_in;
  wire [63:0] mepc_val;
  wire pc_hold;
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

  wire decode_rst;
  wire [15:0] decode_exception;
  wire decode_wen;
  wire regfile_rst;
  wire regfile_wen;
  wire memory_bubble;
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
  wire [63:0]  regfile_rs1val;
  wire [63:0]  regfile_rs2val;
  wire wb_rf_wen;
  wire [63:0] rf_write_val;
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
  wire alu_rst;
  wire alu_wen;
  wire [63:0] alu_rs1val;
  wire [63:0] alu_rs2val;
  wire [4:0] alu_rs1;
  wire [4:0] alu_rs2;
  wire [4:0] alu_rd;
  wire [63:0] alu_pc;
  wire [63:0] alu_imm64;
  wire [5:0] alu_shamt;
  wire [6:0] alu_opcode;
  wire [6:0] alu_instrId;
  wire  alu_branch;
  wire  alu_regw;
  wire  alu_memr;
  wire  alu_memw;
  wire [63:0] alu_result;
  wire [4:0] alu_zimm;
  wire dmem_rst;
  wire dmem_wen;
  wire [4:0] dmem_rd;
  wire [63:0] dmem_pc;
  wire [63:0] dmem_result;
  wire [63:0] dmem_rs1val;
  wire [63:0] dmem_rs2val;
  wire [63:0] dmem_imm64;
  wire [6:0] dmem_instrId;
  wire [6:0] dmem_opcode;
  wire dmem_regw;
  wire dmem_memw;
  wire dmem_memr;
  wire dmem_branch;
  wire [4:0] dmem_zimm;
  wire [63:0] dmem_sextdata;
  wire [63:0]  dmem_axi_data_o;
  wire [63:0]  dmem_axi_data_i;
  wire dmem_axi_done;
  wire dmem_axi_req;
  wire dmem_axi_rw;
  wire [63:0] dmem_axi_addr;
  wire [7:0] dmem_axi_len;
  wire [63:0] dmem_data;
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
  wire wb_rst;
  wire wb_wen;
  wire [4:0] wb_rd;
  wire [63:0] wb_pc;
  wire [63:0] wb_result;
  wire [6:0] wb_instrId;
  wire [63:0] wb_memdata;
  wire [63:0] wb_rs1val;
  wire [63:0] wb_rs2val;
  wire [6:0] wb_opcode;
  wire [63:0] wb_csrval;
  wire wb_regw;
  wire wb_memw;
  wire wb_memr;
  wire wb_branch;
  wire dcache_hold;
  wire [3:0] wmask;
  wire [63:0] csrval;
  wire [63:0] mepc_overri;
  wire [63:0] mcause_overri;
  wire [63:0] mtvec_val;
  wire csr_wen;

  wire rs1_alu_bypass;
  wire rs2_alu_bypss;
  wire rs1_dmem_bypass;
  wire rs2_dmem_bypass;
  wire [63:0] wb_bypass_data;
  wire [63:0] dmem_bypass_data;

  wire [63:0] rs1val;
  wire [63:0] rs2val;

  wire regld_bubble;

  assign dmem_axi_len = 8'd8;
  assign pc_hold =  regld_bubble | ~ic_valid | dcache_hold;
  assign alu_rst = pc_rel_branch | pc_abs_branch;
  assign alu_wen = ~dcache_hold;
  assign dmem_cache_req = dmem_memr | dmem_memw;
  assign dmem_cache_rw = dmem_memw;
  assign dmem_rw_addr = dmem_result;
  assign dmem_rw_data = dmem_rs2val;
  assign decode_rst = pc_rel_branch | pc_abs_branch | regld_bubble;
  assign decode_wen = ~dcache_hold;
  assign regfile_wen = ~dcache_hold;
  assign decode_exception = exception_to_decode;
  assign regfile_rst = pc_rel_branch | pc_abs_branch;

  assign regld_bubble = ((reg_w == 1 | mem_w == 1) & // reg
                        (alu_opcode == 7'b0000011) & //load
                        ((rs2 == alu_rd) | (rs1 == alu_rd)) &
                        (alu_rd != 0));

  assign dcache_hold = (dmem_memr & ~dmem_r_valid) | (dmem_memw & ~dmem_w_valid);
  assign wmask = (dmem_instrId == `i_sb) ? 1 :
                (dmem_instrId == `i_sh) ? 2 :
                (dmem_instrId == `i_sw) ? 4 :
                (dmem_instrId == `i_sd) ? 8 : 0;
                
  assign mmio_rw = dmem_cache_req & dmem_rw_addr > 64'h0000000010000000 & dmem_rw_addr < 64'h000000001fffffff;
  assign mmio_addr = dmem_rw_addr;
  assign mmio_burst_len = 0;
  assign dmem_rst = 1'b0;
  assign dmem_wen = ~dcache_hold;
  assign wb_rst = dcache_hold;
  assign wb_wen = ~dcache_hold;
  assign csr_wen = dmem_instrId == `i_csrrw || dmem_instrId == `i_csrrs ||
          dmem_instrId == `i_csrrc || dmem_instrId == `i_csrrwi ||
          dmem_instrId == `i_csrrsi || dmem_instrId == `i_csrrci;

  assign mepc_overri = wb_pc;
  assign mcause_overri = wb_instrId == `i_ecall ? 64'd11 : 64'd0;
  assign wb_rf_wen = wb_regw & (wb_rd!= 0) & (wb_opcode != 7'b1100011);
  assign rf_write_val = (wb_opcode == 7'b0000011) ? wb_memdata :
          (wb_opcode == 7'b1110011) ? wb_csrval :
          (wb_instrId == `i_jal) ? wb_pc + 4 :
          (wb_instrId == `i_jalr) ? wb_pc + 4 :
          wb_result;

  assign wb_bypass_data = wb_memr ? wb_memdata : wb_result;
  assign dmem_bypass_data = dmem_memr ? dmem_data : dmem_result;

  assign dmem_sextdata = (dmem_instrId == `i_lb) ? {{56{dmem_data[7]}}, dmem_data[7:0]} :
          (dmem_instrId == `i_lh) ?  {{48{dmem_data[15]}}, dmem_data[15:0]} :
          (dmem_instrId == `i_lw) ? {{32{dmem_data[31]}}, dmem_data[31:0]} :
          (dmem_instrId == `i_ld ) ? dmem_data :
          (dmem_instrId == `i_lbu) ? {{56{1'b0}},dmem_data[7:0]} :
          (dmem_instrId == `i_lhu) ? {{48{1'b0}},dmem_data[15:0]} :
          (dmem_instrId == `i_lwu) ? {{32{1'b0}},dmem_data[31:0]} : 64'hdeadbeef;

  axi_ctl ac0(
    .clk(clk),
    .rst(rst),
    .axi_req1(dmem_axi_req),
    .rw_req1(dmem_axi_rw),
    .addr1(dmem_axi_addr),
    .data1(0),
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

  pc pc64 (
    .clk (clk),
    .rst (rst),
    .hold(pc_hold),
    .exception(pc_exception),
    .rel_branch (pc_rel_branch),
    .abs_branch (pc_abs_branch),
    .immediate (pc_imm_in),
    .mtvec(mtvec_val),
    .branch_base(branch_base_pc),
    .pc_out_reg(pc)
  );

  icache #(.WAY_NUMBER(8)) ic0(
    .clk(clk),
    .rst(rst),

    .core_addr_i(pc),
    .valid_o(ic_valid),
    .data_o(ic_data),

    .axi_done(if_axi_done),
    .axi_req(if_axi_req),
    .axi_data_i(if_axi_data),
    .axi_req_addr(if_axi_addr),
    .axi_fifo_idx(axi_fifo_idx),
    .axi_fifo_done(axi_fifo_done)
  );

  reg_fetch_dec reg_fd(
    .clk(clk),
    .rst(decode_rst),
    .enable(decode_wen),
    .pc_from_fetch(pc),
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

  regfile registerFile(
    .clk(clk),
    .wdata(rf_write_val),
    .waddr(wb_rd),
    .wen(wb_rf_wen),
    .r_addr1(rs1),
    .r_addr2(rs2),
    .r_out1(regfile_rs1val),
    .r_out2(regfile_rs2val)
  );

  reg_dec_exe reg_de(
    .clk(clk),
    .reset(alu_rst),
    .enable(alu_wen),
    .rs1(rs1),
    .rs2(rs2),
    .rd(rd),
    .rs1val(regfile_rs1val),
    .rs2val(regfile_rs2val),
    .pc_from_decode(decode_pc),
    .imm64_from_decode(imm64),
    .shamt_from_decode(shamt),
    .opcode_from_decode(opcode),
    .instrId_from_decode(instr_id),
    .branch_from_decode(branch),
    .regw_from_decode(reg_w),
    .memr_from_decode(mem_r),
    .memw_from_decode(mem_w),
    .alu_zimm(alu_zimm),
    .alu_rd(alu_rd),
    .alu_rs1(alu_rs1),
    .alu_rs2(alu_rs2),
    .alu_rs1val(alu_rs1val),
    .alu_rs2val(alu_rs2val),
    .alu_pc(alu_pc),
    .alu_imm64(alu_imm64),
    .alu_shamt(alu_shamt),
    .alu_opcode(alu_opcode),
    .alu_instrId(alu_instrId),
    .alu_branch(alu_branch),
    .alu_regw(alu_regw),
    .alu_memr(alu_memr),
    .alu_memw(alu_memw)
  );

  alu_bypass alu_bypass1(
    .alu_rs1(alu_rs1),
    .alu_rs2(alu_rs2),
    .dmem_rd(dmem_rd),
    .dmem_regw(dmem_regw),
    .rs1_alu_bypass(rs1_alu_bypass),
    .rs2_alu_bypass(rs2_alu_bypss)
  );

  dmem_bypass dmem_bypass1(
    .alu_rs1(alu_rs1),
    .alu_rs2(alu_rs2),
    .wb_rd(wb_rd),
    .wb_regw(wb_regw),
    .rs1_dmem_bypass(rs1_dmem_bypass),
    .rs2_dmem_bypass(rs2_dmem_bypass)
  );

  alu_reg_mux alu_reg_mux1(
    .alu_bypass_rs1(rs1_alu_bypass),
    .alu_bypass_rs2(rs2_alu_bypss),
    .dmem_bypass_rs1(rs1_dmem_bypass),
    .dmem_bypass_rs2(rs2_dmem_bypass),
    .alu_rs1val(alu_rs1val),
    .alu_rs2val(alu_rs2val),
    .dmem_result(dmem_bypass_data),
    .wb_result(wb_bypass_data),
    .rs1val(rs1val),
    .rs2val(rs2val)
  );

  alu alu64(
    .clk (clk),
    .instr_in (alu_instrId),
    .rs1 (rs1val),
    .rs2 (rs2val),
    .imm (alu_imm64),
    .shamt (alu_shamt),
    .pc (alu_pc),
    .result (alu_result)
  );

  branch_resolver branch_resolver_0(
    .alu_branch(alu_branch),
    .alu_instrId(alu_instrId),
    .alu_imm64(alu_imm64),
    .alu_pc(alu_pc),
    .alu_result(alu_result),
    .mepc_val(mepc_val),
    .pc_rel_branch(pc_rel_branch),
    .pc_abs_branch(pc_abs_branch),
    .branch_base_pc(branch_base_pc),
    .pc_imm_in(pc_imm_in)
  );

  reg_alu_dmem reg_ad(
    .clk(clk),
    .rst(dmem_rst),
    .enable(dmem_wen),
    .zimm_from_alu(alu_zimm),
    .rd_from_alu(alu_rd),
    .pc_from_alu(alu_pc),
    .rs1val_from_alu(rs1val),
    .rs2val_from_alu(rs2val),
    .imm_from_alu(alu_imm64),
    .instrId_from_alu(alu_instrId),
    .opcode_from_alu(alu_opcode),
    .alu_result(alu_result),
    .regw_from_alu(alu_regw),
    .memw_from_alu(alu_memw),
    .memr_from_alu(alu_memr),
    .branch_from_alu(alu_branch),
    .zimm_to_dmem(dmem_zimm),
    .rd_to_dmem(dmem_rd),
    .pc_to_dmem(dmem_pc),
    .rs1val_to_dmem(dmem_rs1val),
    .rs2val_to_dmem(dmem_rs2val),
    .imm_to_dmem(dmem_imm64),
    .alu_result_to_dmem(dmem_result),
    .instrId_to_dmem(dmem_instrId),
    .opcode_to_dmem(dmem_opcode),
    .regw_to_dmem(dmem_regw),
    .memw_to_dmem(dmem_memw),
    .memr_to_dmem(dmem_memr),
    .branch_to_dmem(dmem_branch)
  );

  dcache #(.WAY_NUMBER(8)) dc0(
    .clk(clk),
    .rst(rst),
    // core interfaces
    .dcache_req(dmem_cache_req),
    .cache_rw(dmem_cache_rw),
    .core_addr_i(dmem_result),
    .core_data_i(dmem_rs2val), // riscv only writes to ram with the data in rs2
    .write_mask(wmask),
    .w_valid_o(dmem_w_valid),
    .r_valid_o(dmem_r_valid),
    .data_o(dmem_data),
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

  CSR csr(
    .clk(clk),
    .rst(rst),
    .wen(csr_wen),
    .excep_wen(0),
    .rs1(dmem_rs1val),
    .addr(dmem_imm64[11:0]),
    .instr(dmem_instrId),
    .imm(dmem_zimm),
    .mepc_val(mepc_val),
    .mtvec_val(mtvec_val),
    .mcause_overri(mcause_overri),
    .mepc_overri(mepc_overri),
    .csrVal(csrval)
  );

  reg_dmem_wb reg_dw(
    .clk(clk),
    .rst(wb_rst),
    .enable(wb_wen),
    .csrval(csrval),
    .rd_from_dmem(dmem_rd),
    .pc_from_dmem(dmem_pc),
    .result_from_dmem(dmem_result),
    .instrId_from_dmem(dmem_instrId),
    .dmem_data(dmem_sextdata),
    .rs1val_from_dmem(dmem_rs1val),
    .rs2val_from_dmem(dmem_rs2val),
    .regw_from_dmem(dmem_regw),
    .memw_from_dmem(dmem_memw),
    .memr_from_dmem(dmem_memr),
    .branch_from_dmem(dmem_branch),
    .opcode_from_dmem(dmem_opcode),
    .csrval_to_wb(wb_csrval),
    .rd_to_wb(wb_rd),
    .pc_to_wb(wb_pc),
    .result_to_wb(wb_result),
    .instrId_to_wb(wb_instrId),
    .dmem_data_to_wb(wb_memdata),
    .rs1val_to_wb(wb_rs1val),
    .rs2val_to_wb(wb_rs2val),
    .regw_to_wb(wb_regw),
    .memw_to_wb(wb_memw),
    .memr_to_wb(wb_memr),
    .branch_to_wb(wb_branch),
    .opcode_to_wb(wb_opcode)
 );

endmodule

module axi_ctl #(
    parameter RW_DATA_WIDTH  = 64,
    parameter RW_ADDR_WIDTH  = 64,
    parameter AXI_DATA_WIDTH = 64,
    parameter AXI_ADDR_WIDTH = 64,
    parameter AXI_ID_WIDTH   = 4,
    parameter AXI_STRB_WIDTH = AXI_DATA_WIDTH / 8,
    parameter AXI_USER_WIDTH = 1
) (
    input                             clk,
    input                             rst,
    output                            axi_ctrl_ready,
    input                             mmio_req,
    input                             mmio_rw,
    output reg                        mmio_valid,
    input      [                63:0] mmio_addr,
    input      [                63:0] mmio_data,
    //port 1
    input                             axi_req1,
    input                             rw_req1,
    input      [                63:0] addr1,
    input      [                63:0] data1,
    input      [                 7:0] rw_len1,
    output     [                63:0] data_o1,
    output reg                        axi_done1,
    //port 2
    input                             axi_req2,
    input                             rw_req2,
    input      [                63:0] addr2,
    input      [                63:0] data2,
    input      [                 7:0] rw_len2,
    output     [                63:0] data_o2,
    output reg                        axi_done2,
    // fifo interface
    input      [                 8:0] cache_fifo_idx,
    input      [                63:0] cache_fifo_data_i,
    input                             cache_fifo_done,
    input                             cache_fifo_wen,
    // Advanced eXtensible Interface
    input                             axi_aw_ready,
    output                            axi_aw_valid,
    output     [  AXI_ADDR_WIDTH-1:0] axi_aw_addr,
    output     [                 2:0] axi_aw_prot,
    output     [    AXI_ID_WIDTH-1:0] axi_aw_id,
    output     [  AXI_USER_WIDTH-1:0] axi_aw_user,
    output     [                 7:0] axi_aw_len,
    output     [                 2:0] axi_aw_size,
    output     [                 1:0] axi_aw_burst,
    output                            axi_aw_lock,
    output     [                 3:0] axi_aw_cache,
    output     [                 3:0] axi_aw_qos,
    output     [                 3:0] axi_aw_region,
    input                             axi_w_ready,
    output                            axi_w_valid,
    output     [  AXI_DATA_WIDTH-1:0] axi_w_data,
    output     [AXI_DATA_WIDTH/8-1:0] axi_w_strb,
    output                            axi_w_last,
    output     [  AXI_USER_WIDTH-1:0] axi_w_user,
    output                            axi_b_ready,
    input                             axi_b_valid,
    input      [                 1:0] axi_b_resp,
    input      [    AXI_ID_WIDTH-1:0] axi_b_id,
    input      [  AXI_USER_WIDTH-1:0] axi_b_user,
    input                             axi_ar_ready,
    output                            axi_ar_valid,
    output     [  AXI_ADDR_WIDTH-1:0] axi_ar_addr,
    output     [                 2:0] axi_ar_prot,
    output     [    AXI_ID_WIDTH-1:0] axi_ar_id,
    output     [  AXI_USER_WIDTH-1:0] axi_ar_user,
    output     [                 7:0] axi_ar_len,
    output     [                 2:0] axi_ar_size,
    output     [                 1:0] axi_ar_burst,
    output                            axi_ar_lock,
    output     [                 3:0] axi_ar_cache,
    output     [                 3:0] axi_ar_qos,
    output     [                 3:0] axi_ar_region,
    output                            axi_r_ready,
    input                             axi_r_valid,
    input      [                 1:0] axi_r_resp,
    input      [  AXI_DATA_WIDTH-1:0] axi_r_data,
    input                             axi_r_last,
    input      [    AXI_ID_WIDTH-1:0] axi_r_id,
    input      [  AXI_USER_WIDTH-1:0] axi_r_user
);

  `define READ_REQ 1'b0
  `define WRITE_REQ 1'b1
  localparam fifo_len = 8;


  // axi user interface
  reg         axi_rw_valid;
  wire        axi_ready;
  wire        axi_read_req;
  wire        axi_write_req;
  wire [63:0] axi_rdata;
  /* verilator lint_off UNDRIVEN */
  wire [63:0] axi_wdata;
  /* verilator lint_off UNDRIVEN */
  wire [ 7:0] axi_rw_size;
  reg  [ 7:0] axi_rw_len;
  reg         axi_req;

  assign axi_rw_size = 8'd1;
  assign axi_ctrl_ready = axi_ready && state == state_wait;
  assign axi_read_req = ~axi_rw_req & axi_req;
  assign axi_write_req = axi_rw_req & axi_req;

  // save the information of the serviced port
  reg [63:0] axi_req_addr;
  /* verilator lint_off UNUSED */
  reg [63:0] axi_req_data;
  /* verilator lint_off UNUSED */
  reg axi_rw_req;
  reg axi_req_size;
  // port selector
  reg port_sel;
  reg mmio;

  // data fifo for axi to access
  wire fifo_wen = (axi_rw_req == `WRITE_REQ  && cache_fifo_wen) | ( axi_rw_req == `WRITE_REQ  && mmio) | axi_fifo_wen;
  reg [64*8-1:0] fifo;

  reg r_last;
  wire axi_r_done;

  // state machine
  reg [3:0] state;
  reg [31:0] cnt;
  localparam state_wait = 0;
  localparam state_axi_trans = 1;
  localparam state_write = 2;
  localparam state_axi_fifo = 3;

  always @(posedge clk) begin
    if (rst) begin
      state     <= state_wait;
      axi_done1 <= 0;
      axi_done2 <= 0;
      mmio_valid <= 1;
    end else begin
      case (state)
        // wait for both user request and axi is available
        state_wait: begin
          axi_rw_valid <= 0;
          if ((axi_req1 | axi_req2) && axi_ready) begin
            port_sel     <= axi_req1 ? 0 : 1;
            axi_req      <= 1;
            axi_req_addr <= axi_req1 ? addr1 : addr2;
            axi_req_data <= axi_req1 ? data1 : data2;
            axi_rw_req   <= axi_req1 ? rw_req1 : rw_req2;
            axi_rw_len   <= axi_req1 ? rw_len1 : rw_len2;
            state        <= state_axi_trans;
            cnt          <= 0;
            // $display("handling cache request, rw:%d", axi_rw_req);
          end else if (mmio_req && axi_ready) begin
            mmio          <= 1;
            axi_req_addr  <= mmio_addr;
            axi_req_data  <= mmio_data;
            axi_rw_req    <= mmio_rw;
            axi_rw_len    <= 1;
            axi_req       <= 1;
            state         <= state_axi_trans;
            mmio_valid    <= 0;
            cnt           <= 0;
            // $display("handling mmio request rw:%d, addr:%x, data: %x", mmio_rw, mmio_addr, mmio_data);
          end
        end
        // read transcation
        state_axi_trans: begin
          // synchronize with axi_rw ,when axi_r_last comes,
          // last r_data will be latched into axi_r_data
          // so that r_last and last axi_rdata comes together
          r_last <= axi_r_last;
          // read transcation
          if (axi_rw_req == `READ_REQ) begin
            axi_rw_valid <= 1;
            if (axi_r_done) begin
              state   <= state_axi_fifo;
              // $display("axi ctrl axi r done");
            end
            if (fifo_wen) begin
              fifo[cnt+:64] <= axi_rdata;
              cnt           <= cnt + 64;
            end
          end  // write transcation
          else begin
            if (fifo_wen) begin
              fifo[cnt+:64] <= mmio ? axi_req_data : cache_fifo_data_i;
              cnt           <= mmio ? cnt : cnt + 64;
              state         <= mmio ? state_write : state;
              axi_rw_valid  <= mmio ? 1 : 0;
              // $display("write data, write req: %d, axi_req: %d",axi_write_req,axi_req);
            end
            // all 512 bits has been written
            if (cnt == 32'd512) begin
              cnt          <= 0;
              axi_rw_valid <= 1;
              state        <= state_write;
            end
          end
        end
        
        state_write: begin
          if (axi_fifo_ren)
            cnt <= cnt + 64;
          if (axi_b_valid) begin
            state   <= state_axi_fifo;
          end
        end
        
        // exchange data between caches and axi controller
        state_axi_fifo: begin
          r_last <= 0;
          axi_req <= 0;
          if (mmio) begin
            state <= state_wait;
            mmio  <= 0;
            mmio_valid <= 1;
            // $display("axi ctrl done");
          end else if (cache_fifo_done) begin
            state <= state_wait;
            axi_done1 <= 0;
            axi_done2 <= 0;
          end else begin
            axi_done1 <= port_sel == 0;
            axi_done2 <= port_sel == 1;
          end
          // data_o1   <= fifo[cache_fifo_idx+:64];
          // data_o2   <= fifo[cache_fifo_idx+:64];
        end
      endcase
    end
  end

  assign data_o1   = fifo[cache_fifo_idx+:64];
  assign data_o2   = fifo[cache_fifo_idx+:64];
  assign axi_wdata = fifo[cnt+:64];


  wire axi_fifo_wen;
  wire axi_fifo_ren;

  axi_rw a0 (
      .clock(clk),
      .reset(rst),

      .rw_valid_i (axi_rw_valid),  //IF&MEM输入信号
      .rw_ready_o (axi_ready),     //IF&MEM输入信号
      .data_read_o(axi_rdata),     //IF&MEM输入信号
      .rw_w_data_i(axi_wdata),     //IF&MEM输入信号
      .rw_addr_i  (axi_req_addr),  //IF&MEM输入信号
      .rw_size_i  (axi_rw_size),   //IF&MEM输入信号
      .rw_len_i   (axi_rw_len),    // burst 长度
      .read_req   (axi_read_req),  // 读请求
      .write_req  (axi_write_req), // 写请求

      //fifo interface
      .rfifo_wen      (axi_fifo_wen),
      .wfifo_ren      (axi_fifo_ren),
      .axi_r_done     (axi_r_done),
      // Advanced eXtensible Interface
      .axi_aw_ready_i (axi_aw_ready),
      .axi_aw_valid_o (axi_aw_valid),
      .axi_aw_addr_o  (axi_aw_addr),
      .axi_aw_prot_o  (axi_aw_prot),
      .axi_aw_id_o    (axi_aw_id),
      .axi_aw_user_o  (axi_aw_user),
      .axi_aw_len_o   (axi_aw_len),
      .axi_aw_size_o  (axi_aw_size),
      .axi_aw_burst_o (axi_aw_burst),
      .axi_aw_lock_o  (axi_aw_lock),
      .axi_aw_cache_o (axi_aw_cache),
      .axi_aw_qos_o   (axi_aw_qos),
      .axi_aw_region_o(axi_aw_region),

      .axi_w_ready_i(axi_w_ready),
      .axi_w_valid_o(axi_w_valid),
      .axi_w_data_o (axi_w_data),
      .axi_w_strb_o (axi_w_strb),
      .axi_w_last_o (axi_w_last),
      .axi_w_user_o (axi_w_user),

      .axi_b_ready_o(axi_b_ready),
      .axi_b_valid_i(axi_b_valid),
      .axi_b_resp_i (axi_b_resp),
      .axi_b_id_i   (axi_b_id),
      .axi_b_user_i (axi_b_user),

      .axi_ar_ready_i (axi_ar_ready),
      .axi_ar_valid_o (axi_ar_valid),
      .axi_ar_addr_o  (axi_ar_addr),
      .axi_ar_prot_o  (axi_ar_prot),
      .axi_ar_id_o    (axi_ar_id),
      .axi_ar_user_o  (axi_ar_user),
      .axi_ar_len_o   (axi_ar_len),
      .axi_ar_size_o  (axi_ar_size),
      .axi_ar_burst_o (axi_ar_burst),
      .axi_ar_lock_o  (axi_ar_lock),
      .axi_ar_cache_o (axi_ar_cache),
      .axi_ar_qos_o   (axi_ar_qos),
      .axi_ar_region_o(axi_ar_region),

      .axi_r_ready_o(axi_r_ready),
      .axi_r_valid_i(axi_r_valid),
      .axi_r_resp_i (axi_r_resp),
      .axi_r_data_i (axi_r_data),
      .axi_r_last_i (axi_r_last),
      .axi_r_id_i   (axi_r_id),
      .axi_r_user_i (axi_r_user)
  );

endmodule

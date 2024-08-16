module dcache #(WAY_NUMBER = 8)
    (input                       clk,
     input                       rst,
     // core interface
     input                       axi_ready,
     input                       dcache_req,
     input                       cache_rw,
     input           [3:0]       write_mask,
     input           [63:0]      core_addr_i,
     input           [63:0]      core_data_i,
     // axi_ctl interface
     input                       axi_done,
     input           [63:0]      axi_data_i,
     output reg      [63:0]      axi_fifo_data_o,
     output reg                  axi_fifo_wen,
     output reg      [63:0]      axi_req_addr,
     output reg      [8:0]       axi_fifo_idx,
     output reg                  axi_fifo_done,
     output reg                  axi_req,
     output reg                  axi_rw,
     // cache output
     output                      w_valid_o,
     output                      r_valid_o,
     output reg      [63:0]      data_o
    );

`define TRUE            1'b1
    `define CACHE_READ      1'b0
    `define CACHE_WRITE     1'b1
    `define AXI_READ        1'b0
    `define AXI_WRITE       1'b1

    // 4k bytes set , 64 bytes block for 64 blocks(lines)
    wire [5:0] index  = core_addr_i[11:6];
    wire [5:0] offset = core_addr_i[5:0];
    wire [51:0] tag   = core_addr_i[63:12];

    /* for set associative cache , block offset width is the width of numbers of bytes in a block
     block index width is the width of the number of the blocks ,which is TOTALSIZE / BLOCKSIZE
     in here is 4K/64 = 64 blocks
     
     64bit word address
     [valid][presence] [tag][block index][block offset]
                        63:12   11:6       5:0
     */

    // for each line , there is valid and tag field
    // tags are 52 bits long and each set has 64 blocks(lines) so there is 64 tags
    reg [51:0] line_tag [WAY_NUMBER-1:0][63:0];
    // valid bit for each block
    // same applies to every way
    reg line_valid [WAY_NUMBER-1:0][63:0];
    reg line_dirty [WAY_NUMBER-1:0][63:0];
    // for each way , there is a precense bit to control way behaviors
    wire presence [WAY_NUMBER-1:0];
    // wires connecting to internal ram
    reg  [63:0] way_ram_in [WAY_NUMBER-1:0];
    wire [63:0] way_ram_out [WAY_NUMBER-1:0];
    reg         way_wen [WAY_NUMBER-1:0];
    reg  [63:0] store_addr_buffer;
    reg  [63:0] store_buffer;
    wire [63:0] cache_r_data;

    reg  [3:0]   ram_write_mask;
    reg  [63:0]  ram_w_addr;
    reg  [63:0]  ram_addr;
    reg  [63:0]  ram_r_offset;

    // generate number of ram block for each way
    generate
        for(genvar j = 0;j<WAY_NUMBER;j = j+1) begin
            cache_ram #(.O_WIDTH(64)) ram
                      (
                          .clk(clk),
                          .wen(way_wen[j]),
                          // read offset
                          .r_addr(core_addr_i + ram_r_offset),
                          // write offset , starts from 1 , decrement 1 to start writing at addr 0
                          .w_addr(ram_w_addr),
                          .write_mask(ram_write_mask),
                          .data_in(way_ram_in[j]),
                          .data_out(way_ram_out[j])
                      );
        end
    endgenerate

    /* check whether the tag of the specified index(line)
     matches any of the line tags in each way
     if the line is valid then set the presence bit
    */

    generate
        for(genvar way = 0;way<WAY_NUMBER;way = way+1) begin
            assign presence[way] = (line_tag[way][index] == tag && line_valid[way][index] == `TRUE) ? 1 : 0;
        end
    endgenerate

    // in order to or presence bits together , generate a presence bus wire
    // for the sake of using verilog | operator
    wire [WAY_NUMBER-1:0] presence_w;
    generate
        for(genvar k = 0;k<WAY_NUMBER;k++) begin
            assign presence_w[k] = presence[k];
        end
    endgenerate

    // using | to or presence together to form hit field
    wire cache_hit;
    assign cache_hit = |presence_w;

    // kv based multiplexor for way data selection
    // a lut is required to form the mutiplexing mapping
    wire [$clog2(WAY_NUMBER)-1:0] hit_way;
    localparam lut_entry_len = $clog2(WAY_NUMBER)+WAY_NUMBER;

    // lut is reuiqred to generate kv mutiplexor mapping
    wire [(WAY_NUMBER*lut_entry_len)-1:0] mux_lut;
    /*  key in kv mapping , 4 way example
     0000 is default
     0001 is way zro
     0010 is way one
     */
    wire [WAY_NUMBER-1:0] way_key[WAY_NUMBER-1:0];
    generate
        for(genvar i = 0;i<WAY_NUMBER;i = i+1) begin
            assign way_key[i]                              = 1<<(i[WAY_NUMBER-1:0]);
            assign mux_lut[i*lut_entry_len+:lut_entry_len] = {way_key[i],i[$clog2(WAY_NUMBER)-1:0]};
        end
    endgenerate

    MuxKey #(.NR_KEY(WAY_NUMBER), .KEY_LEN(WAY_NUMBER), .DATA_LEN($clog2(WAY_NUMBER))) 
            m0 (hit_way, presence_w, 0, mux_lut);


    // PLRU replacement policy
    // for N way cache , N-1 bits is needed to store PLRU state info
    /*
     8way example
     
            [1]        // root nodes
           /   \
      [2]         [3]
     /   \        /  \
   [4]    [5]   [6]   [7]
   /  \  / \    / \   /  \ 
  w0  w1 w2 w3 w4 w5 w6  w7 */

    /* verilator lint_off UNOPTFLAT */
    // plru tree direction struct
    // tree node number from 1 to way number
    reg [WAY_NUMBER-1:1] plru_dir;
    wire [$clog2(WAY_NUMBER)-1:0] node_id[$clog2(WAY_NUMBER)-1:0];

    // treverse the plru tree , from level 2 , to last non leaf level
    // apparently first level will always be the root node , which is node 1
    assign node_id[0] = 1;
    generate
        for(genvar i = 1; i < $clog2(WAY_NUMBER); i = i + 1) begin
            assign node_id[i] = plru_dir[node_id[i-1]] ? node_id[i-1] << 1 : (node_id[i-1] << 1) + 1;
        end
    endgenerate

    /* verilator lint_off UNOPTFLAT */
    wire [$clog2(WAY_NUMBER)-1:0] way_replace;
    wire [$clog2(WAY_NUMBER)-1:0] last_node_id = node_id[$clog2(WAY_NUMBER)-1];

    // last node specifies the target way number , node id is 1 larger than the target way number
    assign way_replace = plru_dir[last_node_id] ? ((last_node_id) << 1) - 3'd7 - 3'd1 : (last_node_id << 1) - 3'd7;

    /*state machine of the dcache procedure
     whenever input addr is set , the presence bits are imediately confirmed
     then state is divided into cache hit and miss states
     */

    reg [3:0] state;
    reg [31:0] cnt;
    reg [3:0] plru_step_count;
    reg [$clog2(WAY_NUMBER)-1:0] cur_replace_way;
    localparam state_check          = 4'd0;
    localparam state_write_dirty    = 4'd1;
    localparam state_refill         = 4'd2;

    assign w_valid_o = cache_hit & dcache_req & cache_rw == `CACHE_WRITE;
    assign r_valid_o = cache_hit & dcache_req & cache_rw == `CACHE_READ;
    assign cache_r_data = (core_addr_i == store_addr_buffer) ? store_buffer : way_ram_out[hit_way];
    assign data_o = cache_hit ? cache_r_data : 64'd0;

    always_ff @(posedge clk) begin
        if (rst) begin
            // on reset , clear all valid bits on all ways and all lines
            for(integer x = 0; x< WAY_NUMBER; x = x+1) begin
                for(integer y = 0; y< 64 ;y = y+1)  begin
                    line_valid[x][y] <= 0;
                    line_dirty[x][y] <= 0;
                end
            end
            // reset wen for all ways
            for(integer x=0; x<WAY_NUMBER; x=x+1) begin
                way_wen[x] <= 0;
                plru_dir[x] <= 0;
            end
            // set default state and output valid bit
            state           <= state_check;
        end

        // state machine of icache
        case(state)
            // check state
            state_check: begin
                if(dcache_req & core_addr_i >= 64'h0000_0000_8000_0000) begin
                    axi_fifo_done                           <= 0;
                    ram_r_offset                            <= 0;
                    if (cache_hit) begin
                        if(cache_rw == `CACHE_WRITE) begin
                            // $display("write way: %d, block:%d, offset:%d, data:%x, mask:%d",hit_way,index,offset,core_data_i, write_mask);
                            ram_w_addr                      <= core_addr_i;
                            way_ram_in[hit_way]             <= core_data_i;
                            store_addr_buffer               <= core_addr_i;
                            store_buffer                    <= core_data_i;
                            way_wen[hit_way]                <= 1;
                            ram_write_mask                  <= write_mask;
                            // if write to cache , set dirty bits
                            line_dirty[hit_way][index]      <= 1;
                        end 
                        // else if(core_addr_i >= 64'h0000_0000_8100_0000) begin
                        //     $display("read addr:%x way: %d, block:%d, offset:%d,data:%x, store_addr:%x, store_buffer:%x",core_addr_i,hit_way,index,offset,data_o,store_addr_buffer,store_buffer);
                        // end
                    end
                    // cache miss
                    else begin
                        // resolve plru
                        ram_addr        <= core_addr_i;
                        if (plru_step_count == 2) begin
                            plru_dir[1] <= ~plru_dir[1];
                            plru_step_count <= plru_step_count + 1;
                        end
                        else if (plru_step_count == 1) begin
                            plru_dir[2] <= plru_dir[1] == 0 ? ~plru_dir[2] : plru_dir[2];
                            plru_dir[3] <= plru_dir[1] == 1 ? ~plru_dir[3] : plru_dir[3];
                            plru_step_count <= plru_step_count + 1;
                        end
                        else if (plru_step_count == 0) begin
                            plru_dir[4] <= plru_dir[1] == 0 && plru_dir[2] == 0 ? ~plru_dir[4] : plru_dir[4];
                            plru_dir[5] <= plru_dir[1] == 0 && plru_dir[2] == 1 ? ~plru_dir[5] : plru_dir[5];
                            plru_dir[6] <= plru_dir[1] == 1 && plru_dir[3] == 0 ? ~plru_dir[6] : plru_dir[6];
                            plru_dir[7] <= plru_dir[1] == 1 && plru_dir[3] == 1 ? ~plru_dir[7] : plru_dir[7];
                            plru_step_count <= plru_step_count + 1;
                        end
                        else if(plru_step_count == 3) begin
                            // if (ram_w_addr >= 64'h0000_0000_8100_0000) begin
                            //     $display("addr:%x replace way: %d, block:%d dirty:%d",
                            //         ram_addr,way_replace,index,line_dirty[way_replace][index]);
                            // end
                            way_wen[hit_way]    <= 0;
                            axi_fifo_idx        <= 0;
                            cnt                 <= 0;
                            plru_step_count     <= 0;
                            axi_req             <= line_dirty[way_replace][index] ? 0 : 1;                   // if the line is dirty , request axi write later
                            axi_rw              <= line_dirty[way_replace][index] ? `AXI_WRITE : `AXI_READ;
                            axi_req_addr        <= ram_addr - {{58{1'b0}},offset};
                            cur_replace_way     <= way_replace;
                            if (axi_ready) begin
                                state           <= line_dirty[way_replace][index] ? state_write_dirty : state_refill;
                            end
                        end
                    end
                end
                else begin
                    for(integer x =0;x<WAY_NUMBER; x=x+1) begin
                        way_wen[x] <= 0;
                        plru_step_count <= 0;
                    end
                end
            end
            // if the way being replaced is dirty , write back to ram first
            state_write_dirty: begin
                // $display("write dirty");
                axi_fifo_wen    <= 1;                               // send data to axi_ctl fifo
                ram_r_offset    <= {{32{1'b0}},cnt} - {{58{1'b0}},offset};
                axi_fifo_data_o <= way_ram_out[cur_replace_way];
                cnt             <= cnt + 8;                         // count how many bytes has been written
                // if all 64 bytes has been written to axi_ctl fifo , fire an axi write request
                // to write the dirty line back to ram
                if(cnt >= 32'd64) begin
                    if(axi_done) begin
                        // goto refill
                        axi_fifo_done <= 1;
                        state         <= state_refill;
                        axi_rw        <= `AXI_READ;
                        cnt           <= 0;
                    end
                    else begin
                        axi_req         <= 1;
                        axi_fifo_wen    <= 0;
                        //ram_r_addr        <= 0;   // reset offset to zero to maintain that the cache_ram port always has the data of input addr
                    end
                end
            end
            // refill state
            state_refill: begin
                // request the entire line (which is the address of (address - offset) )
                // refill the entire line
                ram_w_addr    <= axi_req_addr;
                if (cnt == 32'd64) begin
                    state                                   <= state_check;
                    axi_fifo_done                           <= 1;
                    axi_req                                 <= 0;
                    line_dirty[cur_replace_way][index]      <= 0;
                    line_valid[cur_replace_way][index]      <= 1;
                    line_tag[cur_replace_way][index]        <= tag;
                end
                // if the axi transcation has been finished , ready to fill the cache line
                if(axi_done) begin
                    // write 8 bytes a time to cache_ram
                    // $display("cache read addr:%x data:%x", ram_w_addr, axi_data_i);
                    ram_write_mask              <= 4'd8;
                    way_wen[cur_replace_way]    <= cnt == 32'd64 ? 0:1;
                    way_ram_in[cur_replace_way] <= axi_data_i;
                    // $display("AXI data: %x", axi_data_i);
                    cnt                         <= way_wen[cur_replace_way] ? cnt + 8 : cnt;
                    ram_w_addr                  <= way_wen[cur_replace_way] ? axi_req_addr + {{32{1'b0}},cnt} : ram_w_addr;
                    axi_fifo_idx                <= way_wen[cur_replace_way] ? axi_fifo_idx + 64 : 0;
                end // end axi_done
            end
            default: begin
                state <= state_check;
            end
        endcase
    end // end always
endmodule
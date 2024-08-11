module icache #(WAY_NUMBER = 8)
               (input clk,
                input rst,
                // core interface
                input           [63:0]  core_addr_i,
                output          [31:0]  data_o,
                output                  valid_o,
                // axi interface
                input                   axi_done,
                input           [63:0]  axi_data_i,
                output reg      [63:0]  axi_req_addr,
                output reg      [8:0]   axi_fifo_idx,
                output reg              axi_fifo_done,
                output reg              axi_req
                );
    
    localparam true = 1;
    
    // 4k bytes set , 64 bytes block for 64 blocks(lines)
    wire [5:0] offset = core_addr_i[5:0];
    wire [5:0] index  = core_addr_i[11:6];
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
    // for each way , there is a precense bit to control way behaviors
    reg presence [WAY_NUMBER-1:0];
    // wires connecting to internal ram
    wire [31:0] way_ram_out [WAY_NUMBER-1:0];
    reg  [63:0] way_ram_in [WAY_NUMBER-1:0];
    reg        way_wen [WAY_NUMBER-1:0];
    
    reg  [3:0] ram_write_mask;
    reg  [63:0] ram_r_addr;
    reg  [63:0] ram_w_addr;

    // generate number of ram block for each way
    generate
    for(genvar j = 0;j<WAY_NUMBER;j = j+1) begin
        cache_ram #(.O_WIDTH(32)) ram
        (
        .clk(clk),
        .wen(way_wen[j]),
        // read offset
        .r_addr(ram_r_addr),
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
        assign presence[way] = (line_tag[way][index] == tag && line_valid[way][index] == true) ? 1 : 0;
    end
    endgenerate
    
    // in order to or presence bits together , generate a presence bus wire
    // for the sake of using verilog | operator
    wire [WAY_NUMBER-1:0] presence_w ;
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
    reg [$clog2(WAY_NUMBER)-1:0] hit_way;
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
    
    // kv based way_data mutiplexor
    // provided by ysyx.org
    MuxKey #(.NR_KEY(WAY_NUMBER), .KEY_LEN(WAY_NUMBER), .DATA_LEN($clog2(WAY_NUMBER))) m0 (hit_way, presence_w, 0, mux_lut);
    
    
    // PLRU replacement policy
    // for N way cache , N-1 bits is needed to store PLRU state info
    /*
     4way example
     
         [1]        // root nodes
        /   \
     [2]     [3]
     /   \   /   \
    w0   w1  w2   w3
    */

    /* verilator lint_off UNOPTFLAT */
    // plru tree direction struct
    // tree node number from 1 to way number
    reg [WAY_NUMBER-1:1] plru_dir;
    wire [$clog2(WAY_NUMBER)-1:0] node_id[$clog2(WAY_NUMBER)-1:0];
    
    // treverse the plru tree , from level 2 , to last non leaf level
    // apparently first level will always be the root node , which is node 1
    assign node_id[0] = 1;
    generate
    for(genvar i = 1;i<$clog2(WAY_NUMBER);i = i + 1) begin
        assign node_id[i] = plru_dir[node_id[i-1]] ? node_id[i-1] << 1 : (node_id[i-1] << 1) +1;
    end
    endgenerate


    // update PLRU bits
    // treat PLRU nodes as a binary tree , all nodes are non leaf nodes
    // so there are clog2(WAY_NUMBER)-1 non leaf levels
    generate
        assign plru_dir[1] = (|presence_w[0+:(WAY_NUMBER/2)] && !(|presence_w[(WAY_NUMBER/2)+:WAY_NUMBER/2])) ? 0 : 1;
        // start from level 2
        for (genvar i = 1; i < $clog2(WAY_NUMBER); i = i+1) begin
            for (genvar j = 0; j < 2**i; j = j+1) begin // for each level there is 2^i nodes
                // for each node , checking if the way number falls in the coresponding range
                // if true , check if the way number falls in which child node range, set to 0 if is in left child , otherwise 1
                // if false remain unchanged
                assign plru_dir[2**i+j] = (cache_hit & (((2**i+j) >> 1) + plru_dir[(2**i+j) >> 1] == 2**i+j )) ? // is father node pointing to us ?
                // if pointing to us , check if the presence bits falls to children's coresponding range , and set bit
                ((|presence_w[(WAY_NUMBER/(i<<2))*((j<<1)+j)+:(WAY_NUMBER/(i<<2))] && !(|presence_w[(WAY_NUMBER/(i<<2))*((j<<1)+j+1)+:WAY_NUMBER/(i<<2)])) ? 0:1) : 
                // otherwise remain unchanged
                plru_dir[2**i+j];
            end
        end
    endgenerate
    
    /* verilator lint_off UNOPTFLAT */
    wire [$clog2(WAY_NUMBER)-1:0] way_replace;
    wire [$clog2(WAY_NUMBER)-1:0] last_node_id = node_id[$clog2(WAY_NUMBER)-1];

    // last node specifies the target way number , node id is 1 larger than the target way number
    assign way_replace = plru_dir[last_node_id] ? (last_node_id-1) : last_node_id;

    /*state machine of the icache procedure
     whenever input addr is set , the presence bits are imediately confirmed
     then state is divided into cache hit and miss states
     */
    
    reg [3:0] state;
    reg [31:0] cnt;
    reg [$clog2(WAY_NUMBER)-1:0] cur_replace_way;
    localparam state_check  = 4'b0;
    localparam state_refill = 4'b1;

    // assign valid_o = cache_hit;
    assign valid_o = cache_hit;
    assign data_o = way_ram_out[hit_way];
    assign ram_r_addr = core_addr_i;

    always_ff @(posedge clk) begin
        if (rst) begin
            // on reset , clear all valid bits on all ways and all lines
            for(integer x = 0; x< WAY_NUMBER; x = x+1) begin
                for(integer y = 0; y< 64 ;y = y+1)  begin
                    line_valid[x][y] <= 0;
                end
            end
            // reset wen for all ways
            for(integer x =0;x<WAY_NUMBER; x=x+1) begin
                way_wen[x] <= 0;
            end
            // set default state and output valid bit
            state   <= state_check;
        end
        
        // state machine of icache
        case(state)
            // check state
            state_check: begin
                // in case of previous write , clear write bit
                // if there was a write , way_replace will not change since 
                // now is a retry of the last cache visit
                axi_fifo_done               <= 0;           // release axi_ctl transcation
                for(byte i=0;i<WAY_NUMBER-1;i=i+1)          // disable all write 
                    way_wen[i[2:0]] <= 0;
                if (cache_hit) begin
                    // ram_r_addr                      <= core_addr_i;
                    state                           <= state;
                    // valid_o         <= cache_hit;
                    // data_o          <= way_ram_out[hit_way];
                end else begin
                    if(core_addr_i >= 64'h0000_0000_8000_0000) begin
                        // set external fifo index to 0
                        axi_fifo_idx    <= 0;
                        // init ram counters
                        ram_w_addr      <= core_addr_i - {{58{1'b0}},offset};
                        cnt             <= 0;
                        // fire an axi request
                        axi_req         <= 1;
                        axi_req_addr    <= core_addr_i - {{58{1'b0}},offset};
                        // track which way to replace
                        cur_replace_way <= way_replace;
                        // change state to refill
                        state           <= state_refill;
                    end
                end
            end
            // refill state
            state_refill: begin
                // request the entire line (which is the address of (address - offset) )
                // refill the entire line
                // if the axi transcation has been finished , ready to fill the cache line
                if(axi_done) begin
                    // write 8 bytes a time to cache_ram
                    ram_write_mask              <= 4'd8;
                    way_wen[cur_replace_way]    <= 1;
                    way_ram_in[cur_replace_way] <= axi_data_i;
                    cnt                         <= cnt + 8;
                    ram_w_addr                  <= axi_req_addr + {{32{1'b0}},cnt};
                    axi_fifo_idx                <= axi_fifo_idx + 64;
                end // end axi_done

                if (cnt == 32'd64) begin
                    // after 64 bytes are all filled (replaced an entire line)
                    // set the target line tag and valid bits
                    // change state to cache check again , theoratically will fix the cache miss
                    state                                   <= state_check;
                    axi_fifo_done                           <= 1;
                    axi_req                                 <= 0;
                    line_valid[cur_replace_way][index]      <= 1;
                    line_tag[cur_replace_way][index]        <= tag;
                    way_wen[cur_replace_way]                <= 0;
                end
            end
            default: begin
                state <= state_check;
            end
        endcase
    end // end always
endmodule

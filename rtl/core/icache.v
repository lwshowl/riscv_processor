module icache #(WAY_NUMBER = 8)
               (input clk,
                input rst,
                input [63:0] axi_addr_i,
                input [63:0] axi_data_i,
                input axi_done,
                output reg [63:0] axi_req_addr,
                output reg [8:0] axi_fifo_idx,
                output reg fifo_done,
                output reg axi_r_req,
                output reg valid_o,
                output reg [31:0] data_o);
    
    localparam true = 1;
    
    // 4k bytes set , 64 bytes block for 64 blocks(lines)
    wire [5:0] index  = axi_addr_i[11:6];
    wire [5:0] offset = axi_addr_i[5:0];
    wire [51:0] tag   = axi_addr_i[63:12];
    
    /* for set associative cache , block offset width is the width of numbers of bytes in a block
     block index width is the width of the number of the blocks ,which is TOTALSIZE / BLOCKSIZE
     in here is 4K/64 = 64 blocks
     
     64bit word address
     [valid][presence] [tag][block index][block offset]
                        63:12   11:6       5:0
     */
    
    // for each line , there is valid and tag field
    // tags are 52 bits long and each set has 64 blocks(lines) so there is 64 tags
    reg [51:0] way_tag [WAY_NUMBER-1:0][63:0];
    // valid bit for each block
    // same applies to every way
    reg way_valid [WAY_NUMBER-1:0][63:0];
    // for each way , there is a precense bit to control way behaviors
    reg presence [WAY_NUMBER-1:0];
    // wires connecting to internal ram
    reg [7:0] way_ram_in [WAY_NUMBER-1:0];
    reg [31:0] way_ram_out [WAY_NUMBER-1:0];
    reg way_wen [WAY_NUMBER-1:0];

    reg [3:0] write_mask;
    
    // generate number of ram block for each way
    generate
    for(genvar j = 0;j<WAY_NUMBER;j = j+1) begin
        cache_ram #(.O_WIDTH(32)) ram
        (
        .clk(clk),
        .wen(way_wen[j]),
        .index(index),
        // read offset
        .r_offset(offset),
        // write offset , starts from 1 , decrement 1 to start writing at addr 0
        .w_offset(w_offset[5:0]),
        .write_mask(write_mask),
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
        assign presence[way] = (way_tag[way][index] == tag && way_valid[way][index] == true) ? 1 : 0;
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
                // is father node pointing to us ?
                assign plru_dir[2**i+j] = (cache_hit & (((2**i+j) >> 1) + plru_dir[(2**i+j) >> 1] == 2**i+j )) ?
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

    reg [6:0] w_offset;

    /*state machine of the icache procedure
     whenever input addr is set , the presence bits are imediately confirmed
     then state is divided into cache hit and miss states
     */
    
    reg [3:0] state;
    reg [31:0] cnt;
    reg [$clog2(WAY_NUMBER)-1:0] cur_replace_way;
    localparam state_check  = 4'b0;
    localparam state_refill = 4'b1;

    assign valid_o = cache_hit;
    assign data_o = cache_hit ? way_ram_out[hit_way] : 32'd0;

    always @(posedge clk) begin
        if (rst) begin
            // on reset , clear all valid bits on all ways and all lines
            for(integer x = 0; x< WAY_NUMBER; x = x+1) begin
                for(integer y = 0; y< 64 ;y = y+1)  begin
                    way_valid[x][y] <= 0;
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
                // now is a retry of the cache visit
                way_wen[way_replace] <= 0;
                if (cache_hit) begin
                    state <= state;
                    // valid_o         <= cache_hit;
                    // data_o          <= way_ram_out[hit_way];
                end else begin
                    // set external fifo index to 0
                    axi_fifo_idx        <= 0;
                    // init ram counters
                    w_offset        <= 0;
                    cnt             <= 0;
                    axi_r_req       <= 1;
                    // track which way to replace
                    cur_replace_way <= way_replace;
                    // change state to refill
                    state           <= state_refill;
                end
            end
            // refill state
            state_refill: begin
                  // request the entire line (which is the address of (address - offset) )
                axi_req_addr <= addr_i - {{58{1'b0}},offset};
                // refill the entire line
                if (axi_fifo_idx == 9'd512) begin
                    // after 64 bytes are all filled (replaced an entire line)
                    // set the target line tag and valid bits
                    // change state to cache check again , theoratically will fix the cache miss
                    state                           <= state_check;
                    fifo_done                       <= 1;
                    axi_r_req                       <= 0;
                    way_valid[cur_replace_way][index]   <= 1;
                    way_tag[cur_replace_way][index]     <= tag;
                end
                // if the axi transcation has been finished , ready to fill the cache line
                if(axi_done) begin
                    way_wen[cur_replace_way] <= 1;
                    write_mask <= 8'd8;
                    way_ram_in[cur_replace_way] <= axi_data_i;
                    axi_fifo_idx <= axi_fifo_idx + 64;
                    w_offset    <= way_wen[cur_replace_way] ? w_offset + 8 : w_offset;
                end // end axi_done
            end
            default: begin
                state <= state_check;
            end
        endcase
    end // end always
endmodule

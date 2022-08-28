module icache #(WAY_NUMBER = 8)
               (input clk,
                input rst,
                input [63:0] addr_i,
                input [63:0] ext_data,
                input axi_last_data,
                output reg [63:0] axi_req_addr,
                output reg axi_r_req,
                output reg valid_o,
                output reg [31:0] data_o);
    
    localparam true = 1;
    
    // 4k bytes set , 64 bytes block for 64 blocks(lines)
    wire [5:0] index  = addr_i[11:6];
    wire [5:0] offset = addr_i[5:0];
    wire [51:0] tag   = addr_i[63:12];
    
    /* for set associative cache , block offset width is the width of numbers of bytes in a block
     block index width is the width of the number of the blocks ,which is TOTALSIZE / BLOCKSIZE
     in here is 4K/64 = 64 blocks
     
     64bit word address
     [valid][presence] [tag][block index][block offset]
     63:12   11:6           5:0
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
    
    // generate number of ram block for each way
    generate
    for(genvar j = 0;j<WAY_NUMBER;j = j+1) begin
        cache_ram #(.O_WIDTH(32)) ram
        (
        .clk(clk),
        .wen(way_wen[j]),
        .index(index),
        .offset(offset),
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
    wire [31:0] cache_data;
    localparam lut_entry_len = 32+WAY_NUMBER;
    
    // lut is reuiqred to generate kv mutiplexor mapping
    wire [WAY_NUMBER*lut_entry_len-1:0] mux_lut;
    /*  key in kv mapping , 4 way example
     0000 is default
     0001 is way one
     0010 is way two
     */
    wire [WAY_NUMBER-1:0] way_key;
    generate
    for(genvar i = 0;i<WAY_NUMBER;i = i+1) begin
        assign way_key                                 = 1<<i[WAY_NUMBER-1:0];
        assign mux_lut[i*lut_entry_len+:lut_entry_len] = {way_key,way_ram_out[i]};
    end
    endgenerate
    
    // kv based way_data mutiplexor
    // provided by ysyx.org
    MuxKey #(.NR_KEY(WAY_NUMBER), .KEY_LEN(WAY_NUMBER), .DATA_LEN(32)) m0 (cache_data, presence_w, 32'b0, mux_lut);
    
    /*state machine of the icache procedure
     whenever input addr is set , the presence bits are imediately confirmed
     then state is divided into cache hit and miss states
     */
    
    reg [3:0] state;
    reg [31:0] cnt;
    localparam state_check  = 4'b0;
    localparam state_refill = 4'b1;
    
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
    
    always @(posedge clk) begin
        if (rst) begin
            // on reset , clear all valid bits on all ways and all lines
            for(integer x = 0; x< WAY_NUMBER; x = x+1) begin
                for(integer y = 0; y< 64 ;y = y+1)  begin
                    way_valid[x][y] <= 0;
                end
                cnt      <= 0;
            end
            // set default state and output valid bit
            state   <= state_check;
            valid_o <= 0;
            end else begin // begin not rst
            if (state == state_check) begin
                if (cache_hit) begin
                    valid_o <= 1;
                    data_o  <= cache_data;
                    way_wen[way_replace] <= 0;
                end else begin
                    valid_o <= 0;
                    state   <= state_refill;
                end
            end else if (state == state_refill) begin
                axi_r_req  <= 1;
                axi_req_addr <= addr_i;
                way_wen[way_replace] <= 1;
                // refill the entire line
                if (axi_last_data == 1 && cnt == 32'd64) begin
                    state                  <= state_check;
                    way_valid[way_replace][index] <= 1;
                    way_tag[way_replace][index] <= tag;
                end
                way_ram_in[way_replace] <= ext_data[cnt+:8];
                cnt <= cnt + 8;
            end // end state_refiil
        end // end not rst
    end // end always
endmodule

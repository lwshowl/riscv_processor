module set (input clk,
            input rst,
            input [51:0] tag,
            input [5:0] index,
            input [5:0] offset,
            input [7:0] len,
            output reg presence,
            output reg [63:0] data);
    
    // 4k bytes set , 64 bytes block
    wire [63:0] ram_out;
    cache_ram ram
    (
    .clk(clk),
    .wen(wen),
    .index(index),
    .offset(offset),
    .data_in(ram_in),
    .data_out(ram_out)
    );
    
    wire wen         = 0;
    wire [7:0] ram_in;
    
    /* for set associative cache , block offset is the width of the block length
     block index is the width of the number of the blocks ,which is SIZE / BLOCKSIZE
     
     64bytes
     [valid]     [tag][block index][block offset]
     64           63:12   11:6           5:0
     
     */
    
    // valid bit for each block
    // tags are 52 bits long and each set has 64 blocks
    reg valid [0:63];
    reg [51:0] tags [0:63];
    integer  i;
    always @(posedge clk) begin
        if (rst) begin
            for(i = 0; i< 64 ; i = i+1)
                valid[i] = 0;
        end
        
        else if (tag == tags[index]) begin
            presence <= 1;
            data     <= ram_out;
        end
    end
endmodule

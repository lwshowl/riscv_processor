module cache_ram #(O_WIDTH = 64)
                  (input clk,
                   input wen,
                   input [5:0] index,
                   input [3:0] write_mask,
                   input [5:0] r_offset,
                   input [5:0] w_offset,
                   input [63:0] data_in,
                   output reg [O_WIDTH-1:0] data_out);
    
    reg [7:0] mem[63:0][63:0];
    
    generate
        for(genvar idx= 0;idx<O_WIDTH/8;idx = idx+1)
            assign data_out[idx*8+:8] = mem[index][r_offset+idx[5:0]];
    endgenerate

    always @(posedge clk) begin
        if (wen) begin
            for(byte idx= 0;idx < write_mask;idx = idx + 1)
                mem[index][w_offset + idx] <= data_in[idx*8+:8];
        end
    end
endmodule

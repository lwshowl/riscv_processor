module cache_ram 
            (input clk,
             input wen,
             input [5:0] index,
             input [5:0] offset,
             input [7:0] data_in,
             output reg [63:0] data_out);
    
    reg [7:0] mem [0:63][0:63];
    
    always @(posedge clk) begin
        if (wen) begin
            mem[index][offset] <= data_in;
        end
        else begin
            // output an entire 64bit word
            data_out <= {mem[index][offset+7],mem[index][offset+6],
                            mem[index][offset+5],mem[index][offset+4],
                                mem[index][offset+3],mem[index][offset+2],
                                    mem[index][offset+1],mem[index][offset]};
        end
    end
endmodule

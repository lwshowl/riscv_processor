module cache_ram #(O_WIDTH = 64)
                  (input clk,
                   input wen,
                   input [3:0]  write_mask,
                   /* verilator lint_off UNUSED */
                   input [63:0] w_addr,
                   input [63:0] r_addr,
                   /* verilator lint_off UNUSED */
                   input [63:0] data_in,
                   output [O_WIDTH-1:0] data_out);
    
    reg [7:0] mem[63:0][63:0];
    wire [5:0] r_index;
    wire [5:0] r_offset;
    wire [5:0] w_index;
    wire [5:0] w_offset;

    assign r_offset = r_addr[5:0];
    assign r_index  = r_addr[11:6];
    assign w_offset = w_addr[5:0];
    assign w_index  = w_addr[11:6];

    generate
        for(genvar idx= 0;idx<O_WIDTH/8;idx = idx+1)
            assign data_out[idx*8+:8] = mem[r_index][r_offset+idx[5:0]];
    endgenerate

    always_ff @(posedge clk) begin
        if (wen) begin
            // at least write one byte
            mem[w_index][w_offset] <= data_in[7:0];


            // if write mask >= 2 , write at least one more byte  
            if(write_mask >= 2) begin
                mem[w_index][w_offset + 1] <= data_in[15:8];        
            end

            // if write mask >= 4 , write at least two more bytes 
            if(write_mask >= 4) begin
                mem[w_index][w_offset + 2] <= data_in[23:16];
                mem[w_index][w_offset + 3] <= data_in[31:24];
            end

            // if write mask == 8 , write remaining 4 bytes
            if(write_mask == 8) begin        
                mem[w_index][w_offset + 4] <= data_in[39:32];        
                mem[w_index][w_offset + 5] <= data_in[47:40];        
                mem[w_index][w_offset + 6] <= data_in[55:48];        
                mem[w_index][w_offset + 7] <= data_in[63:56];        
            end
        end
    end
endmodule

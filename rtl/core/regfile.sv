module regfile (input clk,
                input rst,
                input [63:0] wdata,
                input [4:0] waddr,
                input wen,
                input [4:0] r_addr1,
                input [4:0] r_addr2,
                output reg [63:0] r_out1,
                output reg [63:0] r_out2);
    
    reg [63:0] registers [31:0];

    assign r_out1 = r_addr1 == 0 ? 0 : r_addr1 == waddr ? wdata : registers[r_addr1];
    assign r_out2 = r_addr2 == 0 ? 0 : r_addr2 == waddr ? wdata : registers[r_addr2];

    always_ff @(posedge clk) begin
        if (rst) begin
            for (integer i=0; i<32; i++) begin
                registers[i] <= 0;
            end
        end
        if (wen) begin
            registers[waddr] <= wdata;
        end
    end

endmodule
module pc #(parameter WIDTH = 32)
           (input clk,
            input rst,
            input branch,
            input [WIDTH-1:0] immediate,
            output reg [WIDTH-1:0] pc_out_reg,
            output reg out_valid);
    
    always @(posedge clk) begin
        if (rst == 1) begin
            pc_out_reg <= 0;
            out_valid  <= 1;
            end else begin
            if (branch == 1) begin
                pc_out_reg <= immediate;
                out_valid  <= 1;
                end else begin
                pc_out_reg <= pc_out_reg + 1;
                out_valid  <= 1;
            end
        end
    end
endmodule

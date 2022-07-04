module pc (input clk,
           input rst,
           input branch,
           input abs_branch,
           input bubble,
           input [63:0] ref_pc,
           input [63:0] immediate,
           output reg [63:0] pc_out_reg);
    
    always @(posedge clk) begin
        if (rst == 1) begin
            pc_out_reg <= 64'h0000_0000_8000_0000;
            end else begin
            if (branch == 1) begin
                pc_out_reg <= ref_pc + immediate;
            end
            else if (abs_branch == 1)begin
                pc_out_reg <= {immediate[63:1],1'b0};
            end
                else if (bubble == 1) begin
                pc_out_reg <= pc_out_reg;
                end
            else begin
                pc_out_reg <= pc_out_reg + 4;
            end
        end
    end
endmodule

module pc (input clk,
           input rst,
           input rel_branch,
           input abs_branch,
           input exception,
           input bubble,
           input [63:0] branch_base,
           input [63:0] immediate,
           input [63:0] mtvec,
           output reg [63:0] pc_out_reg);
    
    always @(posedge clk) begin
        if (rst == 1) begin
            pc_out_reg <= 64'h0000_0000_8000_0000;
            end else begin
            if (rel_branch == 1 && exception == 0) begin
                pc_out_reg <= branch_base + immediate;
            end
            else if (abs_branch == 1 && exception == 0)begin
                pc_out_reg <= {immediate[63:0]};
            end
            else if (bubble == 1 && exception == 0) begin
                pc_out_reg <= pc_out_reg;
            end
            else if (exception ==1) begin
                  pc_out_reg <= mtvec;
            end
            else begin
                pc_out_reg <= pc_out_reg + 4;
            end
        end
    end
endmodule

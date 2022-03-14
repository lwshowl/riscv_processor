module pc #(parameter WIDTH = 32)
           (input clk,
           /* verilator lint_off UNUSED */
            input r_clk,
            /* verilator lint_off UNUSED */
            input w_clk,
            input rst,
            input branch,
            input abs_branch,
            input [WIDTH-1:0] immediate,
            output reg [WIDTH-1:0] pc_out_reg);
            
    initial begin
        pc_out_reg = 0;
    end
    
    reg branch_r;
    reg abs_branch_r;
    
    always @(negedge w_clk) begin
        if (rst == 1) begin
            pc_out_reg <= 0;
            end else begin
            if (branch_r == 1) begin
                pc_out_reg <= pc_out_reg + immediate;
                end else if (abs_branch_r == 1)begin
                pc_out_reg <= {immediate[31:1],1'b0};
                end else
                begin
                pc_out_reg <= pc_out_reg + 4;
            end
        end
    end
    
    always @(clk) begin
        branch_r     = branch;
        abs_branch_r = abs_branch;
    end
    
endmodule

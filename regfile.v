module regfile #(parameter WIDTH = 32)
                (input r_clk,
                 input w_clk,
                 input r_enable,
                 input w_enable,
                 input [4:0]rs1_select,
                 input [4:0]rs2_select,
                 input [4:0]rd_select,
                 input [WIDTH-1:0]w_val,
                 output [WIDTH-1:0]rs1_out,
                 output [WIDTH-1:0]rs2_out);
    
    
    //存储信号寄存器
    reg  [4:0] rs1_select_reg;
    reg  [4:0] rs2_select_reg;
    reg  [4:0] rd_select_reg;
    reg  [WIDTH-1:0] w_val_reg;
    /* verilator lint_off UNUSED */
    reg  r_enable_reg;
    /* verilator lint_off UNUSED */
    reg  w_enable_reg;
    
    reg [WIDTH-1:0] r_file [0:31];
    
    initial begin
        r_file[0] = 0;
    end
    
    //存储上升沿上的信号
    always @(posedge r_clk) begin
        rs1_select_reg <= rs1_select;
        rs2_select_reg <= rs2_select;
        rd_select_reg  <= rd_select;
        w_val_reg      <= w_val;
        r_enable_reg   <= r_enable;
        w_enable_reg   <= w_enable;
    end
    
    //写
    always @(negedge w_clk) begin
        if (w_enable_reg)begin
            r_file[rd_select_reg] <= w_val_reg;
        end
    end
    
    assign rs1_out = r_file[rs1_select_reg];
    assign rs2_out = r_file[rs2_select_reg];
endmodule

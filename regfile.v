module regfile #(parameter WIDTH = 32)
                (input clk,
                 input r_enable,
                 input w_enable,
                 input [$clog2(WIDTH)-1:0]r_select, 
                 input [$clog2(WIDTH)-1:0]w_select, 
                 input [WIDTH-1:0]w_val, 
                 output [WIDTH-1:0]r_out, output valid);
    
    
    //存储信号寄存器
    reg  [$clog2(WIDTH)-1:0] r_select_reg;
    reg  [$clog2(WIDTH)-1:0] w_select_reg;
    reg  [WIDTH-1:0] w_val_reg;
    reg  [WIDTH-1:0] r_out_reg;
    reg  valid_reg;
    reg  r_enable_reg;
    reg  w_enable_reg;
    
    reg [WIDTH-1:0] r_file [0:31];
    
    
    //存储上升沿上的信号
    always @(posedge clk) begin
        r_select_reg <= r_select;
        w_select_reg <= w_select;
        w_val_reg    <= w_val;
        r_enable_reg <= r_enable;
        w_enable_reg <= w_enable;
    end
    
    //读写
    always @(posedge clk) begin
        if (r_enable_reg == 1) begin
            r_out_reg <= r_file[r_select_reg];
            valid_reg <= 1;
            end else if (w_enable_reg == 1) begin
            r_file[w_select_reg] <= w_val_reg;
            valid_reg            <= 1;
        end
    end
    
    assign r_out = r_out_reg;
    assign valid = valid_reg;
endmodule

module clock_divider #(parameter divider = 100,
                       parameter ratio1 = 0.5,
                       parameter ratio2 = 0.5)
                      (input clk_in,
                       output reg clk_out1,
                       output reg clk_out2);
    
    reg [31:0] counter;
    
    initial begin
        counter = 0;
    end
    
    always @(posedge clk_in) begin
        counter <= counter + 32'b1;
        if (counter > divider -2) begin
            counter <= 32'd0;
        end
        clk_out1 <= (counter < divider/2) ? 1'b1 : 1'b0;
        clk_out2 <= (counter < (divider/4)*3) ? 1'b1 : 1'b0;
    end
endmodule

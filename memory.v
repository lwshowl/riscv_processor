module memory #(parameter SIZE = 256,
                parameter WIDTH = 32)
               (input clk,
                input w_enable,
                input r_enable,
                input [31:0] addr_select,
                input [WIDTH - 1:0] data_in,
                output [WIDTH-1:0] data_out);
    
    
    parameter MEM_INIT_FILE = "test.txt";
    
    //memory
    reg [7:0] mem [0:SIZE-1];
    
    //输入寄存
    reg w_enable_r;
    /* verilator lint_off UNUSED */
    reg r_enable_r;
    /* verilator lint_off UNUSED */
    reg [31:0] addr_select_r;
    reg [WIDTH-1:0] data_in_r;
    
    
    initial begin
        /* verilator lint_off WIDTH */
        if (MEM_INIT_FILE != "") begin
            /* verilator lint_off WIDTH */
            $readmemb(MEM_INIT_FILE, mem);
        end
    end
    
    //寄存所有输入信号
    always @(posedge clk) begin
        w_enable_r    <= w_enable;
        r_enable_r    <= r_enable;
        addr_select_r <= addr_select;
        data_in_r     <= data_in;
    end
    
    //输出内容
    always @(posedge clk) begin
        if (w_enable_r) begin
            mem[addr_select_r]    <= data_in_r[7:0];
            mem[addr_select_r +1] <= data_in_r[15:8];
            mem[addr_select_r +2] <= data_in_r[23:16];
            mem[addr_select_r +3] <= data_in_r[31:24];
        end
    end
    
    assign data_out = {mem[addr_select_r+3],mem[addr_select_r+2],mem[addr_select_r+1],mem[addr_select_r]};
endmodule

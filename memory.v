module memory #(parameter SIZE = 1048576,
                parameter WIDTH = 32)
               (input clk,
                input w_clk,
                input w_enable,
                input r_enable,
                input [31:0] r_addr_select1,
                input [31:0] r_addr_select2,
                input [31:0] w_addr_select,
                input [WIDTH - 1:0] data_in,
                output [WIDTH-1:0] data_out1,
                output [WIDTH-1:0] data_out2);
    
    
    //memory
    reg [7:0] cells [0:SIZE-1];
    
    //输入寄存
    reg w_enable_r;
    /* verilator lint_off UNUSED */
    reg r_enable_r;
    /* verilator lint_off UNUSED */
    reg [31:0] r_addr_select1_r;
    reg [31:0] r_addr_select2_r;
    reg [WIDTH-1:0] data_in_r;
    reg [31:0] w_addr_select_r;
    
    parameter FILE = "./verify/tinyrv32.hex";
    
    
    // export "DPI" task apb_write;//这里的只能够在program里面进行使用
    // task apb_write(input int addr,data);
    //     bus_sem.get(1);
    //     #10 $display("VLOG : APB Write :Addr = %x,Data = %x",addr,data);
    //     bus_sem.put(1);
    // endtask
    
    initial begin
        /* verilator lint_off WIDTH */
        if (FILE != "") begin
            /* verilator lint_off WIDTH */
            $readmemh(FILE, cells);
            end else begin
            $display("file not exist");
        end
    end
    
    export "DPI-C" task initmem;
    task initmem(input string file);
        /* verilator lint_off WIDTH */
        if (file != "") begin
            /* verilator lint_off WIDTH */
            $readmemh(file, cells);
            end else begin
            $display("file not exist");
        end
    endtask
    
    //寄存所有输入信号
    always @(posedge clk) begin
        w_enable_r       <= w_enable;
        r_enable_r       <= r_enable;
        r_addr_select1_r <= r_addr_select1;
        r_addr_select2_r <= r_addr_select2;
        data_in_r        <= data_in;
        w_addr_select_r  <= w_addr_select;
    end
    
    //写入内容
    always @(negedge w_clk) begin
        if (w_enable_r) begin
            cells[w_addr_select_r]    <= data_in_r[7:0];
            cells[w_addr_select_r +1] <= data_in_r[15:8];
            cells[w_addr_select_r +2] <= data_in_r[23:16];
            cells[w_addr_select_r +3] <= data_in_r[31:24];
        end
    end
    
    assign data_out1 = {cells[r_addr_select1+3],cells[r_addr_select1+2],cells[r_addr_select1+1],cells[r_addr_select1]};
    assign data_out2 = {cells[r_addr_select2+3],cells[r_addr_select2+2],cells[r_addr_select2+1],cells[r_addr_select2]};
    
endmodule

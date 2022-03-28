module board(input clk,
            /* verilator lint_off UNUSED */
             input rst,
             /* verilator lint_off UNUSED */
             output [7:0] led);
    
    /* verilator lint_off UNUSED */
    wire [31:0] m_addr;
    wire [31:0] m_data;
    reg  [7:0] led_r;
    wire wea;
    /* verilator lint_off UNUSED */
    
    tinyrv32 rv32(
    .clk (clk),
    .rst (0),    
    .m_addr (m_addr),    
    .m_data (m_data),
    .wea (wea)
    );  
        
    always @(posedge clk) begin   
        led_r[0] <= ((m_addr == 32'h3000) & wea) ? m_data[0]:led_r[0];
        led_r[1] <= ((m_addr == 32'h3004) & wea) ? m_data[0]:led_r[1];
        led_r[2] <= ((m_addr == 32'h3008) & wea) ? m_data[0]:led_r[2];
        led_r[3] <= ((m_addr == 32'h300C) & wea) ? m_data[0]:led_r[3];  
        led_r[4] <= ((m_addr == 32'h3010) & wea) ? m_data[0]:led_r[4];
        led_r[5] <= ((m_addr == 32'h3014) & wea) ? m_data[0]:led_r[5];
        led_r[6] <= ((m_addr == 32'h3018) & wea) ? m_data[0]:led_r[6];
        led_r[7] <= ((m_addr == 32'h301C) & wea) ? m_data[0]:led_r[7];   
    end
    
    assign led = led_r;  
    
endmodule  

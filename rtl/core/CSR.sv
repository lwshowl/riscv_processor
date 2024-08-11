`include "instructions.sv"
module CSR(input clk,
           input rst,
           input wen,
           input excep_wen,
           input [63:0] rs1,
           input [11:0] addr,
           input [6:0] instr,
           input [63:0] mepc_overri,
           input [63:0] mcause_overri,
           /* verilator lint_off UNUSED */
           input [4:0] imm,
           /* verilator lint_off UNUSED */
           output reg [63:0] mepc_val,
           output reg [63:0] mtvec_val,
           output reg [63:0] csrVal);
    
    reg [63:0] mtvec;
    reg [63:0] mstatus;
    reg [63:0] mepc;
    reg [63:0] mcause;
    
    assign mtvec_val = mtvec;
    assign mepc_val = mepc;

    always_ff @(posedge clk) begin
        if(rst) begin
          mtvec     <= 0;
          mstatus   <= 64'h0000_000a_0000_1800;
          mcause    <= 0;
          mepc      <= 64'h0000_0000_8000_0000;
        end 
        else if(excep_wen) begin
            mepc    <= mepc_overri;
            mcause  <= mcause_overri;
        end
        else if (wen) begin
            if (addr == 12'h305) begin
                csrVal  <= mtvec;
                mtvec   <= instr == `i_csrrs ? mtvec | rs1 : rs1;
            end
            else if (addr == 12'h342) begin
                csrVal  <= mcause;
                mcause  <= instr == `i_csrrs ? mcause | rs1 : rs1;
            end
        end
    end
endmodule

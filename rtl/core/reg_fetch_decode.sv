module reg_fetch_decode (
    input clk,
    input rst,
    input enable,
    input [63:0] pc_from_fetch,
    input [31:0] instr_from_fetch,
    input [15:0] exception_from_fetch,
    output reg [63:0] pc_to_decode,
    output reg [31:0] instr_to_decode,
    output reg [15:0] exception_to_decode
);
    always_ff @(posedge clk) begin
        if (enable) begin
            pc_to_decode <= pc_from_fetch;
            instr_to_decode <= instr_from_fetch;
            exception_to_decode <= exception_from_fetch;
        end else if (rst) begin
            pc_to_decode <= 0;
            instr_to_decode <= 0;
            exception_to_decode <= 0;
        end
    end
endmodule


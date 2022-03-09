/*
 * Primitive alu example for Verilator example
 *
 * File name: alu.sv
 * Author: Norbertas Kremeris 2021
 *
 */


module alu #(parameter WIDTH = 32)
            (input clk,
             input rst,
             input [1:0] op_in,
             input [WIDTH-1:0] a_in,
             input [WIDTH-1:0] b_in,
             input in_valid,
             output [WIDTH-1:0] out,
             output out_valid);
    
    reg  [WIDTH-1:0]  a_in_r;
    reg  [WIDTH-1:0]  b_in_r;
    reg               in_valid_r;
    reg  [WIDTH-1:0]  result;
    reg  [1:0] op_in_r;
    reg   done;
    
    localparam  add = 1;
    localparam  sub = 2;
    localparam  nop = 0;
    
    // Register all inputs
    always @ (posedge clk, posedge rst)
    begin
        if (rst == 1)
        begin
            op_in_r    <= '0;
            a_in_r     <= '0;
            b_in_r     <= '0;
            in_valid_r <= '0;
        end
        else
        begin
            op_in_r    <= op_in;
            a_in_r     <= a_in;
            b_in_r     <= b_in;
            in_valid_r <= in_valid;
            done       <= 0;
            
            if (in_valid_r == 1)
            begin
                case (op_in_r)
                    add: result     <= a_in_r + b_in_r;
                    sub: result     <= a_in_r + (~b_in_r+1'b1);
                    default: result <= '0;
                endcase
                done <= 1;
            end
            else
            begin
                result <= '0;
            end
            
        end
    end
    
    assign out       = result;
    assign out_valid = done;
    
    endmodule;

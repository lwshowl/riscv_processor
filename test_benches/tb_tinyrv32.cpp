#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtinyrv32.h"

#define MAX_SIM_TIME 100
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
vluint64_t i = 0;

int main(int argc, char **argv, char **env)
{
    Vtinyrv32 *dut = new Vtinyrv32;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");
  
    while (sim_time < MAX_SIM_TIME)
    {
        dut->clk ^= 1;
        dut->eval();
        if (dut->clk == 1)  
        {
            posedge_count++;

            // dut->tinyrv32__DOT__imem_r_enable = 1;
            // dut->tinyrv32__DOT__imm_select = 0;
        }

        m_trace->dump(sim_time);
        sim_time++;
    }  
    m_trace->close();
    delete dut;      
    exit(EXIT_SUCCESS); 
}    
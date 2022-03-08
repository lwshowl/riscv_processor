#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vregfile.h"

#define MAX_SIM_TIME 20
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
vluint64_t i = 0;

int main(int argc, char **argv, char **env)
{
    Vregfile *dut = new Vregfile;

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
            std::cout << "reg: " << dut->r_out << std::endl;
            dut->r_enable = 1;
            dut->w_enable = 0;
            dut->r_select = 6;
            if (posedge_count == 1)
            {
                dut->r_enable = 0;
                dut->w_enable = 1;
                dut->w_select = 6;
                dut->w_val = 123;
            }
        }

        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vpc.h"
#include "Vpc__Syms.h"

#define MAX_SIM_TIME 100
vluint64_t sim_time = 0;

int main(int argc, char **argv, char **env)
{
    Vpc *dut = new Vpc;

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
            dut->rst = 0;
            dut->branch = 0;
            dut->immediate = 0;
            std::cout << dut->pc_out_reg << std::endl;
            if (sim_time == 10)
            {
                dut->rst = 1;
            }

            if (sim_time == 50)
            {
                dut->branch = 1;
                dut->immediate = 12345;
            }
        }
        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtinyrv32.h"

#define MAX_SIM_TIME 1000
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
vluint64_t i = 0;
vluint64_t fetch_count = 0;
vluint64_t pos_fetch_times = 0;

#define N 5
int last_val;

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
        if (dut->tinyrv32__DOT__fetch_clk == 1)
        {
            if (fetch_count > pos_fetch_times)
            {
                std::cout << "pc:\t" << (int)dut->tinyrv32__DOT__pc_out << "\t";
                for (i = 0; i < 31; i++)
                {
                    std::cout << "x" << i << ":\t";
                    std::cout << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[i] << "\t";
                }
                std::cout << "imm:\t" << (int)dut->tinyrv32__DOT__alu32__DOT__imm_r << "\t";
                std::cout << std::endl << std::endl;
            }
            pos_fetch_times = fetch_count;
        }
        else
        {
            fetch_count++;
        }

        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
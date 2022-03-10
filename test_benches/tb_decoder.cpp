#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vdecoder.h"

#define MAX_SIM_TIME 50
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
vluint64_t i = 0;

void decoderReset(Vdecoder *dut)
{
    dut->instr = 0;
}

int main(int argc, char **argv, char **env)
{
    Vdecoder *dut = new Vdecoder;

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
            std::cout << "instr_id: " << std::dec << (int)dut->instr_id << std::endl
                      << "rs1: " << (int)dut->rs1 << std::endl
                      << "rs2: " << (int)dut->rs2 << std::endl
                      << "rd: " << (int)dut->rd << std::endl
                      << "imm: " << (int)dut->imm << std::endl;
            if (posedge_count == 1)
            {
                dut->instr = 0b11000111111011110100111011100011;
            }
        }

        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
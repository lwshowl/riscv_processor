#include "instructions.h"
#include "inc/core_dump.h"
#include "simulator/inc/axi_slave_mem.h"
using namespace std;

#define MAX_SIM_TIME -1
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
uint64_t dnpc_at_commit;

uint64_t run_once()
{
    uint64_t pc;
    while (1)
    {
        pc = is_committing();
        if (sim_time < MAX_SIM_TIME && !pc)
        {
            dut->clk ^= 1;
            mem_sigs.update_input(mem_ref);
            dut->eval();
            mem.beat(mem_sigs_ref);
            mem_sigs.update_output(mem_ref);
            sim_time++;
            // dut-clk == 1 , 刚刚有一个上升沿，所有信息被写入寄存器
            if (dut->clk == 1)
            {
                posedge_count++;
                update_dnpc();
                //dump_wb();
            }
        }
        else
            break;
    }

    if ((dut->core__DOT__wb_branch_out && dut->core__DOT__wb_result_out) | dut->core__DOT__wb_instrId_out == i_jalr | dut->core__DOT__wb_instrId_out == i_mret | dut->core__DOT__dmem_excep_out > 0 | dut->core__DOT__wb_instrId_out == i_ecall)
    {
        if (!dnpc_queue.empty())
        {
            dnpc_at_commit = dnpc_queue.front();
            dnpc_queue.pop();
        }
    }
    else
        dnpc_at_commit = 0;

    // commit
    dut->clk ^= 1;
    dut->eval();
    if (dut->clk == 1)
    {
        //dump_wb();
        update_dnpc();
    }
    dut->clk ^= 1;
    dut->eval();
    if (dut->clk == 1)
    {
        //dump_wb();
        update_dnpc();
    }
    return pc;
}
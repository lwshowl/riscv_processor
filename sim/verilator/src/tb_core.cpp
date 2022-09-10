#include "inc/instructions.h"
#include "inc/tb_core.h"
#include "inc/core_dump.h"
using namespace std;

Vcore *dut = new Vcore;
#define MAX_SIM_TIME -1

uint64_t core_run_once()
{
    uint64_t pc;
    while (1)
    {
        pc = is_committing();
        if (sim_time < MAX_SIM_TIME && !pc)
        {
            // 上升沿以前
            if (dut->clk == 0)
            {
                // 上升沿以前 通知slave
                mem_sigs.update_input(*mem_ref);
                // 产生上升沿
                dut->clk ^= 1;
                dut->eval();

                // 打印必要信息
                dump_decode();
                // dump_axi_ctl();
                // dump_axi();
                getchar();
                update_dnpc();
            }
            dump_icache();

            // 上升沿已经发生
            if (dut->clk == 1)
            {
                //产生下降沿
                dut->clk ^= 1;
                dut->eval();
                //下降沿之后 获取slave 的输出，以便下一个上升沿使用
                mem.beat(mem_sigs_ref);
                mem_sigs.update_output(*mem_ref);
            }

            sim_time++;
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
        update_dnpc();
    }
    dut->clk ^= 1;
    dut->eval();
    if (dut->clk == 1)
    {
        update_dnpc();
    }
    return pc;
}
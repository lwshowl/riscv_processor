#include "include/instructions.h"
#include "include/tb_core.h"
#include "include/core_dump.h"
#include <map>

using namespace std;
#define MAX_SIM_TIME -1

void core_pass_registers(uint64_t *regs)
{
    for (int i = 0; i < 32; i++)
        regs[i] = dut->core__DOT__registerFile__DOT__registers[i];
}

void dump()
{
    // dump_wb();
    // dump_axi_ctl();
    // dump_axi();
    // dump_icache();
    // dump_pc();
    // dump_icache();
    // dump_decode();
    // dump_regfile();
    // dump_alu();
    // dump_dcache();
    // dump_axi_ctl();
    // dump_wb();
    // cout << endl;
}

void commit()
{
    if (dut->clk == 0)
    {
        // 上升沿以前 通知slave
        mem_sigs.update_input(*mem_ref);
        // 产生上升沿
        dut->clk ^= 1;
        dut->eval();
        dump();
        update_dnpc();
    }
    else
    {
        //产生下降沿
        dut->clk ^= 1;
        dut->eval();
        //下降沿之后 获取slave 的输出，以便下一个上升沿使用
        mem.beat(mem_sigs_ref);
        mem_sigs.update_output(*mem_ref);

        // 上升沿以前 通知slave
        mem_sigs.update_input(*mem_ref);
        // 产生上升沿
        dut->clk ^= 1;
        dut->eval();
        dump();
        update_dnpc();
    }
}

std::map<uint64_t, uint64_t> imap;
uint64_t core_run_once()
{
    uint64_t pc;
    while (1)
    {

        pc = is_committing();
        auto it = imap.find((uint64_t)dut->core__DOT__decode_instr_out);
        if (it != imap.end())
            it->second = dut->core__DOT__decode_instr_out;
        else
            imap.insert(make_pair((uint64_t)(dut->core__DOT__decode_pc_out),
                                  (uint64_t)(dut->core__DOT__decode_instr_out)));

        if (sim_time < MAX_SIM_TIME && !pc)
        {
            // 上升沿以前,before write
            if (dut->clk == 0)
            {
                // 上升沿以前 通知slave
                mem_sigs.update_input(*mem_ref);
                // 产生上升沿
                dut->clk ^= 1;
                dut->eval();
                dump();
                update_dnpc();
            }
            // 上升沿已经发生,after write , update value
            if (dut->clk == 1)
            {
                //产生下降沿
                dut->clk ^= 1;
                dut->eval();
                //下降沿之后 获取slave 的输出，以便下一个上升沿使用
                mem.beat(mem_sigs_ref);
                mem_sigs.update_output(*mem_ref);
                update_dnpc();
            }

            sim_time++;
        }
        else
            break;
    }

    // check if there is branch , and get the branch target
    if (is_branch())
    {
        if (!dnpc_queue.empty())
        {
            dnpc_at_commit = dnpc_queue.front();
            dnpc_queue.pop();
        }
    }
    else
        dnpc_at_commit = pc + 4;

    commit();

    return dnpc_at_commit;
}
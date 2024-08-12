#include "include/instructions.h"
#include "include/core_dump.h"
#include "include/core_module.h"
#include <map>

using namespace std;

vluint64_t sim_time = 0;
std::queue<uint64_t> dnpc_queue;
std::map<uint64_t, uint64_t> imap;
Vcore *dut = new Vcore;

void core_pass_registers(uint64_t *regs)
{
    for (int i = 0; i < 32; i++)
        regs[i] = dut->core__DOT__registerFile__DOT__registers[i];
}

void core_pass_instr()
{
    auto it = imap.find((uint64_t)dut->core__DOT__decode_pc);
    if (it != imap.end())
        it->second = dut->core__DOT__decode_instr;
    else
        imap.insert(make_pair((uint64_t)(dut->core__DOT__decode_pc),
                              (uint64_t)(dut->core__DOT__decode_instr)));
}

uint64_t core_run_once()
{
    uint64_t pc;
    while (1)
    {
        pc = is_committing();
        core_pass_instr();

        if (sim_time < MAX_SIM_TIME && pc == 0)
        {
            // 上升沿以前, before write
            handleRisingEdge();
            // after write, update value
            handleFallingEdge();
            sim_time++;
            continue;
        } else {
            break;
        }
    }
    // check if there is branch , and get the branch target
    return commit_instr();
}
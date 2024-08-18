#include "include/instructions.h"
#include "include/core_dump.h"
#include "include/core_interface.h"

void handleRisingEdge()
{
    // 上升沿以前,before write
    if (dut->clk == 0)
    {
        step_verilator_sim();
        mem_sigs.update_input(*mem_ref);
        update_dnpc();
    }
}

void handleFallingEdge()
{
    if (dut->clk == 1)
    {
        step_verilator_sim();
        mem.beat(mem_sigs_ref);
        mem_sigs.update_output(*mem_ref);
        dump_internals();
        // 下降沿之后 获取slave 的输出，以便下一个上升沿使用
    }
}

void step_verilator_sim()
{
    dut->clk ^= 1;
    dut->eval();
}

void dump_internals()
{
    // dump_wb();
    // dump_axi_ctl();
    // dump_axi();
    // dump_pc();
    // dump_icache();
    // dump_decode();
    // dump_regfile();
    // dump_alu();
    // dump_dmem();
    // dump_dcache();
    // dump_axi_ctl();
    // dump_wb();
    // std::cout << std::endl;
}

uint64_t commit_instr()
{
    uint64_t pc = next_pc();
    if (dut->clk == 0)
    {
        // 上升沿以前 通知slave
        handleRisingEdge();
    }
    else
    {
        handleFallingEdge();
        handleRisingEdge();
    }
    return pc;
}

uint64_t next_pc()
{
    uint64_t next_pc;
    if (is_branch())
    {
        if (!dnpc_queue.empty())
        {
            next_pc = dnpc_queue.front();
            dnpc_queue.pop();
        }
    }
    else
    {
        next_pc = ((uint64_t)dut->core__DOT__wb_pc) + 4u;
    }
    return next_pc;
}
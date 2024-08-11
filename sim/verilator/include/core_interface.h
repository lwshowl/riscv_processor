#pragma once
#include <stdint.h>
#include <Vcore.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "instructions.h"
#include "socsim/include/axi_slave_mem.h"

void handleRisingEdge();
void handleFallingEdge();
void step_verilator_sim();
void dump_internals();
uint64_t commit_instr();
uint64_t next_pc();

extern axi4_mem<64, 64, 4> mem;
extern axi4_ptr<64, 64, 4> mem_ptr;
extern axi4_ref<64, 64, 4> *mem_ref;
extern axi4<64, 64, 4> mem_sigs;
extern axi4_ref<64, 64, 4> mem_sigs_ref;

extern Vcore *dut;

bool static inline is_branch()
{
    return ((dut->core__DOT__wb_branch && dut->core__DOT__wb_result) // if there is branch
              | dut->core__DOT__wb_instrId == i_jalr | dut->core__DOT__wb_instrId == i_mret | dut->core__DOT__wb_instrId == i_ecall);
}

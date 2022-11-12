#pragma once
#include <stdint.h>
#include "verilator.h"

uint64_t is_committing();
void dump_decode();
void dump_fetch();
void dump_alu();
void dump_wb();
void dump_dmem();
void dump_regfile();
int sanity_check();
void dump_pc();
void dump_csr();
void dump_icache();
void update_dnpc();
void dump_axi();
void dump_axi_ctl();
void dump_dcache();
extern "C" void npc_reset();
extern "C" void npc_dump_registers(uint64_t *regs);
extern "C" void invalid_inst(uint64_t addr);

extern vluint64_t sim_time;
extern vluint64_t posedge_count;
extern Vcore *dut;
extern std::queue<uint64_t> dnpc_queue;
extern uint64_t dnpc_at_commit;

static const char *reg_name[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
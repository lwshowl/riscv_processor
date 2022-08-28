#pragma once
#include "sim_conf.h"

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
void update_dnpc();
extern "C" uint64_t run_once();
extern "C" void npc_reset();
extern "C" void npc_dump_registers(uint64_t *regs);
extern "C" void invalid_inst(uint64_t addr);

std::queue<uint64_t> dnpc_queue;
uint64_t dnpc_at_commit;

static const char *reg_name[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
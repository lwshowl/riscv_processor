#pragma once
#include <stdint.h>
#include "instructions.h"
#include <verilated.h>
#include <Vcore.h>
#include <verilated_vcd_c.h>
#include "socsim/include/axi_slave_mem.h"

uint64_t run_once();
vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
std::queue<uint64_t> dnpc_queue;
uint64_t dnpc_at_commit;

extern axi4_mem<64, 64, 4> mem;
extern axi4_ptr<64, 64, 4> mem_ptr;
extern axi4_ref<64, 64, 4> *mem_ref;
extern axi4<64,64,4> mem_sigs;
extern axi4_ref<64,64,4> mem_sigs_ref;


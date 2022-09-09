#pragma once
#include "axi_slave_mem.h"

axi4_mem<64, 64, 4> mem(4096l * 1024 * 1024);
axi4_ptr<64, 64, 4> mem_ptr;
axi4_ref<64, 64, 4> *mem_ref;
axi4<64,64,4> mem_sigs;
axi4_ref<64,64,4> mem_sigs_ref(mem_sigs);

void axi_mem_connect(axi4_ptr<64,64,4> &ptr);
extern Vcore *dut;
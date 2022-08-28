#pragma once
#include "bus/axi4.hpp"
#include "bus/axi4_slave.hpp"
#include "bus/axi4_mem.hpp"

axi4_mem<64, 64, 4> mem(4096l * 1024 * 1024);
axi4_ptr<64, 64, 4> mem_ptr;
axi4_ref<64, 64, 4> mem_ref(mem_ptr);
axi4<64,64,4> mem_sigs;
axi4_ref<64,64,4> mem_sigs_ref(mem_sigs);
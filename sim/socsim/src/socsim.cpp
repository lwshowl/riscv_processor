#include <iostream>
#include "inc/axi_slave_mem.h"
#include "inc/socsim.h"
#include <bitset>

#define bin_path "../cpu_test/build/add-riscv64-nemu.bin"

extern uint64_t run_once();
extern void core_reset();
using namespace std;

int main()
{
    // init axi slave memory
    mem.load_binary(bin_path, 0x80000000);
    axi_mem_connect(mem_ptr);
    mem_ref = new axi4_ref<64,64,4>(mem_ptr);
    // init cpu core
    core_reset();
    run_once();
}
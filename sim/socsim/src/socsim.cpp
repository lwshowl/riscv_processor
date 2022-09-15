#include <iostream>
#include "include/axi_slave_mem.h"
#include "include/socsim.h"
#include "include/difftest.h"
#include "include/monitor.h"

#include <fstream>

static int diff_test_port = 1234;

extern uint64_t core_run_once();
extern void core_dump_registers(uint64_t *reg);
extern void core_reset();
void execute_and_monitor();
using namespace std;

int main()
{
    // init axi slave memory
    mem.load_binary(IMAGE, 0x80000000);
    axi_mem_connect(mem_ptr);
    mem_ref = new axi4_ref<64, 64, 4>(mem_ptr);


    // init cpu core
    core_reset();
    cpu.pc = RESET_VECTOR;

    // init difftest
    difftest_init(DIFF_TEST_SO, mem.get_prog_size(), diff_test_port);
    
    welcome();

    // executione
    execute_and_monitor();
}

void execute_and_monitor()
{
    string command;
    uint64_t cur_pc;
    do
    {
        cout << "(soc): ";
        cin >> command;
        if (command == "s")
        {
            cur_pc = cpu.pc;
            cpu.pc = core_run_once(); // next pc
            core_dump_registers(cpu.gpr);
            difftest_step(cur_pc, cpu.pc);
        }
        else if (command == "info")
        {
            isa_reg_display();
        }

    } while (true);
}
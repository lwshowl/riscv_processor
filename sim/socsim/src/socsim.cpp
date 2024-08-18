#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "include/axi_slave_mem.h"
#include "include/socsim.h"
#include "include/difftest.h"
#include "include/monitor.h"
#include "include/expr.h"
#include "include/utils.h"
#include "readline/readline.h"
#include "readline/history.h"
#include "bus/uartlite.hpp"
#include <map>

static int diff_test_port = 1234;

void core_reset();
extern "C" void init_disasm(const char *triple);
void execute_and_monitor();
uartlite uart;

axi4_mem<64, 64, 4> mem(4096l * 1024 * 1024);
axi4_ptr<64, 64, 4> mem_ptr;
axi4_ref<64, 64, 4> *mem_ref;
axi4<64,64,4> mem_sigs;
axi4_ref<64,64,4> mem_sigs_ref(mem_sigs);

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
    difftest_init(DIFF_TEST_SO, mem.get_progsize(), diff_test_port);

    // init disasm
    init_disasm("riscv64-pc-linux-gnu");

    // init sdb
    init_regex();

    welcome();

    // executione
    execute_and_monitor();
}

void execute_and_monitor()
{
    string command;
    do
    {
        command = readline("(soc): ");
        string delimiter = " ";
        vector<string> params = split(command, delimiter);
        if (command.size() > 0)
            add_history(command.c_str());

        if (COMMAND_STARTS_WITH("s"))
        {
            int times = params.size() == 2 ? atoi(params[1].c_str()) : 1;
            execute(times);
        }
        else if(COMMAND_STARTS_WITH("q")) 
        {
            return;
        }
        else if (COMMAND_STARTS_WITH("c"))
        {
            execute(-1);
        }
        else if (COMMAND_STARTS_WITH("info"))
        {
            isa_reg_display();
        }
        else if (COMMAND_STARTS_WITH("w"))
        {
            if (params.size() > 1)
                set_watch(params[1]);
        }
        else if (COMMAND_STARTS_WITH("x"))
        {
            int irange = params.size() > 2 ? atoi(params[2].c_str()) : 4;
            word_t start = expr((char *)params[1].c_str());

            if (start == 0xdeadbeef)
                continue;

            string ori;
            for (int i = 0; i < irange * 4; i += 16)
            {
                printf("0x%016lx: ", start + i);
                uint32_t word = (uint32_t)mem_read(start + i, 4, ori);
                std::cout << hex << setw(8) << setfill('0') << word << " ";
                word = (uint32_t)mem_read(start + i + 4, 4, ori);
                std::cout << hex << setw(8) << setfill('0') << word << " ";
                word = (uint32_t)mem_read(start + i + 8, 4, ori);
                std::cout << hex << setw(8) << setfill('0') << word << " ";
                word = (uint32_t)mem_read(start + i + 12, 4, ori);
                std::cout << hex << setw(8) << setfill('0') << word << " ";
                std::cout << " from " << ori;
                std::cout << endl;
            }
            std::cout << endl;
        } else {
            continue;
        }
    } while (true);
}
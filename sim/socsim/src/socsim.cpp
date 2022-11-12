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
#include <map>

static int diff_test_port = 1234;

uint64_t core_run_once();
void core_pass_registers(uint64_t *reg);
void core_reset();
extern "C" void init_disasm(const char *triple);
extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
void execute_and_monitor();

extern std::map<uint64_t, uint64_t> imap;

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
    uint64_t cur_pc;
    do
    {
        cout << "(soc): ";
        getline(std::cin, command);
        if (command == "s")
        {
            cur_pc = cpu.pc;
            cpu.pc = core_run_once(); // next pc
            core_pass_registers(cpu.gpr);

            uint8_t buffer[128];
            uint32_t instr = imap.at(cur_pc);
            uint8_t *inst = (uint8_t *)(&instr);
            uint8_t *p = buffer;
            for (int i = 0; i < 4; i++)
                p += snprintf((char *)p, 4, " %02x", inst[i]);

            memset(p++, ' ', 1);
            disassemble((char *)p, buffer + sizeof(buffer) - p, cur_pc, (uint8_t *)&instr, 4);
            printf("0x%x: %s\n", cur_pc, buffer);
            difftest_step(cur_pc, cpu.pc);
        }
        else if (command == "info")
        {
            isa_reg_display();
        }
        else if (COMMAND_STARTS_WITH("x"))
        {
            string delimiter = " ";
            vector<string> params = split(command, delimiter);
            int irange = params.size() > 2 ? atoi(params[2].c_str()) : 4;
            word_t start = expr((char *)params[1].c_str());

            printf("0x%016lx: ", start);
            string ori;
            for (int i = 0; i < irange * 4; i += 4)
            {
                uint32_t word = (uint32_t)mem_read(start + i, 4, ori);
                std::cout << hex << setw(8) << setfill('0') << word << " ";
            }
            std::cout << " from " << ori;
            printf("\n");
        }
    } while (true);
}
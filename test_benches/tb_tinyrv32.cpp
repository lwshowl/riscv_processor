#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtinyrv32.h"
#include "util.h"
#include "instructions.h"
#include "fstream"
#include <string>
#include "Vtinyrv32__Dpi.h"
#include "svdpi.h"
#include <iomanip>

#define MAX_SIM_TIME 100000

vluint64_t sim_time = 0;
vluint64_t posedge_count = 0;
vluint64_t i = 0;
vluint64_t fetch_count = 0;
vluint64_t pos_fetch_times = 0;
int flag = 0;

#define N 5
int last_val;

// extern void initmem(const char *in_bool);

std::string enum_to_ins(int id)
{
    std::string instr;
    switch (id)
    {
    case i_lui:
        instr = "i_lui";
        break;
    case i_auipc:
        instr = "i_auipc";
        break;
    case i_jal:
        instr = "i_jal";
        break;
    case i_jalr:
        instr = "i_jalr";
        break;
    case i_beq:
        instr = "i_beq";
        break;
    case i_bne:
        instr = "i_bne";
        break;
    case i_blt:
        instr = "i_blt";
        break;
    case i_bge:
        instr = "i_bge";
        break;
    case i_bltu:
        instr = "i_bltu";
        break;
    case i_bgeu:
        instr = "i_bgeu";
        break;
    case i_lb:
        instr = "i_lb";
        break;
    case i_lh:
        instr = "i_lh";
        break;
    case i_lw:
        instr = "i_lw";
        break;
    case i_lbu:
        instr = "i_lbu";
        break;
    case i_lhu:
        instr = "i_lhu";
        break;
    case i_sb:
        instr = "i_sb";
        break;
    case i_sh:
        instr = "i_sh";
        break;
    case i_sw:
        instr = "i_sw";
        break;
    case i_addi:
        instr = "i_addi";
        break;
    case i_slti:
        instr = "i_slti";
        break;
    case i_sltiu:
        instr = "i_sltiu";
        break;
    case i_xori:
        instr = "i_xori";
        break;
    case i_ori:
        instr = "i_ori";
        break;
    case i_andi:
        instr = "i_andi";
        break;
    case i_slli:
        instr = "i_slli";
        break;
    case i_srli:
        instr = "i_srli";
        break;
    case i_srai:
        instr = "i_srai";
        break;
    case i_add:
        instr = "i_add";
        break;
    case i_sub:
        instr = "i_sub";
        break;
    case i_sll:
        instr = "i_sll";
        break;
    case i_slt:
        instr = "i_slt";
        break;
    case i_sltu:
        instr = "i_sltu";
        break;
    case i_xor:
        instr = "i_xor";
        break;
    case i_srl:
        instr = "i_srl";
        break;
    case i_sra:
        instr = "i_sra";
        break;
    case i_or:
        instr = "i_or";
        break;
    case i_and:
        instr = "i_and";
        break;
    case i_ecall:
        instr = "i_ecall";
        break;
    case i_ebreak:
        instr = "i_ebreak";
        break;
    case i_csrrw:
        instr = "i_csrrw";
        break;
    case i_csrrs:
        instr = "i_csrrs";
        break;
    case i_csrrc:
        instr = "i_csrrc";
        break;
    case i_csrrwi:
        instr = "i_csrrwi";
        break;
    case i_csrrsi:
        instr = "i_csrrsi";
        break;
    case i_csrrci:
        instr = "i_csrrci";
        break;
    case i_invalid:
        instr = "i_invalid";
        break;
    }
    return instr;
}

int main(int argc, char **argv, char **env)
{
    Vtinyrv32 *dut = new Vtinyrv32;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");

    int i = 0;
    int trace = 1;
    int enrtry = 0;
    std::string command;
    int proceed = 1;

    while (sim_time < MAX_SIM_TIME)
    {
        dut->clk ^= 1;
        if (dut->tinyrv32__DOT__fetch_clk == 1)
        {
            if (dut->tinyrv32__DOT__pc_out >= enrtry)
            {
                if (fetch_count > pos_fetch_times)
                {
                    std::cout << "pc:" << std::hex << dut->tinyrv32__DOT__mem__DOT__r_addr_select1_r << " ";
                    // std::cout << "instr: " << std::hex << dut->tinyrv32__DOT__mem_data_out << " ";
                    std::cout << enum_to_ins(dut->tinyrv32__DOT__dec__DOT__instr_id_r) << " ";
                    std::cout << "ra: " << std::hex << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[1] << " ";
                    std::cout << "sp: " << std::hex << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[2]
                              << "(" << std::dec << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[2] << ") ";
                    std::cout << "s0: " << std::hex << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[8] << " ";
                    std::cout << std::endl;
                    for (i = 3; i < 32; i++)
                    {
                        std::cout << "x" << i << ":";
                        std::cout << std::dec << (int)dut->tinyrv32__DOT__rfile32__DOT__r_file[i] << " ";
                    }
                    std::cout << "imm:" << (int)dut->tinyrv32__DOT__alu32__DOT__imm_r << " ";
                    std::cout << std::endl;
                    std::string command;
                    proceed--;  

                    do
                    {
                        if (proceed > 0)
                            goto PROCEED;
                        std::cout << "command: ";
                        command.clear();
                        std::cin >> command;
                        std::cout << std::endl;
                        if (command.find_first_of("memdump") != std::string::npos)
                        {
                            int range = 0x50;
                            if (command[7])
                            {
                                range = std::atoi(command.substr(7, command.size() - 1).c_str());
                            }
                            for (int addr = 0x500 - range; addr <= 0x500; addr += 4)
                            {
                                CData *val = dut->tinyrv32__DOT__mem__DOT__cells + addr;
                                std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << ": "
                                          << std::setw(2) << std::setfill('0')
                                          << (int)*val
                                          << std::setw(2) << std::setfill('0')
                                          << (int)*(val + 1)
                                          << std::setw(2) << std::setfill('0')
                                          << (int)*(val + 2)
                                          << std::setw(2) << std::setfill('0')
                                          << (int)*(val + 3) << "  ";
                                std::cout << std::endl;
                            }
                        }

                        if (command.find_first_of("t") != std::string::npos)
                        {
                            if (command[1])
                            {
                                std::string iteration = command.substr(1, command.size() - 1);
                                proceed = std::atoi(iteration.c_str());
                            }
                            else
                                proceed++;
                        }

                        if (proceed > 0)
                            goto PROCEED;
                    } while (proceed == 0);
                }
            }
        PROCEED:
            pos_fetch_times = fetch_count;
        }
        else
        {
            fetch_count++;
        }

        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}
#pragma once
#include "axi_slave_mem.h"
#include "utils.h"
#include "common.h"

void axi_mem_connect(axi4_ptr<64,64,4> &ptr);
extern void difftest_init(char *ref_so_file, long img_size, int port);
extern Vcore *dut;
riscv64_CPU_state cpu;


static void welcome()
{
  Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to %s-SOC!\n", ASNI_FMT("riscv64", ASNI_FG_YELLOW ASNI_BG_RED));
  printf("For help, type \"help\"\n");
  // Log("Exercise: Please remove me in the source code and compile NEMU again.");
  // let go
  // assert(0);
}

#define COMMAND_STARTS_WITH(x) command.substr(0,command.find(" ")) == x
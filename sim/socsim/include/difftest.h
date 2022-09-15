#pragma once
#include "common.h"
#include <stdlib.h>
#include "utils.h"

enum
{
  DIFFTEST_TO_DUT,
  DIFFTEST_TO_REF
};

void difftest_skip_ref();
void difftest_skip_dut(int nr_ref, int nr_dut);
void difftest_set_patch(void (*fn)(void *arg), void *arg);
void difftest_step(vaddr_t pc, vaddr_t npc);
void difftest_detach();
void difftest_attach();

void difftest_init(char *ref_so_file, long img_size, int port);
bool isa_difftest_checkregs(riscv64_CPU_state *ref_r, vaddr_t pc);
static inline bool difftest_check_reg(const char *name, vaddr_t pc, word_t ref, word_t dut);
static inline const char *reg_name(int idx, int width);
void difftest_step(vaddr_t pc, vaddr_t npc);

extern riscv64_CPU_state cpu;

static inline bool difftest_check_reg(const char *name, vaddr_t pc, word_t ref, word_t dut)
{
  if (ref != dut)
  {
    Log("%s is different after executing instruction at pc = " FMT_WORD
        ", right = " FMT_WORD ", wrong = " FMT_WORD ", diff = " FMT_WORD,
        name, pc, ref, dut, ref ^ dut);
    return false;
  }
  return true;
}

static inline const char *reg_name(int idx, int width)
{
  extern const char *regs[];
  return regs[idx];
}
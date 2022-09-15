#include "include/difftest.h"
#include "include/utils.h"
#include "include/axi_slave_mem.h"
#include <dlfcn.h>
#include <fstream>
#include "assert.h"

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction);
void (*ref_difftest_regcpy)(void *dut, bool direction);
void (*ref_difftest_exec)(uint64_t n);
void (*ref_difftest_raise_intr)(uint64_t NO);

bool difftest_checkregs(riscv64_CPU_state *ref_r, vaddr_t pc)
{
  bool state = true;

  for (int i = 0; i <= 31 && state > 0; i++)
    state = difftest_check_reg(reg_name(i, 8), pc, ref_r->gpr[i], cpu.gpr[i]);

  return state;
}

void difftest_step(vaddr_t pc, vaddr_t npc)
{
  riscv64_CPU_state ref_r;

  // if (skip_dut_nr_inst > 0)
  // {
  //   ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);
  //   if (ref_r.pc == npc)
  //   {
  //     skip_dut_nr_inst = 0;
  //     checkregs(&ref_r, npc);
  //     return;
  //   }
  //   skip_dut_nr_inst--;
  //   if (skip_dut_nr_inst == 0)
  //     panic("can not catch up with ref.pc = " FMT_WORD " at pc = " FMT_WORD, ref_r.pc, pc);
  //   return;
  // }

  // if (is_skip_ref)
  // {
  //   // to skip the checking of an instruction, just copy the reg state to reference design
  //   ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
  //   is_skip_ref --;
  //   return;
  // }

  ref_difftest_exec(1);
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT);

  if (npc != ref_r.pc)
  {
    Log("pc is different after executing instruction at pc = " FMT_WORD
        ", right = " FMT_WORD ", wrong = " FMT_WORD ", diff = " FMT_WORD,
        pc, ref_r.pc, npc, ref_r.pc ^ npc);

    return;
  }

  difftest_checkregs(&ref_r, pc);
}

void difftest_init(char *ref_so_file, long img_size, int port)
{
  assert(ref_so_file != nullptr);

  std::ifstream so(ref_so_file);
  void *handle;

  if (so.good())
    assert(handle);
  else
    std::cout << "SO File not exist\n";

  handle = dlopen(ref_so_file, RTLD_LAZY);

  ref_difftest_memcpy = reinterpret_cast<void (*)(paddr_t, void *, size_t, bool)>(
      dlsym(handle, "difftest_memcpy"));
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = reinterpret_cast<void (*)(void *, bool)>(
      dlsym(handle, "difftest_regcpy"));
  assert(ref_difftest_regcpy);

  ref_difftest_exec = reinterpret_cast<void (*)(uint64_t)>(
      dlsym(handle, "difftest_exec"));
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = reinterpret_cast<void (*)(uint64_t)>(
      dlsym(handle, "difftest_raise_intr"));
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = reinterpret_cast<void (*)(int)>(
      dlsym(handle, "difftest_init"));

  assert(ref_difftest_init);

  Log("Differential testing: %s", ASNI_FMT("ON", ASNI_FG_GREEN));
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in menuconfig.",
      ref_so_file);

  extern axi4_mem<64, 64, 4> mem;

  ref_difftest_init(port);
  ref_difftest_memcpy(RESET_VECTOR, (uint8_t*)(mem.get_mem()+RESET_VECTOR), img_size, DIFFTEST_TO_REF);
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}
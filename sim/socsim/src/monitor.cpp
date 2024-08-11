#include <string>
#include <string.h>
#include "include/monitor.h"
#include "include/axi_slave_mem.h"
#include "include/difftest.h"
#include "include/expr.h"

extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
uint64_t core_run_once();
void core_pass_registers(uint64_t *reg);
extern std::map<uint64_t, uint64_t> imap;
std::set<std::string> watchpoints;

const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void isa_reg_display()
{
  // print pc
  printf("%s: 0x%lx\n", "pc", cpu.pc);

  // display the coresponding registers
  for (int i = 0; i < 32; i++)
  {
    printf("%s: 0x%lx\n", regs[i], cpu.gpr[i]);
  }
}

void execute_once()
{
  uint64_t cur_pc = cpu.pc;
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

void execute(int n)
{
  for (unsigned int i = 0; i < n; i++)
  {

    execute_once();
    for (auto e : watchpoints)
    {
      auto val = expr((char *)e.c_str());
      if (val == 1)
      {
        std::cout << e << " hit" << std::endl;
        return;
      }
    }
  }
}

word_t isa_reg_str2val(const char *s, int *success)
{
  // store the register number
  int i;
  if (strcmp(s, "pc") == 0)
  {
    *success = 1;
    return cpu.pc;
  }
  else
  {
    for (i = 0; i < 32; i++)
    {
      if (strcmp(s, regs[i]) == 0)
      {
        *success = 1;
        return cpu.gpr[i];
      }
    }
    *success = 0;
    return 0;
  }
}

void set_watch(std::string e)
{
  auto val = expr((char *)(e.c_str()));
  if (val != 0xdeadbeef)
  {
    std::cout << "watchpoint " << e << " set" << std::endl;
    watchpoints.insert(e);
  }
}

word_t mem_read(word_t addr, size_t len, std::string &ori)
{
  extern axi4_mem<64, 64, 4> mem;
  extern Vcore *dut;
  int index = addr >> 6 & 0b111111;
  int tag = addr >> 12;
  int target_way = -1;
  ori = "mem";

  void *memory;

  for (auto way = 0; way < 8; way++)
  {
    if (tag == dut->core__DOT__dc0__DOT__line_tag[way][index])
    {
      target_way = way;
      ori = "dcache";
    }
    else if (tag == dut->core__DOT__ic0__DOT__line_tag[way][index])
    {
      target_way = way;
      ori = "icache";
    }
  }

  if (ori == "icache")
  {
    addr = addr & 0b111111; // offset
    switch (target_way)
    {
    case 0:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__0__KET____DOT__ram__DOT__mem[index];
      break;
    case 1:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__1__KET____DOT__ram__DOT__mem[index];
      break;
    case 2:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__2__KET____DOT__ram__DOT__mem[index];
      break;
    case 3:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__3__KET____DOT__ram__DOT__mem[index];
      break;
    case 4:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__4__KET____DOT__ram__DOT__mem[index];
      break;
    case 5:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__5__KET____DOT__ram__DOT__mem[index];
      break;
    case 6:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__6__KET____DOT__ram__DOT__mem[index];
      break;
    case 7:
      memory = (void *)dut->core__DOT__ic0__DOT__genblk1__BRA__7__KET____DOT__ram__DOT__mem[index];
      break;
    default:
      break;
    }
  }

  else if (ori == "dcache")
  {
    addr = addr & 0b111111; // offset
    switch (target_way)
    {
    case 0:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__0__KET____DOT__ram__DOT__mem[index];
      break;
    case 1:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__1__KET____DOT__ram__DOT__mem[index];
      break;
    case 2:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__2__KET____DOT__ram__DOT__mem[index];
      break;
    case 3:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__3__KET____DOT__ram__DOT__mem[index];
      break;
    case 4:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__4__KET____DOT__ram__DOT__mem[index];
      break;
    case 5:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__5__KET____DOT__ram__DOT__mem[index];
      break;
    case 6:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__6__KET____DOT__ram__DOT__mem[index];
      break;
    case 7:
      memory = (void *)dut->core__DOT__dc0__DOT__genblk1__BRA__7__KET____DOT__ram__DOT__mem[index];
      break;
    default:
      break;
    }
  }
  else
  {
    memory = mem.get_mem();
  }

  if (len == 1)
    return *(uint8_t *)(((uint8_t *)memory) + addr);
  else if (len == 2)
    return *(uint16_t *)(((uint8_t *)memory) + addr);
  else if (len == 4)
    return *(uint32_t *)(((uint8_t *)memory) + addr);
  else if (len == 8)
    return *(uint64_t *)(((uint8_t *)memory) + addr);
}
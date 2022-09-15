#pragma once
#include <stdint.h>

typedef uint64_t word_t;
typedef uint64_t sword_t;
typedef uint64_t paddr_t;
typedef uint64_t vaddr_t;

#define RESET_VECTOR 0x80000000

typedef struct {
  word_t gpr[32];
  vaddr_t pc;
} riscv64_CPU_state;
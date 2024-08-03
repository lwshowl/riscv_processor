#pragma once
#include <iostream>
#include "common.h"
#include <stdio.h>
#include <string>

extern riscv64_CPU_state cpu;
void isa_reg_display();
word_t isa_reg_str2val(const char *s, int *success);
word_t mem_read(word_t addr, size_t len, std::string &origin);
void execute(int n);
void set_watch(std ::string e);
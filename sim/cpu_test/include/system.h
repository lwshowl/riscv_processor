#pragma once
#include <stdint.h>

#define STDOUT_MMIO ((uint64_t *)0x83000000)

static void error()
{
    while (1)
        ;
}

static __attribute__((noinline)) void check(bool cond)
{
    if (!cond)
        error();
}
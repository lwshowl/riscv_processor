#include <stdint.h>
#include "trap.h"

#define STDOUT_MMIO ((uint64_t*)0x83000000)

void puts(char *str) {
    while (*str) {
        *STDOUT_MMIO = *str++;
    }
}

int main() {
    puts("hello\n");
    return 0;
}
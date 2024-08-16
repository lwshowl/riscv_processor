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

static void puts(char *str)
{
    while (*str)
    {
        *STDOUT_MMIO = *str++;
    }
}

static int strlen(char *str)
{
    int i = 0;
    while (*str++)
    {
        i++;
    }
    return i;
}

static void strrev(char *str)
{
    int len = strlen(str);
    char *right = str + len - 1;
    char *left = str;
    while (left < right)
    {
        char temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

static void itoa(int n, char *string)
{
    if (n == 0)
    {
        string[0] = '0';
        return;
    }
    int idx = 0;
    while (n > 0)
    {
        string[idx++] = (n % 10) + '0';
        n = n / 10;
    }
    string[idx] = 0;
    strrev(string);
}

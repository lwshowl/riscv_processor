#include "klib.h"
#include "system.h"

char *s[] = {
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    ", World!\n",
    "Hello, World!\n",
    "#####"};

char str1[] = "Hello";
char str[20];

int main()
{
    for(int i=0; i<6; i++) {
        printf("%s\n",s[i]);
    }
    check(strcmp(s[0], s[2]) == 0);
    check(strcmp(s[0], s[1]) < 0);
    check(strcmp(s[0] + 1, s[1] + 1) < 0);
    check(strcmp(s[0] + 2, s[1] + 2) < 0);
    check(strcmp(s[0] + 3, s[1] + 3) < 0);

    check(strcmp(strcat(strcpy(str, str1), s[3]), s[4]) == 0);

    check(memcmp(memset(str, '#', 5), s[5], 5) == 0);


    return 0;
}

// // (nemu) x 10 $a0
// [src/monitor/sdb/expr.c:110 make_token] match rules[3] = "\$[0-9a-zA-Z]+" at position 0 with len 3: $a0
// 0x00000000800002e0: 6c6c6548 0000006f 00000000 00000000 00000000 6a78c728 27bfe764 22bb807f 659afee8 4533b563
// (nemu) $ 10 $a1
// Unknown command '$'
// (nemu) w 10 $a1
// inserted at 3
// watch point set at 10 $a1
// (nemu) x 10 $a1
// [src/monitor/sdb/expr.c:110 make_token] match rules[3] = "\$[0-9a-zA-Z]+" at position 0 with len 3: $a1
// 0x0000000080000278: 6f57202c 21646c72 0000000a 00000000 6c6c6548 57202c6f 646c726f 00000a21 23232323 00000023
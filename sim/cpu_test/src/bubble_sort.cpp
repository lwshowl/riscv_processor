#include "system.h"
#include "klib.h"
#define N 20

int a[N] = {2, 12, 14, 6, 13, 15, 16, 10, 0, 18, 11, 19, 9, 1, 7, 5, 4, 3, 8, 17};

void bubble_sort()
{
    int i, j, t;
    for (j = 0; j < N; j++)
    {
        for (i = 0; i < N - 1 - j; i++)
        {
            if (a[i] > a[i + 1])
            {
                t = a[i];
                a[i] = a[i + 1];
                a[i + 1] = t;
                printf("swaping %d %d\n", i, i + 1);
            }
        }
    }
}

int main()
{
    bubble_sort();
    printf("after sorting the array:");
    for (int i=0; i<sizeof(a)/sizeof(a[0]); i++) {
        printf("%d,", a[i]);
    }
    printf("\b\n");
    return 0;
}

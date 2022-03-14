#include "mis.h"

void selectionSort(int *array, int size)
{
    int pMin = 0;
    for (int loop = 0; loop < size; loop++)
    {
        pMin = loop;
        for (int current = loop; current < size; current++)
        {
            if (array[current] < array[pMin])
                pMin = current;
        }
        swap(array[pMin], array[loop]);
    }
}

int arrayPartition(int *A, int start, int end)
{
    int rIndex = 0;
    int currentIndex = 0;
    int pivot = A[end];

    for (currentIndex = 0; currentIndex < end; currentIndex++)
    {
        if (A[currentIndex] < pivot)
        {
            swap(A[currentIndex], A[rIndex]);
            rIndex++;
        }
    }

    swap(A[rIndex], A[end]);
    return rIndex;
}

void quickSort(int *A, int start, int end)
{
    if (start >= end)
        return;
    int pIndex;
    pIndex = arrayPartition(A, start, end);
    quickSort(A, pIndex + 1, end);
    quickSort(A, start, pIndex - 1);
}

void bubbleSort(int *array, int start, int end)
{
    int length = end - start;
    bool haveSwap = false;
    for (int iterate = 0; iterate < length; iterate++)
    {
        for (int index = 0; index < end - iterate; index++)
        {
            if (array[index] > array[index + 1])
            {
                swap(array[index], array[index + 1]);
                haveSwap = true;
            }
        }
        if (haveSwap == false)
            break;
    }
}
#include "shuffle.h"
#include <stdlib.h>
void swap_element(int* arr, int i, int j)
{
    //Get i location element
    int temp = arr[i];
    //set new values
    arr[i] = arr[j];
    arr[j] = temp;
}

int random_location(int first, int last)
{
    return rand() % ((last) - first) + first;
}

void shuffle(int* arr, int n, int seed)
{
    srand(seed);
    int j = 0;
    int i = 0;
    int counter = 0;
    while (counter < n)
    {
        i = random_location(0, n);
        j = random_location(0, n);
        if (i != j)
        {
            swap_element(arr, i, j);
            counter++;
        }
    }
}

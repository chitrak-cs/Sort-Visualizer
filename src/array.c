#include <stdlib.h>

#include "array.h"

int *numbers = NULL;
int count = 40;

void swap(int i, int j)
{
    int temp = numbers[i];
    numbers[i] = numbers[j];
    numbers[j] = temp;
}

void initialise_numbers()
{
    if (numbers != NULL)
    {
        free(numbers);
    }

    numbers = malloc(sizeof(int) * count);

    if (numbers == NULL)
    {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++)
    {
        numbers[i] = i + 1;
    }

    // Fisher-Yates shuffle
    for (int i = count - 1; i >= 0; i--)
    {
        int j = rand() % (i + 1);

        swap(i, j);
    }
}
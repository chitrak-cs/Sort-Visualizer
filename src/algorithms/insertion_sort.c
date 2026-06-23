#include "../sort_state.h"
#include "../array.h"
#include "../metrics.h"
#include "algorithm.h"

#include "insertion_sort.h"
#include <stdbool.h>

typedef enum
{
    IS_COMPARE,
    IS_SWAP,
    IS_ADVANCE,
    IS_DONE
} InsertionState;

static int i = 1;
static int j = 0;
static InsertionState state = IS_COMPARE;

sort_status insertion_step(void)
{
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    switch (state)
    {
    case IS_COMPARE:
        if (i >= count)
        {
            state = IS_DONE;
            break;
        }
        if (j >= 0)
        {
            status.i = j;
            status.j = j + 1;
            metrics.comparisons++;
            if (numbers[j] > numbers[j + 1])
            {
                // Need to swap these two elements
                state = IS_SWAP;
                return status;
            }
        }
        // No more shifts needed for element i
        state = IS_ADVANCE;
        return status;

    case IS_SWAP:
        // Swap numbers[j] and numbers[j+1]
        {
            int tmp = numbers[j];
            numbers[j] = numbers[j + 1];
            numbers[j + 1] = tmp;
        }
        metrics.swaps++;
        status.i = j;
        status.j = j + 1;
        status.swapped = true;
        // Move one position left to continue insertion
        j--;
        state = IS_COMPARE;
        return status;

    case IS_ADVANCE:
        i++;
        j = i - 1;
        sort_n = i; // sorted region extends to i
        state = IS_COMPARE;
        return status;

    case IS_DONE:
        sort_n = count;
        status.done = true;
        return status;
    }

    return status;
}

void reset_insertion(void)
{
    i = 1;
    j = 0;
    sort_n = 0;
    state = IS_COMPARE;
}

Algorithm InsertionSort = {
    .name = "Insertion Sort",
    .step = insertion_step,
    .reset = reset_insertion};

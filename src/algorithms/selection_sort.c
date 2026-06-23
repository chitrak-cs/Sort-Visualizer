#include "../sort_state.h"
#include "../array.h"
#include "../metrics.h"
#include "algorithm.h"

#include "selection_sort.h"
#include <stdbool.h>

typedef enum
{
    SS_COMPARE,
    SS_SWAP,
    SS_ADVANCE,
    SS_DONE
} SelectionState;

static int i = 0;
static int j = 1;
static int min_index = 0;
static SelectionState state = SS_COMPARE;

sort_status selection_step(void)
{
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    switch (state)
    {
    case SS_COMPARE:
        if (i >= count - 1)
        {
            state = SS_DONE;
            break;
        }
        if (j >= count)
        {
            // Done scanning one pass: go swap
            state = SS_SWAP;
            return status;
        }
        // Compare current min with numbers[j]
        status.i = min_index;
        status.j = j;
        metrics.comparisons++;
        if (numbers[j] < numbers[min_index])
        {
            min_index = j;
        }
        j++;
        return status;

    case SS_SWAP:
        // Place min_index element into position i (if not already there)
        if (min_index != i)
        {
            int tmp = numbers[i];
            numbers[i] = numbers[min_index];
            numbers[min_index] = tmp;
            metrics.swaps++;
            status.i = i;
            status.j = min_index;
            status.swapped = true;
        }
        // Advance boundary: one more element is sorted on left
        state = SS_ADVANCE;
        return status;

    case SS_ADVANCE:
        i++;
        sort_n = i; // sorted region grows to i
        min_index = i;
        j = i + 1;
        state = SS_COMPARE;
        return status;

    case SS_DONE:
        sort_n = count; // fully sorted
        status.done = true;
        return status;
    }

    return status;
}

void reset_selection(void)
{
    i = 0;
    j = 1;
    min_index = 0;
    sort_n = 0;
    state = SS_COMPARE;
}

Algorithm SelectionSort = {
    .name = "Selection Sort",
    .step = selection_step,
    .reset = reset_selection};

// src/algorithms/merge_sort.c
#include "../sort_state.h"
#include "../array.h"
#include "../metrics.h"
#include "algorithm.h"

#include "merge_sort.h"
#include <stdbool.h>
#include <stdlib.h>

typedef enum
{
    MS_INIT,
    MS_MERGE,
    MS_COPY,
    MS_ADVANCE,
    MS_DONE
} MergeState;

static int width = 1;
static int left = 0, mid = 0, right = 0;
static int i = 0, j = 0, k = 0;
static int copyIndex = 0;
static int *temp = NULL;
static MergeState state = MS_INIT;

sort_status merge_step(void)
{
    // Allocate temp on first use
    if (!temp)
    {
        temp = malloc(count * sizeof(int));
    }
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    switch (state)
    {
    case MS_INIT:
        if (width >= count)
        {
            state = MS_DONE;
            break;
        }
        left = 0;
        mid = left + width;
        if (mid >= count)
        {
            // Nothing to merge on this width, double and continue
            width *= 2;
            state = MS_INIT;
            return status;
        }
        right = left + 2 * width;
        if (right > count)
            right = count;
        i = left;
        j = mid;
        k = left;
        state = MS_MERGE;
        return status;

    case MS_MERGE:
        if (i < mid && j < right)
        {
            // Compare elements from left and right runs
            status.i = i;
            status.j = j;
            metrics.comparisons++;
            if (numbers[i] <= numbers[j])
            {
                temp[k++] = numbers[i++];
            }
            else
            {
                temp[k++] = numbers[j++];
            }
            return status;
        }
        // If one run is exhausted, go to copying leftover
        state = MS_COPY;
        copyIndex = left;
        return status;

    case MS_COPY:
        // Copy any remaining elements from left run
        if (i < mid)
        {
            temp[k++] = numbers[i++];
            status.i = i - 1;
            status.j = -1;
            status.swapped = true;
            return status;
        }
        // Copy any remaining elements from right run
        if (j < right)
        {
            temp[k++] = numbers[j++];
            status.i = j - 1;
            status.j = -1;
            status.swapped = true;
            return status;
        }
        // All merged into temp; now copy back to original
        if (copyIndex < right)
        {
            numbers[copyIndex] = temp[copyIndex];
            status.i = copyIndex;
            status.j = copyIndex;
            status.swapped = true;
            copyIndex++;
            return status;
        }
        // Merged block done; advance to next block
        state = MS_ADVANCE;
        return status;

    case MS_ADVANCE:
        left += 2 * width;
        if (left >= count)
        {
            width *= 2;
            state = MS_INIT;
        }
        else
        {
            mid = left + width;
            if (mid >= count)
            {
                width *= 2;
                state = MS_INIT;
            }
            else
            {
                right = left + 2 * width;
                if (right > count)
                    right = count;
                i = left;
                j = mid;
                k = left;
                state = MS_MERGE;
            }
        }
        return status;

    case MS_DONE:
        sort_n = count;
        status.done = true;
        return status;
    }

    return status;
}

void reset_merge(void)
{
    width = 1;
    left = mid = right = 0;
    i = j = k = 0;
    copyIndex = 0;
    sort_n = 0;
    state = MS_INIT;
    if (temp)
    {
        free(temp);
        temp = NULL;
    }
}

Algorithm MergeSort = {
    .name = "Merge Sort",
    .step = merge_step,
    .reset = reset_merge};

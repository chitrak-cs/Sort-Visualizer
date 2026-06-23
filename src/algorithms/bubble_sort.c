#include "../sort_state.h"
#include "../metrics.h"
#include "bubble_sort.h"
#include "../array.h" // provides 'count' and 'numbers[]'
#include <stdbool.h>
#include "algorithm.h"

static int i = 0; // current index in pass
static int n = 0; // length of unsorted region (initially count)
int sort_n = 0;   // start of sorted region (elements at >= sort_n are green)
static SortState state = BS_COMPARE;
static int finish_animation = -1;

sort_status sort_step(void)
{
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    // Initialize on first call or after reset
    if (n == 0)
    {
        n = count;
        sort_n = n;
        i = 0;
        state = BS_COMPARE;
    }

    switch (state)
    {
    case BS_COMPARE:
        if (i >= n - 1)
        {
            state = BS_MARK;
            return status;
        }

        status.i = i;
        status.j = i + 1;

        metrics.comparisons++;
        if (numbers[i] > numbers[i + 1])
        {
            state = BS_SWAP;
        }
        else
        {
            state = BS_ADVANCE;
        }

        return status;

    case BS_SWAP:
    {
        int tmp = numbers[i];
        numbers[i] = numbers[i + 1];
        numbers[i + 1] = tmp;

        status.i = i;
        status.j = i + 1;
        status.swapped = true;
        metrics.swaps++;

        state = BS_ADVANCE;

        return status;
    }

    case BS_ADVANCE:
        i++;

        if (i >= n - 1)
        {
            state = BS_MARK;
        }
        else
        {
            state = BS_COMPARE;
        }

        return status;

    case BS_MARK:
        n--;
        sort_n = n;

        if (n <= 0)
        {
            status.done = true;
            state = BS_DONE;
        }
        else
        {
            i = 0;
            state = BS_COMPARE;
        }

        return status;

    case BS_DONE:
        status.done = true;
        return status;
    }

    return status;
}
void reset_sort(void)
{
    n = 0;
    sort_n = 0;
    i = 0;
    state = BS_COMPARE;
}

int get_finish_animation_index(void)
{
    if (finish_animation == -1)
        return count;

    if (finish_animation < count)
        finish_animation++;

    return finish_animation;
}

Algorithm BubbleSort =
    {
        .name = "Bubble Sort",

        .step = sort_step,

        .reset = reset_sort
    };

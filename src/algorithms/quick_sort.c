#include "../sort_state.h"
#include "../array.h"
#include "../metrics.h"

#include "quick_sort.h"

#include <stdbool.h>

#define MAX_STACK 1024

typedef struct
{
    int lo;
    int hi;
} Range;

typedef enum
{
    QS_GET_RANGE,
    QS_COMPARE,
    QS_SWAP_INNER,
    QS_FINAL_SWAP,
    QS_DONE
} QuickState;

static Range stack[MAX_STACK];
static int top = -1;

static int lo = 0;
static int hi = 0;
static int i = 0;
static int store_index = 0;

static QuickState state = QS_GET_RANGE;

static void push_range(int left, int right)
{
    if (left < right && top < MAX_STACK - 1)
    {
        top++;
        stack[top].lo = left;
        stack[top].hi = right;
    }
}

static bool pop_range(Range *r)
{
    if (top < 0)
    {
        return false;
    }

    *r = stack[top];
    top--;
    return true;
}

sort_status quick_step(void)
{
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    switch (state)
    {
    case QS_GET_RANGE:
    {
        Range r;

        while (pop_range(&r))
        {
            if (r.lo < r.hi)
            {
                lo = r.lo;
                hi = r.hi;
                i = lo;
                store_index = lo;
                state = QS_COMPARE;
                sort_n = count; // do not mark a partial sorted region for quicksort
                return status;
            }
        }

        state = QS_DONE;
        return status;
    }

    case QS_COMPARE:
        if (i < hi)
        {
            status.i = i;
            status.j = hi;
            metrics.comparisons++;

            if (numbers[i] <= numbers[hi])
            {
                if (i != store_index)
                {
                    state = QS_SWAP_INNER;
                }
                else
                {
                    store_index++;
                    i++;
                }
            }
            else
            {
                i++;
            }

            return status;
        }

        state = QS_FINAL_SWAP;
        return status;

    case QS_SWAP_INNER:
    {
        int temp = numbers[i];
        numbers[i] = numbers[store_index];
        numbers[store_index] = temp;

        metrics.swaps++;

        status.i = i;
        status.j = store_index;
        status.swapped = true;

        store_index++;
        i++;
        state = QS_COMPARE;

        return status;
    }

    case QS_FINAL_SWAP:
    {
        int pivot_pos = store_index;

        int temp = numbers[pivot_pos];
        numbers[pivot_pos] = numbers[hi];
        numbers[hi] = temp;

        metrics.swaps++;

        status.i = pivot_pos;
        status.j = hi;
        status.swapped = true;

        // Push right first, then left, so the left side is processed next
        push_range(pivot_pos + 1, hi);
        push_range(lo, pivot_pos - 1);

        state = QS_GET_RANGE;
        sort_n = count; // keep quicksort bars uncolored until completion

        return status;
    }

    case QS_DONE:
        sort_n = count;
        status.done = true;
        return status;
    }

    return status;
}

void reset_quick(void)
{
    top = -1;
    lo = 0;
    hi = 0;
    i = 0;
    store_index = 0;
    sort_n = count;
    state = QS_GET_RANGE;

    if (count > 1)
    {
        push_range(0, count - 1);
    }
    else
    {
        state = QS_DONE;
    }
}

Algorithm QuickSort =
    {
        .name = "Quick Sort",
        .step = quick_step,
        .reset = reset_quick};
#include "../sort_state.h"
#include "../array.h"
#include "../metrics.h"

#include "heap_sort.h"

#include <stdbool.h>

typedef enum
{
    HS_BUILD_LEFT,
    HS_BUILD_RIGHT,
    HS_BUILD_APPLY,
    HS_BUILD_NEXT,

    HS_SORT_SWAP_ROOT,
    HS_SORT_LEFT,
    HS_SORT_RIGHT,
    HS_SORT_APPLY,

    HS_DONE
} HeapState;

static HeapState state = HS_BUILD_NEXT;

static int heap_end = 0;   // exclusive end of heap
static int build_root = 0; // current root while building heap

static int sift_root = 0;    // root currently being sifted
static int sift_largest = 0; // largest among root/children
static int sift_child = 0;   // left child index

static void swap_values(int a, int b)
{
    int temp = numbers[a];
    numbers[a] = numbers[b];
    numbers[b] = temp;
}

sort_status heap_step(void)
{
    sort_status status = {.i = -1, .j = -1, .swapped = false, .done = false};

    switch (state)
    {
    case HS_BUILD_NEXT:
        heap_end = count;
        sort_n = count; // during build, no sorted region yet

        if (count <= 1)
        {
            state = HS_DONE;
            return status;
        }

        build_root = heap_end / 2 - 1;

        if (build_root < 0)
        {
            state = HS_SORT_SWAP_ROOT;
            return status;
        }

        sift_root = build_root;
        sift_largest = sift_root;
        sift_child = 2 * sift_root + 1;
        state = HS_BUILD_LEFT;
        return status;

    case HS_BUILD_LEFT:
        if (build_root < 0)
        {
            state = HS_SORT_SWAP_ROOT;
            return status;
        }

        if (sift_child >= heap_end)
        {
            state = HS_BUILD_NEXT;
            return status;
        }

        status.i = sift_root;
        status.j = sift_child;
        metrics.comparisons++;

        if (numbers[sift_child] > numbers[sift_largest])
        {
            sift_largest = sift_child;
        }

        state = HS_BUILD_RIGHT;
        return status;

    case HS_BUILD_RIGHT:
    {
        int right = sift_child + 1;

        if (right < heap_end)
        {
            status.i = sift_largest;
            status.j = right;
            metrics.comparisons++;

            if (numbers[right] > numbers[sift_largest])
            {
                sift_largest = right;
            }
        }

        state = HS_BUILD_APPLY;
        return status;
    }

    case HS_BUILD_APPLY:
        if (sift_largest != sift_root)
        {
            swap_values(sift_root, sift_largest);
            metrics.swaps++;

            status.i = sift_root;
            status.j = sift_largest;
            status.swapped = true;

            // Continue sifting the element down
            sift_root = sift_largest;
            sift_largest = sift_root;
            sift_child = 2 * sift_root + 1;
            state = HS_BUILD_LEFT;
            return status;
        }

        // Current subtree is heapified; move to previous root
        build_root--;

        if (build_root < 0)
        {
            state = HS_SORT_SWAP_ROOT;
            return status;
        }

        sift_root = build_root;
        sift_largest = sift_root;
        sift_child = 2 * sift_root + 1;
        state = HS_BUILD_LEFT;
        return status;

    case HS_SORT_SWAP_ROOT:
        if (heap_end <= 1)
        {
            sort_n = 0;
            state = HS_DONE;
            return status;
        }

        // Move maximum element to the end of the heap
        swap_values(0, heap_end - 1);
        metrics.swaps++;

        status.i = 0;
        status.j = heap_end - 1;
        status.swapped = true;

        heap_end--;
        sort_n = heap_end; // sorted region is the tail

        if (heap_end <= 1)
        {
            sort_n = 0;
            state = HS_DONE;
            return status;
        }

        sift_root = 0;
        sift_largest = 0;
        sift_child = 1;
        state = HS_SORT_LEFT;
        return status;

    case HS_SORT_LEFT:
        if (sift_child >= heap_end)
        {
            state = HS_SORT_SWAP_ROOT;
            return status;
        }

        status.i = sift_root;
        status.j = sift_child;
        metrics.comparisons++;

        if (numbers[sift_child] > numbers[sift_largest])
        {
            sift_largest = sift_child;
        }

        state = HS_SORT_RIGHT;
        return status;

    case HS_SORT_RIGHT:
    {
        int right = sift_child + 1;

        if (right < heap_end)
        {
            status.i = sift_largest;
            status.j = right;
            metrics.comparisons++;

            if (numbers[right] > numbers[sift_largest])
            {
                sift_largest = right;
            }
        }

        state = HS_SORT_APPLY;
        return status;
    }

    case HS_SORT_APPLY:
        if (sift_largest != sift_root)
        {
            swap_values(sift_root, sift_largest);
            metrics.swaps++;

            status.i = sift_root;
            status.j = sift_largest;
            status.swapped = true;

            // Keep sifting the moved element down
            sift_root = sift_largest;
            sift_largest = sift_root;
            sift_child = 2 * sift_root + 1;
            state = HS_SORT_LEFT;
            return status;
        }

        // Heap property restored, extract next max on next call
        state = HS_SORT_SWAP_ROOT;
        return status;

    case HS_DONE:
        sort_n = 0;
        status.done = true;
        return status;
    }

    return status;
}

void reset_heap(void)
{
    heap_end = 0;
    build_root = 0;
    sift_root = 0;
    sift_largest = 0;
    sift_child = 0;
    sort_n = count;
    state = HS_BUILD_NEXT;
}

Algorithm HeapSort =
    {
        .name = "Heap Sort",
        .step = heap_step,
        .reset = reset_heap};
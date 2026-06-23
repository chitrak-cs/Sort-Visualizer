#include <stdbool.h>
#include <raylib.h>
#include <stddef.h>

#include "algorithms/bubble_sort.h"
#include "algorithms/selection_sort.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/merge_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/heap_sort.h"

#include "algorithm_manager.h"
#include "metrics.h"
#include "array.h"
#include "renderer.h"
#include "sort_state.h"
#include "config.h"

static void update_speed_from_count(int *sorting_speed)
{
    *sorting_speed = count / 20;
    if (*sorting_speed < 1)
    {
        *sorting_speed = 1;
    }
}

static void switch_algorithm(Algorithm *algorithm,
                             sort_status *status,
                             bool *paused,
                             int *sorting_speed)
{
    set_algorithm(algorithm);

    if (current_algorithm != NULL)
    {
        current_algorithm->reset();
    }

    initialise_numbers();
    reset_metrics();

    *status = (sort_status){0};
    *paused = false;

    update_speed_from_count(sorting_speed);
}

int main(void)
{
    initialise_numbers();

    InitWindow(WIDTH, HEIGHT, "Sorting Visualizer");
    SetTargetFPS(60);

    set_algorithm(&BubbleSort);

    if (current_algorithm != NULL)
    {
        current_algorithm->reset();
    }

    reset_metrics();

    sort_status status = (sort_status){0};
    bool paused = false;

    int sorting_speed = count / 20;
    if (sorting_speed < 1)
    {
        sorting_speed = 1;
    }

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_ONE))
        {
            switch_algorithm(&BubbleSort, &status, &paused, &sorting_speed);
        }
        if (IsKeyPressed(KEY_TWO))
        {
            switch_algorithm(&SelectionSort, &status, &paused, &sorting_speed);
        }
        if (IsKeyPressed(KEY_THREE))
        {
            switch_algorithm(&InsertionSort, &status, &paused, &sorting_speed);
        }
        if (IsKeyPressed(KEY_FOUR))
        {
            switch_algorithm(&MergeSort, &status, &paused, &sorting_speed);
        }
        if (IsKeyPressed(KEY_FIVE))
        {
            switch_algorithm(&QuickSort, &status, &paused, &sorting_speed);
        }
        if (IsKeyPressed(KEY_SIX))
        {
            switch_algorithm(&HeapSort, &status, &paused, &sorting_speed);
        }

        if (IsKeyPressed(KEY_R))
        {
            initialise_numbers();

            if (current_algorithm != NULL)
            {
                current_algorithm->reset();
            }

            reset_metrics();
            status = (sort_status){0};
        }

        if (IsKeyPressed(KEY_UP))
        {
            count += 20;

            initialise_numbers();

            if (current_algorithm != NULL)
            {
                current_algorithm->reset();
            }

            reset_metrics();
            status = (sort_status){0};

            update_speed_from_count(&sorting_speed);
        }

        if (IsKeyPressed(KEY_DOWN) && count > 20)
        {
            count -= 20;

            initialise_numbers();

            if (current_algorithm != NULL)
            {
                current_algorithm->reset();
            }

            reset_metrics();
            status = (sort_status){0};

            update_speed_from_count(&sorting_speed);
        }

        if (IsKeyPressed(KEY_EQUAL))
        {
            if (sorting_speed == 0)
            {
                sorting_speed = 1;
            }
            else
            {
                sorting_speed *= 2;
            }
        }

        if (IsKeyPressed(KEY_MINUS))
        {
            if (sorting_speed > 1)
            {
                sorting_speed /= 2;
            }
            else if (sorting_speed == 1)
            {
                sorting_speed = 0;
            }
        }

        if ((paused || sorting_speed == 0) &&
            IsKeyPressed(KEY_N) &&
            !status.done &&
            current_algorithm != NULL)
        {
            status = current_algorithm->step();
        }

        if (!paused &&
            sorting_speed > 0 &&
            !status.done &&
            current_algorithm != NULL)
        {
            for (int k = 0; k < sorting_speed; k++)
            {
                status = current_algorithm->step();
                if (status.done)
                {
                    break;
                }
            }
        }

        BeginDrawing();
        ClearBackground((Color){15, 15, 18, 255});

        draw_bars(status);
        draw_ui(paused, sorting_speed);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
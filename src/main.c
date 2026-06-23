// main.c
#include <stdlib.h>
#include <stdbool.h>
#include <raylib.h>

#include "metrics.h"
#include "array.h" // initialises 'numbers' and 'count'
#include "renderer.h"
#include "sort_state.h" // sort_step(), reset_sort(), sort_n
#include "config.h"     // WIDTH, HEIGHT, SIDEBAR_WIDTH

int main(void)
{
    initialise_numbers(); // fill the array and set count

    InitWindow(WIDTH, HEIGHT, "Sorting Visualizer");
    SetTargetFPS(60); // lock to 60 FPS

    sort_status status = {0};
    bool paused = false;
    int sorting_speed = count / 20;
    if (sorting_speed < 1)
        sorting_speed = 1; // ensure at least 1

    reset_sort(); // initialize sorting state

    while (!WindowShouldClose())
    {
        // Pause/Resume
        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
        }
        // Next-step (manual) only when paused
        if (paused && IsKeyPressed(KEY_N) && !status.done)
        {
            status = sort_step();
        }
        // Shuffle the array
        if (IsKeyPressed(KEY_R))
        {
            initialise_numbers();
            reset_sort();
            status = (sort_status){0};
        }
        // Increase array size
        if (IsKeyPressed(KEY_UP))
        {
            count += 20;
            initialise_numbers();
            reset_sort();
            status = (sort_status){0};
            sorting_speed = count / 20;
            if (sorting_speed < 1)
                sorting_speed = 1;
        }
        // Decrease array size
        if (IsKeyPressed(KEY_DOWN) && count > 20)
        {
            count -= 20;
            initialise_numbers();
            reset_sort();
            reset_metrics();
            status = (sort_status){0};
            sorting_speed = count / 20;
            if (sorting_speed < 1)
                sorting_speed = 1;
        }
        // Adjust speed: '+' (or '=') doubles speed; '-' halves it or to 0
        if (IsKeyPressed(KEY_EQUAL))
        {
            if (sorting_speed == 0)
                sorting_speed = 1;
            else
                sorting_speed *= 2;
        }
        if (IsKeyPressed(KEY_MINUS))
        {
            if (sorting_speed > 1)
            {
                sorting_speed /= 2;
            }
            else if (sorting_speed == 1)
            {
                sorting_speed = 0; // enter manual mode
            }
        }

        // Automatic stepping: up to sorting_speed steps per frame
        if (!paused && sorting_speed > 0 && !status.done)
        {
            for (int k = 0; k < sorting_speed; k++)
            {
                status = sort_step();
                if (status.done)
                    break;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground((Color){15, 15, 18, 255}); // charcoal background
        draw_bars(status);
        draw_ui(paused, sorting_speed);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

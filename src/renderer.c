#include <raylib.h>

#include "metrics.h"
#include "algorithm_manager.h"
#include "renderer.h"
#include "array.h"
#include "sort_state.h"
#include "config.h"

void draw_bars(sort_status status)
{
    float gap = (count <= 100) ? 1.0f : 0.0f;
    float drawable_width = WIDTH - SIDEBAR_WIDTH;

    for (int i = 0; i < count; i++)
    {
        int value = numbers[i];

        float bar_height = ((float)value * HEIGHT * 0.86f) / count;
        float bar_width = (drawable_width / count) - gap;
        float xpos = SIDEBAR_WIDTH + ((float)i / count) * drawable_width;
        float ypos = HEIGHT - bar_height;

        Color color = (Color){230, 230, 230, 255};

        if (status.done)
        {
            color = (Color){0, 220, 90, 255};
        }
        else if (i >= sort_n)
        {
            color = (Color){0, 220, 90, 255};
        }
        else if (i == status.i || i == status.j)
        {
            if (status.swapped)
            {
                color = (Color){255, 70, 70, 255};
            }
            else
            {
                color = SKYBLUE;
            }
        }

        DrawRectangle(
            (int)xpos + 2,
            (int)ypos + 2,
            (int)bar_width,
            (int)bar_height,
            (Color){0, 0, 0, 35});

        DrawRectangle(
            (int)xpos,
            (int)ypos,
            (int)bar_width,
            (int)bar_height,
            color);

        if (count <= 100)
        {
            DrawRectangleLines(
                (int)xpos,
                (int)ypos,
                (int)bar_width,
                (int)bar_height,
                (Color){70, 70, 70, 255});
        }
    }

    DrawLine(
        SIDEBAR_WIDTH,
        HEIGHT - 1,
        WIDTH,
        HEIGHT - 1,
        (Color){60, 60, 60, 255});
}

void draw_ui(bool paused, int sorting_speed)
{
    Color labelColor = (Color){150, 150, 150, 255};
    Color valueColor = (Color){230, 230, 230, 255};
    Color statusColor = paused ? (Color){255, 180, 60, 255} : (Color){0, 220, 90, 255};
    Color lineColor = (Color){60, 60, 60, 255};
    Color accentColor = SKYBLUE;
    Color sectionColor = (Color){180, 180, 180, 255};
    Color panelColor = (Color){24, 24, 24, 255};
    Color borderColor = (Color){55, 55, 55, 255};

    DrawRectangle(
        0,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        panelColor);

    DrawLine(
        SIDEBAR_WIDTH,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        borderColor);

    // HEADER
    DrawText("SORTING", 25, 22, 32, GOLD);
    DrawText("VISUALIZER", 25, 60, 24, accentColor);

    DrawLine(20, 100, SIDEBAR_WIDTH - 20, 100, lineColor);

    // STATUS
    DrawText("Status", 25, 120, 18, labelColor);
    DrawText(paused ? "PAUSED" : "RUNNING", 25, 145, 28, statusColor);

    // ARRAY SIZE
    DrawText("Array Size", 25, 200, 18, labelColor);
    DrawText(TextFormat("%d", count), 25, 225, 28, valueColor);

    // SPEED
    DrawText("Animation Speed", 25, 280, 18, labelColor);
    DrawText(TextFormat("%dx", sorting_speed), 25, 305, 28, valueColor);

    DrawLine(20, 350, SIDEBAR_WIDTH - 20, 350, lineColor);

    // STATISTICS
    DrawText("Statistics", 25, 370, 24, accentColor);

    DrawText("Comparisons", 25, 415, 18, labelColor);
    DrawText(TextFormat("%ld", metrics.comparisons), 25, 440, 24, valueColor);

    DrawText("Swaps", 25, 490, 18, labelColor);
    DrawText(TextFormat("%ld", metrics.swaps), 25, 515, 24, valueColor);

    DrawLine(20, 560, SIDEBAR_WIDTH - 20, 560, lineColor);

    // ALGORITHMS
    DrawText("Algorithms", 25, 580, 24, accentColor);
    DrawText("[1] Bubble Sort", 25, 620, 18, sectionColor);
    DrawText("[2] Selection Sort", 25, 650, 18, sectionColor);
    DrawText("[3] Insertion Sort", 25, 680, 18, sectionColor);
    DrawText("[4] Merge Sort", 25, 710, 18, sectionColor);
    DrawText("[5] Quick Sort", 25, 740, 18, sectionColor);
    DrawText("[6] Heap Sort", 25, 770, 18, sectionColor);

    DrawText("Press 1-6 to switch", 25, 805, 16, labelColor);

    DrawLine(20, 835, SIDEBAR_WIDTH - 20, 835, lineColor);

    // FOOTER
    DrawText("Current Algorithm", 25, 852, 18, labelColor);
    DrawText(get_algorithm_name(), 25, 875, 18, accentColor);
}
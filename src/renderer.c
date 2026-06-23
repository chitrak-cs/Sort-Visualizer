#include <raylib.h>

#include "metrics.h"
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

        float bar_height = ((float)value * HEIGHT * 0.85f) / count;
        float bar_width = (drawable_width / count) - gap;
        float xpos = SIDEBAR_WIDTH + ((float)i / count) * drawable_width;
        float ypos = HEIGHT - bar_height;

        Color color = (Color){230, 230, 230, 255};


        // Sorted region during sorting
        if (i >= sort_n)
        {
            color = (Color){0, 220, 90, 255};
        }
        // Current comparison
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

        // Shadow
        DrawRectangle(
            (int)xpos + 2,
            (int)ypos + 2,
            (int)bar_width,
            (int)bar_height,
            (Color){0, 0, 0, 35});

        // Actual bar
        DrawRectangle(
            (int)xpos,
            (int)ypos,
            (int)bar_width,
            (int)bar_height,
            color);

        // Border
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

    // Baseline
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

    DrawRectangle(
        0,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        (Color){24, 24, 24, 255});

    DrawLine(
        SIDEBAR_WIDTH,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        (Color){55, 55, 55, 255});

    //---------------- HEADER ----------------//

    DrawText(
        "SORTING",
        25,
        22,
        32,
        GOLD);

    DrawText(
        "VISUALIZER",
        25,
        60,
        24,
        SKYBLUE);

    DrawLine(
        20,
        100,
        SIDEBAR_WIDTH - 20,
        100,
        lineColor);

    //---------------- STATUS ----------------//

    DrawText(
        "Status",
        25,
        120,
        18,
        labelColor);

    DrawText(
        paused ? "PAUSED" : "RUNNING",
        25,
        145,
        28,
        statusColor);

    //---------------- ARRAY ----------------//

    DrawText(
        "Array Size",
        25,
        195,
        18,
        labelColor);

    DrawText(
        TextFormat("%d", count),
        25,
        220,
        28,
        valueColor);

    //---------------- SPEED ----------------//

    DrawText(
        "Animation Speed",
        25,
        270,
        18,
        labelColor);

    DrawText(
        TextFormat("%dx", sorting_speed),
        25,
        295,
        28,
        valueColor);

    DrawLine(
        20,
        345,
        SIDEBAR_WIDTH - 20,
        345,
        lineColor);

    DrawLine(
        20,
        345,
        SIDEBAR_WIDTH - 20,
        345,
        lineColor);

    // Required data for the sorting
    DrawText(
        "Statistics",
        25,
        365,
        24,
        SKYBLUE);

    DrawText(
        "Comparisons",
        25,
        405,
        18,
        labelColor);

    DrawText(
        TextFormat("%ld", metrics.comparisons),
        25,
        430,
        24,
        valueColor);

    DrawText(
        "Swaps",
        25,
        475,
        18,
        labelColor);

    DrawText(
        TextFormat("%ld", metrics.swaps),
        25,
        500,
        24,
        valueColor);

    //---------------- CONTROLS ----------------//

    DrawText(
        "Controls",
        25,
        365,
        24,
        SKYBLUE);

    int y = 405;
    int gap = 34;

    DrawText("SPACE   Pause / Resume", 25, y, 18, valueColor);
    y += gap;
    DrawText("R       Shuffle", 25, y, 18, valueColor);
    y += gap;
    DrawText("UP      Increase Size", 25, y, 18, valueColor);
    y += gap;
    DrawText("DOWN    Decrease Size", 25, y, 18, valueColor);
    y += gap;
    DrawText("+       Increase Speed", 25, y, 18, valueColor);
    y += gap;
    DrawText("-       Decrease Speed", 25, y, 18, valueColor);
    y += gap;
    DrawText("N       Next Step", 25, y, 18, valueColor);

    //---------------- FOOTER ----------------//

    DrawLine(
        20,
        HEIGHT - 90,
        SIDEBAR_WIDTH - 20,
        HEIGHT - 90,
        lineColor);

    DrawText(
        "Bubble Sort",
        25,
        HEIGHT - 75,
        18,
        SKYBLUE);

    DrawText(
        "Time : O(n²)",
        25,
        HEIGHT - 48,
        16,
        labelColor);
}
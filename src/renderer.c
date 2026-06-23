#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "renderer.h"
#include "metrics.h"
#include "algorithm_manager.h"
#include "array.h"
#include "sort_state.h"
#include "config.h"

#define COL_BACKGROUND (Color){18, 20, 24, 255}
#define COL_CARD (Color){30, 33, 39, 255}
#define COL_BORDER (Color){55, 60, 70, 255}

#define COL_TEXT (Color){235, 235, 240, 255}
#define COL_MUTED (Color){150, 155, 165, 255}

#define COL_BLUE (Color){90, 180, 255, 255}
#define COL_GREEN (Color){0, 220, 100, 255}
#define COL_RED (Color){255, 90, 90, 255}
#define COL_GOLD (Color){255, 210, 70, 255}
#define COL_ORANGE (Color){255, 180, 70, 255}

#define COL_ACTIVE (Color){42, 55, 78, 255}

static void draw_card_with_shadow(
    Rectangle rec,
    Color fill,
    Color border,
    Color accent)
{
    DrawRectangleRounded(
        (Rectangle){
            rec.x + 3,
            rec.y + 4,
            rec.width,
            rec.height},
        0.12f,
        8,
        (Color){0, 0, 0, 70});

    DrawRectangleRounded(
        rec,
        0.12f,
        8,
        fill);

    DrawRectangle(
        rec.x,
        rec.y,
        rec.width,
        4,
        accent);

    DrawRectangleRoundedLines(
        rec,
        0.12f,
        8,
        border);
}

// Text Helper
static void draw_title(
    int x,
    int y,
    const char *text)
{
    DrawText(
        text,
        x,
        y,
        18,
        COL_MUTED);
}

static void draw_value(
    int x,
    int y,
    const char *text)
{
    DrawText(
        text,
        x,
        y,
        22,
        COL_TEXT);
}

// keyboard button
static void draw_key(
    int x,
    int y,
    const char *key)
{
    int w = MeasureText(key, 16) + 18;

    if (w < 48)
        w = 48;

    DrawRectangleRounded(
        (Rectangle){
            x,
            y,
            w,
            24},
        0.20f,
        6,
        (Color){42, 45, 54, 255});

    DrawRectangleRoundedLines(
        (Rectangle){
            x,
            y,
            w,
            24},
        0.20f,
        6,
        COL_BORDER);

    DrawText(
        key,
        x + (w - MeasureText(key, 16)) / 2,
        y + 4,
        16,
        COL_TEXT);
}

// algorithm highlight
static bool algorithm_selected(
    const char *name)
{
    return strcmp(
               name,
               get_algorithm_name()) == 0;
}

// draw bars
void draw_bars(sort_status status)
{
    float gap =
        (count <= 100) ? 1.0f : 0.0f;

    float drawableWidth =
        WIDTH - SIDEBAR_WIDTH;

    for (int i = 0; i < count; i++)
    {
        float width =
            drawableWidth / count - gap;

        float height =
            ((float)numbers[i] * HEIGHT * 0.86f) / count;

        float x =
            SIDEBAR_WIDTH +
            i * (drawableWidth / count);

        float y =
            HEIGHT - height;

        Color color =
            (Color){235, 235, 238, 255};

        if (status.done)
        {
            color = COL_GREEN;
        }
        else if (i >= sort_n)
        {
            color = COL_GREEN;
        }
        else if (i == status.i || i == status.j)
        {
            if (status.swapped)
                color = COL_RED;
            else
                color = COL_BLUE;
        }

        DrawRectangle(
            x + 2,
            y + 3,
            width,
            height,
            (Color){0, 0, 0, 50});

        DrawRectangle(
            x,
            y,
            width,
            height,
            color);

        if (count <= 120)
        {
            DrawRectangleLines(
                x,
                y,
                width,
                height,
                (Color){45, 45, 45, 255});
        }
    }

    DrawLine(
        SIDEBAR_WIDTH,
        HEIGHT - 1,
        WIDTH,
        HEIGHT - 1,
        COL_BORDER);
}

// draw ui
void draw_ui(bool paused, int sorting_speed)
{
    DrawRectangle(
        0,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        COL_BACKGROUND);

    DrawLine(
        SIDEBAR_WIDTH,
        0,
        SIDEBAR_WIDTH,
        HEIGHT,
        COL_BORDER);

    //----------------------------------------
    // Header
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 15, SIDEBAR_WIDTH - 30, 90},
        COL_CARD,
        COL_BORDER,
        COL_BLUE);

    DrawText(
        "SORTING",
        30,
        28,
        32,
        COL_GOLD);

    DrawText(
        "VISUALIZER",
        30,
        62,
        24,
        COL_BLUE);

    DrawText(
        "Visualize Algorithms",
        30,
        88,
        13,
        COL_MUTED);

    //----------------------------------------
    // Status Card
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 120, SIDEBAR_WIDTH - 30, 90},
        COL_CARD,
        COL_BORDER,
        COL_GREEN);

    draw_title(
        30,
        132,
        "STATUS");

    DrawCircle(
        38,
        170,
        6,
        paused ? COL_ORANGE : COL_GREEN);

    DrawText(
        paused ? "PAUSED" : "RUNNING",
        52,
        160,
        24,
        paused ? COL_ORANGE : COL_GREEN);

    //----------------------------------------
    // Statistics
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 225, SIDEBAR_WIDTH - 30, 125},
        COL_CARD,
        COL_BORDER,
        COL_BLUE);

    draw_title(
        30,
        238,
        "STATISTICS");

    DrawText(
        "Comparisons",
        30,
        270,
        17,
        COL_MUTED);

    DrawText(
        TextFormat("%ld", metrics.comparisons),
        215,
        268,
        20,
        COL_TEXT);

    DrawText(
        "Swaps",
        30,
        305,
        17,
        COL_MUTED);

    DrawText(
        TextFormat("%ld", metrics.swaps),
        215,
        303,
        20,
        COL_TEXT);

    //----------------------------------------
    // Playback
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 365, SIDEBAR_WIDTH - 30, 115},
        COL_CARD,
        COL_BORDER,
        COL_GOLD);

    draw_title(
        30,
        378,
        "PLAYBACK");

    DrawText(
        "Array Size",
        30,
        410,
        17,
        COL_MUTED);

    DrawText(
        TextFormat("%d", count),
        215,
        408,
        20,
        COL_TEXT);

    DrawText(
        "Speed",
        30,
        440,
        17,
        COL_MUTED);

    if (sorting_speed == 0)
    {
        DrawText(
            "MANUAL",
            215,
            438,
            20,
            COL_ORANGE);
    }
    else
    {
        DrawText(
            TextFormat("%dx", sorting_speed),
            215,
            438,
            20,
            COL_TEXT);
    }

    //----------------------------------------
    // Controls
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 495, SIDEBAR_WIDTH - 30, 180},
        COL_CARD,
        COL_BORDER,
        COL_ORANGE);

    draw_title(
        30,
        508,
        "CONTROLS");

    int y = 540;

    draw_key(30, y, "SPACE");
    DrawText("Pause / Resume", 110, y + 4, 16, COL_TEXT);

    y += 30;

    draw_key(30, y, "R");
    DrawText("Shuffle", 110, y + 4, 16, COL_TEXT);

    y += 30;

    draw_key(30, y, "UP");
    DrawText("Increase Size", 110, y + 4, 16, COL_TEXT);

    y += 30;

    draw_key(30, y, "DOWN");
    DrawText("Decrease Size", 110, y + 4, 16, COL_TEXT);

    y += 30;

    draw_key(30, y, "+  -");
    DrawText("Speed", 110, y + 4, 16, COL_TEXT);

    //----------------------------------------
    // Algorithms
    //----------------------------------------

    draw_card_with_shadow(
        (Rectangle){15, 690, SIDEBAR_WIDTH - 30, 180},
        COL_CARD,
        COL_BORDER,
        COL_BLUE);

    draw_title(
        30,
        703,
        "ALGORITHMS");

    const char *algos[] = {
        "Bubble Sort",
        "Selection Sort",
        "Insertion Sort",
        "Merge Sort",
        "Quick Sort",
        "Heap Sort"};

    int ay = 735;

    for (int i = 0; i < 6; i++)
    {
        bool active =
            algorithm_selected(algos[i]);

        if (active)
        {
            DrawRectangleRounded(
                (Rectangle){
                    26,
                    ay - 4,
                    SIDEBAR_WIDTH - 52,
                    24},
                0.20f,
                8,
                COL_ACTIVE);

            DrawCircle(
                38,
                ay + 8,
                4,
                COL_BLUE);
        }

        DrawText(
            TextFormat("%d", i + 1),
            50,
            ay,
            16,
            active ? COL_BLUE : COL_MUTED);

        DrawText(
            algos[i],
            78,
            ay,
            16,
            active ? COL_BLUE : COL_TEXT);

        ay += 27;
    }

    //----------------------------------------
    // Footer
    //----------------------------------------

    DrawLine(
        18,
        HEIGHT - 42,
        SIDEBAR_WIDTH - 18,
        HEIGHT - 42,
        COL_BORDER);

    DrawText(
        TextFormat("FPS : %d", GetFPS()),
        30,
        HEIGHT - 30,
        14,
        COL_MUTED);

    DrawText(
        TextFormat("Time : %.2fs", metrics.elapsed_time),
        160,
        HEIGHT - 30,
        14,
        COL_MUTED);
}
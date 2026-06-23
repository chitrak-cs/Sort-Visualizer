#include <raylib.h>
#include <stdbool.h>
#include <string.h>

#include "metrics.h"
#include "algorithm_manager.h"
#include "renderer.h"
#include "array.h"
#include "sort_state.h"
#include "config.h"

static void draw_card(int x, int y, int w, int h, Color fill, Color border, Color accent)
{
    DrawRectangle(x + 3, y + 4, w, h, (Color){0, 0, 0, 60});
    DrawRectangleRounded((Rectangle){(float)x, (float)y, (float)w, (float)h}, 0.12f, 8, fill);
    DrawRectangle(x, y, w, 4, accent);
    DrawRectangleLinesEx((Rectangle){(float)x, (float)y, (float)w, (float)h}, 1.0f, border);
}

static void draw_row(int x, int y, const char *key, const char *value, Color keyColor, Color valueColor)
{
    DrawText(key, x, y, 16, keyColor);
    DrawText(value, x + 92, y, 16, valueColor);
}

static void draw_keycap(int x, int y, const char *key, Color fill, Color border, Color textColor)
{
    int keyWidth = MeasureText(key, 16) + 18;
    if (keyWidth < 52)
        keyWidth = 52;

    DrawRectangle(x, y - 2, keyWidth, 22, fill);
    DrawRectangleLines(x, y - 2, keyWidth, 22, border);
    DrawText(key, x + 8, y, 16, textColor);
}

static bool is_active_algorithm(const char *name)
{
    const char *current = get_algorithm_name();
    return (current != NULL && name != NULL && strcmp(current, name) == 0);
}

static void draw_algorithm_row(int x, int y, const char *hotkey, const char *name, bool active, Color muted, Color accent, Color activeFill)
{
    if (active)
    {
        DrawRectangle(x + 4, y - 2, SIDEBAR_WIDTH - 40, 24, activeFill);
        DrawRectangle(x - 8, y + 6, 5, 5, accent);
    }

    DrawText(hotkey, x, y, 16, active ? accent : muted);
    DrawText(name, x + 42, y, 16, active ? accent : muted);
}

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

        Color color = (Color){232, 232, 238, 255};

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
            color = status.swapped ? (Color){255, 90, 90, 255} : SKYBLUE;
        }

        DrawRectangle(
            (int)xpos + 2,
            (int)ypos + 2,
            (int)bar_width,
            (int)bar_height,
            (Color){0, 0, 0, 40});

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
                (Color){70, 70, 78, 255});
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
    const Color bg = (Color){18, 18, 22, 255};
    const Color panel = (Color){28, 30, 36, 255};
    const Color border = (Color){58, 60, 68, 255};
    const Color title = GOLD;
    const Color accent = SKYBLUE;
    const Color text = (Color){235, 235, 240, 255};
    const Color muted = (Color){155, 160, 172, 255};
    const Color success = (Color){0, 220, 90, 255};
    const Color warning = (Color){255, 180, 60, 255};
    const Color activeFill = (Color){35, 42, 56, 255};

    const int x = 14;
    const int w = SIDEBAR_WIDTH - 28;

    DrawRectangle(0, 0, SIDEBAR_WIDTH, HEIGHT, bg);
    DrawLine(SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, HEIGHT, border);

    // HEADER
    draw_card(x, 12, w, 84, panel, border, accent);
    DrawText("SORTING", 28, 26, 30, title);
    DrawText("VISUALIZER", 28, 58, 24, accent);
    DrawText("Press 1-6 to switch algorithms", 28, 84, 12, muted);

    // STATUS
    draw_card(x, 108, w, 128, panel, border, warning);
    DrawText("STATUS", 28, 120, 16, muted);

    DrawCircle(38, 156, 6, paused ? warning : success);
    DrawText(paused ? "PAUSED" : "RUNNING", 52, 144, 28, paused ? warning : success);

    DrawText("CURRENT ALGORITHM", 28, 182, 14, muted);
    DrawText(TextFormat("▶ %s", get_algorithm_name()), 28, 202, 18, accent);

    // STATISTICS
    draw_card(x, 248, w, 92, panel, border, success);
    DrawText("STATISTICS", 28, 260, 16, muted);

    DrawText("Comparisons", 28, 286, 14, muted);
    DrawText(TextFormat("%ld", metrics.comparisons), 180, 280, 22, text);

    DrawText("Swaps", 28, 314, 14, muted);
    DrawText(TextFormat("%ld", metrics.swaps), 180, 308, 22, text);

    // PLAYBACK
    draw_card(x, 352, w, 92, panel, border, title);
    DrawText("PLAYBACK", 28, 364, 16, muted);

    DrawText("Array Size", 28, 390, 14, muted);
    DrawText(TextFormat("%d", count), 180, 384, 22, text);

    DrawText("Speed", 28, 418, 14, muted);
    if (sorting_speed == 0)
    {
        DrawText("MANUAL", 180, 412, 22, warning);
    }
    else
    {
        DrawText(TextFormat("%dx", sorting_speed), 180, 412, 22, text);
    }

    // CONTROLS
    draw_card(x, 456, w, 188, panel, border, accent);
    DrawText("CONTROLS", 28, 468, 16, muted);

    draw_keycap(28, 496, "SPACE", (Color){40, 44, 52, 255}, border, text);
    DrawText("Pause / Resume", 118, 496, 16, text);

    draw_keycap(28, 524, "R", (Color){40, 44, 52, 255}, border, text);
    DrawText("Shuffle", 118, 524, 16, text);

    draw_keycap(28, 552, "UP / DOWN", (Color){40, 44, 52, 255}, border, text);
    DrawText("Array Size", 118, 552, 16, text);

    draw_keycap(28, 580, "+ / -", (Color){40, 44, 52, 255}, border, text);
    DrawText("Speed", 118, 580, 16, text);

    draw_keycap(28, 608, "N", (Color){40, 44, 52, 255}, border, text);
    DrawText("Next Step", 118, 608, 16, text);

    draw_keycap(28, 636, "1 - 6", (Color){40, 44, 52, 255}, border, text);
    DrawText("Switch Algorithm", 118, 636, 16, text);

    // ALGORITHMS
    draw_card(x, 656, w, 224, panel, border, warning);
    DrawText("ALGORITHMS", 28, 668, 16, muted);

    const char *names[] = {
        "Bubble Sort",
        "Selection Sort",
        "Insertion Sort",
        "Merge Sort",
        "Quick Sort",
        "Heap Sort"};

    const char *keys[] = {
        "[1]",
        "[2]",
        "[3]",
        "[4]",
        "[5]",
        "[6]"};

    int row_y = 696;
    for (int i = 0; i < 6; i++)
    {
        bool active = is_active_algorithm(names[i]);
        draw_algorithm_row(28, row_y, keys[i], names[i], active, muted, accent, activeFill);
        row_y += 28;
    }

    DrawText("Current selection is highlighted", 28, 858, 12, muted);
}
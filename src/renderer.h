#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include "sort_state.h"

void draw_bars(sort_status status);
void draw_ui(bool paused, int sorting_speed);

#endif
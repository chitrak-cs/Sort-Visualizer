#ifndef SORT_STATE_H
#define SORT_STATE_H

#include <stdbool.h>

typedef enum
{
    BS_COMPARE,
    BS_SWAP,
    BS_ADVANCE,
    BS_MARK,
    BS_DONE
} SortState;

typedef struct
{
    int i, j;
    bool swapped;
    bool done;
} sort_status;

sort_status sort_step(void);
void reset_sort(void);

int get_finish_animation_index(void);

extern int sort_n;

#endif
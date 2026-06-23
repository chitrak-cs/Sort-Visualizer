#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../sort_state.h"

typedef struct
{
    const char *name;

    sort_status (*step)(void);

    void (*reset)(void);

} Algorithm;

#endif
#ifndef ALGORITHM_MANAGER_H
#define ALGORITHM_MANAGER_H

#include "algorithms/algorithm.h"

extern Algorithm *current_algorithm;

void set_algorithm(Algorithm *algo);

const char *get_algorithm_name(void);

#endif
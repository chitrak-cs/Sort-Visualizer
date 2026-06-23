#include <stddef.h>

#include "algorithm_manager.h"

Algorithm *current_algorithm = NULL;

void set_algorithm(Algorithm *algo)
{
    current_algorithm = algo;
}

const char *get_algorithm_name(void)
{
    if (current_algorithm == NULL)
        return "None";

    return current_algorithm->name;
}
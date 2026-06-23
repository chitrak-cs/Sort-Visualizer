#include "metrics.h"

Metrics metrics =
    {
        0,
        0,
        0.0,
        0};

void reset_metrics(void)
{
    metrics.comparisons = 0;
    metrics.swaps = 0;
    metrics.elapsed_time = 0.0;
    metrics.frames = 0;
}
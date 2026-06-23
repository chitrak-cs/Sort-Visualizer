#ifndef METRICS_H
#define METRICS_H

typedef struct
{
    long comparisons;
    long swaps;

    double elapsed_time;

    int frames;
} Metrics;

extern Metrics metrics;

void reset_metrics(void);

#endif
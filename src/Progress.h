#pragma once

#include <stdbool.h>
#include <time.h>
#include "Config.h"


typedef struct sProgress {
    int max;  // the maximum value
    int current;  // the current value
    char *task;  // the task
    bool running;  // whether the task is running

    clock_t last_clock;  // the last clock
    int last_current;  // the last current value
} Progress;

/**
 * @brief Initialize the progress
 * @param progress The progress to initialize
 * @param max The maximum value
 * @param current The current value
 * @param task The task
 */
void init_progress(Progress *progress, const int max, const int current, char *task);

/**
 * @brief Update the progress
 * @param progress The progress to update
 * @param current The current value
 */
void update_progress(Progress *progress, const int current);

/**
 * @brief Finish the progress
 * @param progress The progress to finish
 */
void finalize_progress(Progress *progress);

/**
 * @brief Print the bamboubrain logo
 */
void print_logo();
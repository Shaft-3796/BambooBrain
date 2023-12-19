#pragma once

#include "../Dataset.h"

/**
 * @brief BaggingMode the mode of the bagging function
 * BAGGING_MODE_PROPORTIONAL: Generate a subproblem by randomly selecting a proportion of instances
 */
typedef enum BaggingMode {
    BAGGING_MODE_PROPORTIONAL,
} BaggingMode;

/**
 * @brief BaggingArgs the arguments for the bagging function
 */
typedef struct BaggingArgs {
} BaggingArgs;

/**
 * @brief BaggingConfig the configuration for the bagging function
 * @param mode the mode of the bagging function
 * @param bagging_function the function to call to generate dataset bagging
 * @param proportion used to choose the proportion of instances to select
 * For BAGGING_MODE_PROPORTIONAL
 */
typedef struct BaggingConfig {
    const BaggingMode mode;
    Subproblem** (*bagging_function)(const struct BaggingConfig *config, const BaggingArgs *args, const Dataset *data, int count);

    float proportion;
} BaggingConfig;

/**
 * @brief Generate a subproblem by randomly selecting a proportion of instances (BAGGING_MODE_PROPORTIONAL)
 * @param config the configuration for the bagging function
 * - proportion: the proportion of instances to select
 * @param args mode specific arguments for the bagging function
 * No arguments are expected.
 * @param data the dataset
 * @param count the number of bags
 * @return a list of subproblems
 */
Subproblem **bagging_from_proportion(const BaggingConfig *config, const BaggingArgs *args, const Dataset *data, int count);
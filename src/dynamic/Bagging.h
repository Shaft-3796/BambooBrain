#pragma once

#include "../Dataset.h"

/**
 * @brief BaggingMode the mode of the bagging function
 * BAGGING_MODE_PROPORTION: Generate a subproblem by randomly selecting a proportion of instances
 */
typedef enum BaggingMode {
    BAGGING_MODE_PROPORTION,
} BaggingMode;

/**
 * @brief BaggingArgs the arguments for the bagging function
 * Common:
 * @param data the dataset
 * @param bags the number of bags
 * Specific:
 * @param proportion used for BAGGING_MODE_PROPORTION to choose the proportion of instances to select
 */
typedef struct BaggingArgs {
    BaggingMode mode;
    /* Common arguments */
    Dataset *data;
    int bags;
    /* Specific arguments */
    float proportion;
} BaggingArgs;

/**
 * @brief Generate subproblems by bagging a dataset
 * @param args the arguments for the bagging function
 * @return a list of subproblems
 */
Subproblem **bagg_dataset(const BaggingArgs *args);

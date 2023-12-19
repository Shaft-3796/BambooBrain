#pragma once

#include "../Dataset.h"
#include "Threshold.h"
#include "Impurity.h"

/**
 * @brief ComputeSplitMode the mode of the split_compute function
 * COMPUTE_SPLIT_MODE_PUREST_THRESHOLD: the threshold that gives the purest split
 */
typedef enum ComputeSplitMode {
    COMPUTE_SPLIT_MODE_PUREST_THRESHOLD,
} ComputeSplitMode;

/**
 * @brief ComputeSplitArgs the arguments for the compute_split function
 * Common:
 * @param sp the subproblem
 * Specific:
 * @param threshold_args used for COMPUTE_SPLIT_MODE_PUREST_THRESHOLD to compute the threshold
 * @param impurity_args used for COMPUTE_SPLIT_MODE_PUREST_THRESHOLD to compute the impurity
 */
typedef struct ComputeSplitArgs {
    ComputeSplitMode mode;
    /* Common arguments */
    Subproblem* sp;
    /* Specific arguments */
    ThresholdArgs *threshold_args;
    ImpurityArgs *impurity_args;
} ComputeSplitArgs;

/**
 * @brief compute_split computes the best split for a subproblem
 * @param args the arguments for the split_compute function
 * @return the split
 */
Split compute_split(const ComputeSplitArgs *args);

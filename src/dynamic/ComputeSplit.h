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
 */
typedef struct ComputeSplitArgs {
} ComputeSplitArgs;

/**
 * @brief ComputeSplitConfig the configuration for the ComputeSplit function
 * @param mode the mode of the split_compute function
 * @param compute_split_function the function to call to compute the split
 * @param threshold_config the configuration for the threshold function
 * For COMPUTE_SPLIT_MODE_PUREST_THRESHOLD
 * @param impurity_config the configuration for the impurity function
 * For COMPUTE_SPLIT_MODE_PUREST_THRESHOLD
 */
typedef struct ComputeSplitConfig {
    const ComputeSplitMode mode;
    Split (*compute_split_function)(const struct ComputeSplitConfig *config, const ComputeSplitArgs *args, const Subproblem *sp);

    ThresholdConfig *threshold_config;
    ImpurityConfig *impurity_config;
} ComputeSplitConfig;


/**
 * @brief compute_purest_threshold_split compute the best split for a subproblem based on the purest threshold (COMPUTE_SPLIT_MODE_PUREST_THRESHOLD)
 * @param config the configuration for the split_compute function
 * - threshold_config: the configuration for the threshold function
 * - impurity_config: the configuration for the impurity function
 * @param args mode specific arguments for the split_compute function
 * No arguments are expected.
 * @param sp the subproblem
 * @return the split
 */
Split compute_purest_threshold_split(const ComputeSplitConfig *config, const ComputeSplitArgs *args, const Subproblem *sp);
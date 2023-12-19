#pragma once

#include "../Dataset.h"

/**
 * @brief ThresholdMode the mode of the threshold function
 * THRESHOLD_MODE_MID_MIN_MAX: (minFeatureValue + maxFeatureValue) / 2
 */
typedef enum Threshold {
    THRESHOLD_MODE_MID_MIN_MAX,
} ThresholdMode;

/**
 * @brief ThresholdArgs the arguments for the threshold function
 */
typedef struct ThresholdArgs {
} ThresholdArgs;

/**
 * @brief ThresholdConfig the configuration for the threshold function
 * @param mode the mode of the threshold function
 * @param threshold_function the function to call to generate dataset threshold
 */
typedef struct ThresholdConfig {
    const ThresholdMode mode;
    float (*threshold_function)(const struct ThresholdConfig *config, const ThresholdArgs *args, const Subproblem *sp, int feature_id);
} ThresholdConfig;


/**
 * @brief Computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2 (THRESHOLD_MODE_MID_MIN_MAX)
 * @param config the configuration for the threshold function
 * @param args mode specific arguments for the threshold function
 * No arguments are expected.
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @return the threshold
 */
float get_subproblem_threshold_min_max(
    const ThresholdConfig *config,
    const ThresholdArgs *args,
    const Subproblem *sp,
    int feature_id
    );
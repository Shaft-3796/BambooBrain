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
 * Common:
 * @param sp the subproblem
 * Specific:
 * @param featureID used for THRESHOLD_MODE_MID_MIN_MAX to choose the feature.
 */
typedef struct ThresholdArgs {
    ThresholdMode mode;
    /* Common arguments */
    Subproblem *sp;
    /* Specific arguments */
    int feature_id;
} ThresholdArgs;

/**
 * @brief return a threshold for a given subproblem
 * @param args the arguments for the threshold function
 * @return the threshold
 */
float get_subproblem_threshold(const ThresholdArgs *args);

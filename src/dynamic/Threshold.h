#pragma once

#include "../Config.h"
#include "../Dataset.h"


/**
 * @brief Computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2 (THRESHOLD_MODE_MID_MIN_MAX)
 * @param config the configuration for the threshold function
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @return the threshold in a float array
 */
float *get_subproblem_threshold_min_max(const Config *config, const Subproblem *sp, int feature_id);

/**
 * @brief Computes all the thresholds to use for a given feature based on all values between min and max with a step (THRESHOLD_MODE_ALL_VALUES)
 * @param config the configuration for the threshold function
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @return the threshold in a float array
 */
float *get_subproblem_threshold_all_values(const Config *config, const Subproblem *sp, int feature_id);
#pragma once

#include "Config.h"
#include "Dataset.h"
#include "Threshold.h"
#include "Impurity.h"


/**
 * @brief compute_purest_feature_split compute the best split for a subproblem based on the purest feature (COMPUTE_SPLIT_MODE_PUREST_FEATURE)
 * @param config the configuration for the split_compute function
 * - threshold_config: the configuration for the threshold function
 * - impurity_config: the configuration for the impurity function
 * @param sp the subproblem
 * @return the split
 */
Split compute_purest_feature_split(const Config *config, const Subproblem *sp);
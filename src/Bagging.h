#pragma once

#include "Config.h"
#include "Dataset.h"


/**
 * @brief Generate subproblems by randomly selecting a proportion of instances (BAGGING_MODE_PROPORTIONAL)
 * @param config the configuration for the bagging function
 * - proportion: the proportion of instances to select
 * @param data the dataset
 * @param count the number of bags
 * @return a list of subproblems
 */
Subproblem **bagging_from_proportion(const Config *config, const Dataset *data, int count);

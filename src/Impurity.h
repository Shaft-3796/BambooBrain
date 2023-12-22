#pragma once

#include "Config.h"
#include "Dataset.h"
#include "SplitTools.h"


/**
 * @brief Computes the gini impurity of a subproblem given a feature and a threshold (IMPURITY_MODE_GINI)
 * @param config the configuration for the impurity function
 * @param sp the subproblem
 * @param feature_id the feature id
 * @param threshold the threshold
 * @return the gini impurity
*/
float gini_impurity(const Config *config, const Subproblem *sp, int feature_id, int threshold);
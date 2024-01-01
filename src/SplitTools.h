#pragma once

#include "Dataset.h"
#include "dynamic/Impurity.h"
#include "dynamic/Threshold.h"

/**
 * @brief split_subproblem splits a subproblem into 2 subproblems based on a feature and a threshold
 * @param sp the subproblem to split
 * @param feature_id the feature to split on
 * @param threshold the threshold to split on
 * @param spl the left subproblem
 * @param spr the right subproblem
 */
void split_subproblem(const Subproblem *sp, int feature_id, float threshold, Subproblem *spl, Subproblem *spr);
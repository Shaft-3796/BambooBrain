#ifndef SPLIT_H
#define SPLIT_H

#include "Dataset.h"

typedef struct sSplit{
    int featureID;
    float threshold;
} Split;

/**
 * @brief Split_gini computes the gini impurity of a subproblem given a feature and a threshold
 * @param sp the subproblem
 * @param featureID the feature to split on
 * @param threshold the threshold to split on
*/
float Split_gini(Subproblem *sp, int featureID, float threshold);

/**
 * @brief Split_threshold computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2
 * @param sp the subproblem
 * @param featureID the feature to split on
 * @return the threshold
 */
float Split_threshold(Subproblem *sp, int featureID);

/**
 * @brief Split_compute computes the best split for a subproblem
 * @param sp the subproblem
 * @return the split
 */
Split Split_compute(Subproblem *sp);

#endif //SPLIT_H

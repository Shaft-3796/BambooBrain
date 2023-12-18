#pragma once
#include "../Dataset.h"
#include "../Split.h"

/**
 * @brief ImpurityMode the mode of the impurity function
 * BAGGING_MODE_GINI: Gini impurity
 */
typedef enum ImpurityMode {
    IMPURITY_MODE_GINI,
} ImpurityMode;

/**
 * @brief ImpurityArgs the arguments for the impurity function
 * Common:
 * @param sp the subproblem
 * Specific:
 * @param featureID used for IMPURITY_MODE_GINI to choose the feature to split on
 * @param trehshold used for IMPURITY_MODE_GINI to choose the threshold to split on
 */
typedef struct ImpurityArgs {
    ImpurityMode mode;
    /* Common arguments */
    Subproblem* sp;
    /* Specific arguments */
    int feature_id;
    float threshold;
} ImpurityArgs;

/**
 * @brief Get the impurity of a subproblem
 * @param args the arguments for the impurity function
 * @return the impurity between 0 and 1
 */
float get_subproblem_impurity(const ImpurityArgs *args);

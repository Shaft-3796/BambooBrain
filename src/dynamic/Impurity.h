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
 * @param feature_id used to choose the feature to split on
 * For IMPURITY_MODE_GINI
 * @param trehshold used to choose the threshold to split on
 * For IMPURITY_MODE_GINI
 */
typedef struct ImpurityArgs {
    int feature_id;
    float threshold;
} ImpurityArgs;

/**
 * @brief ImpurityConfig the configuration for the Impurity function
 * @param mode the mode of the impurity function
 * @param impurity_function the function to call to compute impurity
 */
typedef struct ImpurityConfig {
    const ImpurityMode mode;
    float (*impurity_function)(const struct ImpurityConfig *config, const ImpurityArgs *args, const Subproblem *sp);
} ImpurityConfig;


/**
 * @brief Computes the gini impurity of a subproblem given a feature and a threshold (IMPURITY_MODE_GINI)
 * @param config the configuration for the impurity function
 * @param args mode specific arguments for the impurity function
 * - feature_id: the feature to split on
 * - threshold: the threshold to split on
 * @param sp the subproblem
 * @return the gini impurity
*/
float gini_impurity(const ImpurityConfig *config, const ImpurityArgs *args, const Subproblem *sp);
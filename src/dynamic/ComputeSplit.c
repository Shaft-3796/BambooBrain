#include "ComputeSplit.h"

/**
 * @brief compute_purest_feature_split compute the best split for a subproblem based on the purest feature (COMPUTE_SPLIT_MODE_PUREST_FEATURE)
 * @param config the configuration for the split_compute function
 * - threshold_config: the configuration for the threshold function
 * - impurity_config: the configuration for the impurity function
 * @param args mode specific arguments for the split_compute function
 * No arguments are expected.
 * @param sp the subproblem
 * @return the split
 */
Split compute_purest_feature_split(const ComputeSplitConfig *config, const ComputeSplitArgs *args, const Subproblem *sp) {
    Split split = {0, 0.0};
    float impurity = 1.0;

    for(int feature_id=0; feature_id < sp->feature_count; feature_id++) {

        // Compute the threshold for the feature
        ThresholdArgs threshold_args = {};
        const float _threshold = config->threshold_config->threshold_function(config->threshold_config, &threshold_args, sp, feature_id);

        ImpurityArgs impurity_args = {.feature_id=feature_id, .threshold=_threshold};
        const float _impurity = config->impurity_config->impurity_function(config->impurity_config, &impurity_args, sp);

        if(_impurity <= impurity) {
            split.feature_id = feature_id; split.threshold = _threshold;
            impurity = _impurity;
        }
    }

    return split;
}
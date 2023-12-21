#include "ComputeSplit.h"

/**
 * @brief compute_purest_feature_split compute the best split for a subproblem based on the purest feature (COMPUTE_SPLIT_MODE_PUREST_FEATURE)
 * @param config the configuration for the split_compute function
 * - threshold_config: the configuration for the threshold function
 * - impurity_config: the configuration for the impurity function
 * @param sp the subproblem
 * @return the split
 */
Split compute_purest_feature_split(const Config *config, const Subproblem *sp) {
    Split split = {0, 0.0};
    float impurity = 1.0;

    for(int feature_id=0; feature_id < sp->feature_count; feature_id++) {
        // Compute the threshold for the feature
        float *thresholds = config->get_thresholds(config, sp, feature_id);

        for(int threshold_id=0; thresholds[threshold_id] != -1.0; threshold_id++) {
            const float _impurity = config->get_impurity(config, sp, feature_id, thresholds[threshold_id]);

            if(_impurity <= impurity) {
                split.feature_id = feature_id; split.threshold = thresholds[threshold_id];
                impurity = _impurity;
            }
        }
        free(thresholds);
    }

    return split;
}
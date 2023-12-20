#include "Threshold.h"

/**
 * @brief Computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2 (THRESHOLD_MODE_MID_MIN_MAX)
 * @param config the configuration for the threshold function
 * @param args mode specific arguments for the threshold function
 * No arguments are expected.
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @return the threshold
 */
float get_subproblem_threshold_min_max(const ThresholdConfig *config, const ThresholdArgs *args, const Subproblem *sp, const int feature_id) {
    if(!sp->instance_count) {printf("Error: get_subproblem_threshold_min_max: subproblem has no instances\n"); exit(1);}

    int min = sp->instances[0]->values[feature_id], max = sp->instances[0]->values[feature_id];
    for(int instance_id=0; instance_id < sp->instance_count; instance_id++) {
        const int value = sp->instances[instance_id]->values[feature_id];
        if(value < min) min = value;
        if(value > max) max = value;
    }
    return (float)(min + max) / 2.0;
}

#include "Threshold.h"

/**
 * @brief Computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @return the threshold
 */
static float mid_min_max_threshold(const Subproblem *sp, const int feature_id) {
    int min = 255, max = 0;
    for(int instance_id=0; instance_id < sp->instance_count; instance_id++) {
        const int value = sp->instances[instance_id]->values[feature_id];
        if(value < min) min = value;
        if(value > max) max = value;
    }
    return (float)(min + max) / 2.0;
}

/**
 * @brief return a threshold for a given subproblem
 * @param args the arguments for the threshold function
 * @return the threshold
 */
float get_subproblem_threshold(const ThresholdArgs *args){

    switch(args->mode) {
        case THRESHOLD_MODE_MID_MIN_MAX:
            return mid_min_max_threshold(args->sp, args->feature_id);
        default:
            printf("Error: unknown threshold mode\n");
        return 1;
    }
}
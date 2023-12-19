#include "ComputeSplit.h"

/**
 * @brief compute_purest_threshold_split compute the best split for a subproblem based on the purest threshold
 * @param sp the subproblem
 * @param threshold_args the arguments for the threshold function
 * @param impurity_args the arguments for the impurity function
 * @return the split
 */
static Split compute_purest_threshold_split(Subproblem *sp, ThresholdArgs *threshold_args, ImpurityArgs *impurity_args) {
    Split split = {0, 0.0};
    float impurity = 1.0;

    for(int feature_id=0; feature_id < sp->feature_count; feature_id++) {

        threshold_args->sp = sp;
        threshold_args->feature_id = feature_id;
        const float _threshold = get_subproblem_threshold(threshold_args);

        impurity_args->feature_id = feature_id; impurity_args->threshold = _threshold;
        impurity_args->sp = sp;
        const float _impurity = get_subproblem_impurity(impurity_args);

        if(_impurity <= impurity) {
            split.feature_id = feature_id; split.threshold = _threshold;
            impurity = _impurity;
        }
    }

    return split;
}

/**
 * @brief compute_split computes the best split for a subproblem
 * @param args the arguments for the split_compute function
 * @return the split
 */
Split compute_split(const ComputeSplitArgs *args) {
    switch(args->mode) {
        case COMPUTE_SPLIT_MODE_PUREST_THRESHOLD:
            return compute_purest_threshold_split(args->sp, args->threshold_args, args->impurity_args);
        default:
            printf("Error: unknown split mode\n");
        return (Split){0, 0.0};
    }
}
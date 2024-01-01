#include "SplitTools.h"

/**
 * @brief split_subproblem splits a subproblem into 2 subproblems based on a feature and a threshold
 * @param sp the subproblem to split
 * @param feature_id the feature to split on
 * @param threshold the threshold to split on
 * @param spl the left subproblem
 * @param spr the right subproblem
 */
void split_subproblem(const Subproblem *sp, const int feature_id, const float threshold, Subproblem *spl, Subproblem *spr) {
    for (int i = 0; i < sp->instance_count; ++i) {
        (float)sp->instances[i]->values[feature_id] <= threshold ?
            insert_subproblem_instance(spl, sp->instances[i]) :
            insert_subproblem_instance(spr, sp->instances[i]);
    }

    // RAM optimization
    free_subproblem_excess_memory(spl);
    free_subproblem_excess_memory(spr);
}


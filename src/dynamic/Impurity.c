#include "Impurity.h"

static float gini_compute_impurity(const Subproblem *sp) {
    if (sp->instance_count == 0) return 0.0;  // Perfectly pure subproblem

    float g = 1.0;
    for(int class_id=0; class_id < sp->class_count; ++class_id) {
        const float p = (float)sp->classes[class_id].instance_count / (float)sp->instance_count;
        g -= p * p;
    }
    return g;
}

/**
 * @brief Computes the gini impurity of a subproblem given a feature and a threshold (IMPURITY_MODE_GINI)
 * @param config the configuration for the impurity function
 * @param sp the subproblem
 * @param feature_id the feature id
 * @param threshold the threshold
 * @return the gini impurity
*/
float gini_impurity(const Config *config, const Subproblem *sp, const int feature_id, const int threshold) {
    if(sp->instance_count==0) return 0.0; // Perfectly pure subproblem

    Subproblem *spl = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    Subproblem *spr = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);

    split_subproblem(sp, feature_id, threshold, spl, spr);

    /* |spl| + |spr| = |sp| => (|spl|+|spr|)/|sp| = 1 => (|spl|/|sp|) + (|spr|/|sp|) = 1
    * With 0 <= g(spl) <= 1 and 0 <= g(spr) <= 1, we have 0 <= (|spl|/|sp|)*g(spl) + (|spr|/|sp|)*g(spr) <= 1
    * So 0 <= gini <= 1
    */

    const float r = (float)spl->instance_count/(float)sp->instance_count * gini_compute_impurity(spl) +
           (float)spr->instance_count/(float)sp->instance_count * gini_compute_impurity(spr);

    destroy_subproblem(spl);
    destroy_subproblem(spr);
    return r;
}
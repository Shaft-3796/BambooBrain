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
 * @brief Computes the gini impurity of a subproblem given a feature and a threshold
 * @param sp the subproblem
 * @param feature_id the feature to split on
 * @param threshold the threshold to split on
 * @return the gini impurity
*/
static float gini_impurity(const Subproblem *sp, const int feature_id, const float threshold) {
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

/**
 * @brief Get the impurity of a subproblem
 * @param args the arguments for the impurity function
 * @return the impurity between 0 and 1
 */
float get_subproblem_impurity(const ImpurityArgs *args) {
    switch(args->mode) {
        case IMPURITY_MODE_GINI:
            return gini_impurity(args->sp, args->feature_id, args->threshold);
        default:
            printf("Error: unknown impurity mode\n");
            return 1;
    }
}
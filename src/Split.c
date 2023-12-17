#include "Split.h"

/**
 * @brief Split_subproblem splits a subproblem into 2 subproblems based on a feature and a threshold
 * @param sp the subproblem to split
 * @param featureID the feature to split on
 * @param threshold the threshold to split on
 * @param spl the left subproblem
 * @param spr the right subproblem
 */
void Split_subproblem(Subproblem *sp, int featureID, float threshold, Subproblem *spl, Subproblem *spr) {
    for (int i = 0; i < sp->instanceCount; ++i) {
        (float)sp->instances[i]->values[featureID] <= threshold ?
            Subproblem_insert(spl, sp->instances[i]) :
            Subproblem_insert(spr, sp->instances[i]);
    }
}

static float split_compute_impurity(Subproblem *sp) {
    if (sp->instanceCount == 0) return 0.0;  // Perfectly pure subproblem

    float g = 1.0;
    for(int classID=0; classID < sp->classCount; ++classID) {
        const float p = (float)sp->classes[classID].instanceCount / (float)sp->instanceCount;
        g -= p * p;
    }
    return g;
}

/**
 * @brief Split_gini computes the gini impurity of a subproblem given a feature and a threshold
 * @param sp the subproblem
 * @param featureID the feature to split on
 * @param threshold the threshold to split on
 * @return the gini impurity
*/
float Split_gini(Subproblem *sp, int featureID, float threshold) {
    if(sp->instanceCount==0) return 0.0; // Perfectly pure subproblem

    Subproblem *spl = Subproblem_create(sp->instanceCount, sp->featureCount, sp->classCount);
    Subproblem *spr = Subproblem_create(sp->instanceCount, sp->featureCount, sp->classCount);

    Split_subproblem(sp, featureID, threshold, spl, spr);

    /* |spl| + |spr| = |sp| => (|spl|+|spr|)/|sp| = 1 => (|spl|/|sp|) + (|spr|/|sp|) = 1
    * With 0 <= g(spl) <= 1 and 0 <= g(spr) <= 1, we have 0 <= (|spl|/|sp|)*g(spl) + (|spr|/|sp|)*g(spr) <= 1
    * So 0 <= gini <= 1
    */

    const float r = (float)spl->instanceCount/(float)sp->instanceCount * split_compute_impurity(spl) +
           (float)spr->instanceCount/(float)sp->instanceCount * split_compute_impurity(spr);

    Subproblem_destroy(spl);
    Subproblem_destroy(spr);
    return r;
}

/**
 * @brief Split_threshold computes the threshold to use for a given feature based on (minFeatureValue + maxFeatureValue) / 2
 * @param sp the subproblem
 * @param featureID the feature to split on
 * @return the threshold
 */
float Split_threshold(Subproblem *sp, int featureID) {
    int min = 255, max = 0;
    for(int instanceID=0; instanceID < sp->instanceCount; instanceID++) {
        const int value = sp->instances[instanceID]->values[featureID];
        if(value < min) min = value;
        if(value > max) max = value;
    }
    return (float)(min + max) / 2.0;
}

/**
 * @brief Split_compute computes the best split for a subproblem
 * @param sp the subproblem
 * @return the split
 */
Split Split_compute(Subproblem *sp) {
    Split split = {0, 0.0};
    float impurity = 1.0;

    for(int featureID=0; featureID < sp->featureCount; featureID++) {
        const float _threshold = Split_threshold(sp, featureID);
        const float _impurity = Split_gini(sp, featureID, _threshold);
        if(_impurity <= impurity) {
            split.featureID = featureID; split.threshold = _threshold;
            impurity = _impurity;
        }
    }

    return split;
}

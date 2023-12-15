//
// Created by loe on 14/12/23.
//

#include "Split.h"
#include <limits.h>

float Split_impurity(Subproblem *sp) {

    // Handle division by zero
    if (sp->instanceCount == 0) return 0; // !TODO

    float impurity = 1.0;
    float temp;
    for (int i = 0; i < sp->classCount; ++i) {
        temp = (float)sp->classes[i].instanceCount  / (float)sp->instanceCount;
        impurity -= temp * temp;
    }

    return impurity;
}

float Split_threshold(Subproblem *sp, int featureID) {
    int minj = INT_MAX, maxj = INT_MIN, value;

    for (int i = 0; i < sp->instanceCount; ++i) {
        value = sp->instances[i]->values[featureID];
        if (value < minj) minj = value;
        if (value > maxj) maxj = value;
    }

    return (float)(minj + maxj) / 2.0;
}

float Split_gini(Subproblem *sp, int featureID, float threshold) {
    Subproblem *spl = Subproblem_create(1, sp->featureCount, sp->classCount);
    Subproblem *spr = Subproblem_create(1, sp->featureCount, sp->classCount);

    // Split sp into 2 sub problems
    for (int i = 0; i < sp->instanceCount; ++i) {
        int value = sp->instances[i]->values[featureID];
        if ((float)value <= threshold) {
            Subproblem_insert(spl, sp->instances[i]);
        }
        else {
            Subproblem_insert(spr, sp->instances[i]);
        }
    }


    // Handle division by zero
    if (!sp->instanceCount || !((float)spr->instanceCount/(float)sp->instanceCount) * Split_impurity(spr)) {
        return 1;
    }

    float gini = ((float)spl->instanceCount/(float)sp->instanceCount) * Split_impurity(spl) +
                 ((float)spr->instanceCount/(float)sp->instanceCount) * Split_impurity(spr);


    // Clamp the result between 0 and 1
    if (gini > 1) return 1;
    if (gini < 0) return 0;
    return gini;
}

Split Split_compute(Subproblem *subproblem) {
    Split *split = (Split*) calloc(1, sizeof(Split));
    split->featureID = -1;
    float minGini = INT_MAX;

    // Test each feature and get the minimum split
    for (int i = 0; i < subproblem->featureCount; ++i) {
        float threshold = Split_threshold(subproblem, i);
        float gini = Split_gini(subproblem, i, threshold);
        if (gini <= minGini) {
            minGini = gini;
            split->threshold = threshold;
            split->featureID = i;
        }
    }

    return *split;
}
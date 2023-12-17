//
// Created by loe on 14/12/23.
//

#ifndef SPLIT_H
#define SPLIT_H
#include "Dataset.h"

typedef struct sSplit{
    int featureID;
    float threshold;
} Split;

float Split_gini(Subproblem *sp, int featureID, float threshold);
float Split_threshold(Subproblem *sp, int featureID);
Split Split_compute(Subproblem *subproblem);

#endif //SPLIT_H

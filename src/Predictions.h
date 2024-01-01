#pragma once
#include <stdlib.h>

typedef struct sPredictions {
    float *predictions;  // the predictions for each instance
    int prediction_count;  // the number of predictions
    int main_prediction; // the main prediction
} Predictions;

void destroy_predictions(Predictions *predictions);
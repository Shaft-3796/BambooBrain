#include "Predictions.h"

void destroy_predictions(Predictions *predictions) {
    if(predictions && predictions->predictions) free(predictions->predictions);
    if(predictions) free(predictions);
}
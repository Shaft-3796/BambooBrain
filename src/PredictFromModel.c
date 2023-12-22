#include "PredictFromModel.h"

/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority (PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY)
 * @param config the configuration for the predict_from_model function
 * - predict_from_tree_config: the configuration for the predict_from_tree function
 * @param model the model
 * @param instance the instance
* @return the predictions with only main_prediction or everything depending on the configuration
 */
Predictions *predict_from_random_forest_majority(const Config *config, const Model *model, const Instance *instance) {
    int *votes = (int*) calloc(model->class_count, sizeof(int));

    Predictions *final_predictions = (Predictions*) calloc(1, sizeof(Predictions));

    for(int i=0; i<model->tree_count; ++i) {
        const Predictions *predictions = config->predict_from_tree(config, model->trees[i], instance);

        // Parse the predictions
        votes[predictions->main_prediction]++;

        if(predictions->predictions) {
            if(!final_predictions->predictions) {
                final_predictions->predictions = (float*) calloc(predictions->prediction_count, sizeof(float));
                final_predictions->prediction_count = predictions->prediction_count;
            }
            for(int j=0; j<predictions->prediction_count; ++j) {
                final_predictions->predictions[j] += predictions->predictions[j];
            }
        }

        free(predictions->predictions);
    }

    int class_id = 0;
    for(int i=0; i<model->class_count; ++i) {
        if(votes[i] > votes[class_id]) class_id = i;
    }

    final_predictions->main_prediction = class_id;

    if(final_predictions->predictions) {
        for(int i=0; i<model->class_count; ++i) {
            final_predictions->predictions[i] /= model->tree_count;
        }
    }

    free(votes);
    return final_predictions;
}

/**
* @brief predict_from_tree predicts the class of an instance based on a single tree (PREDICT_FROM_MODEL_MODE_SINGLE_TREE)
* @param config the configuration for the predict_from_model function
* - predict_from_tree_config: the configuration for the predict_from_tree function
* @param model the model
* @param instance the instance
* @return the class id
* @return the predictions with only main_prediction or everything depending on the configuration
 */
Predictions *predict_from_model_single_tree(const Config *config, const Model *model, const Instance *instance) {
    return config->predict_from_tree(config, model->tree, instance);
}

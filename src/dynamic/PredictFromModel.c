#include "PredictFromModel.h"

/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority (PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY)
 * @param config the configuration for the predict_from_model function
 * - predict_from_tree_config: the configuration for the predict_from_tree function
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
int predict_from_random_forest_majority(const Config *config, const Model *model, const Instance *instance) {
    int *votes = (int*) calloc(model->class_count, sizeof(int));

    for(int i=0; i<model->tree_count; ++i) {
        const int prediction = config->predict_from_tree(config, model->trees[i], instance);
        votes[prediction]++;
    }

    int class_id = 0;
    for(int i=0; i<model->class_count; ++i) {
        if(votes[i] > votes[class_id]) class_id = i;
    }

    free(votes);
    return class_id;
}

/**
* @brief predict_from_tree predicts the class of an instance based on a single tree (PREDICT_FROM_MODEL_MODE_SINGLE_TREE)
* @param config the configuration for the predict_from_model function
* - predict_from_tree_config: the configuration for the predict_from_tree function
* @param model the model
* @param instance the instance
* @return the class id
*/
int predict_from_model_single_tree(const Config *config, const Model *model, const Instance *instance) {
    return config->predict_from_tree(config, model->tree, instance);
}

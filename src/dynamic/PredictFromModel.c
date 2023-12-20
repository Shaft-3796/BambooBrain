#include "PredictFromModel.h"

/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority (PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY)
 * @param config the configuration for the predict_from_model function
 * - predict_from_tree_config: the configuration for the predict_from_tree function
 * @param args mdoe specific arguments for the predict_from_model function
 * No arguments are expected.
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
float predict_from_random_forest_majority(const PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance) {
    int *votes = (int*) calloc(model->class_count, sizeof(int));

    for(int i=0; i<model->tree_count; ++i) {
        PredictFromTreeArgs predict_from_tree_args = {};
        const int prediction = config->predict_from_tree_config->predict_from_tree_function(config->predict_from_tree_config, &predict_from_tree_args, model->trees[i], instance);
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
 * @param args mdoe specific arguments for the predict_from_model function
 * No arguments are expected.
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
float predict_from_model_single_tree(const PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance) {
    const PredictFromTreeArgs predict_from_tree_args = {};
    return config->predict_from_tree_config->predict_from_tree_function(config->predict_from_tree_config, &predict_from_tree_args, model->tree, instance);
}

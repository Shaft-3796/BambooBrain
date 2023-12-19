#include "ModelTools.h"


/**
 * @brief count nodes in a model
 * @param model the model
 * @return the number of nodes
 */
int count_model_nodes(const Model* model){
    int node_count = 0;
    switch(model->mode) {
        case MODEL_MODE_RANDOM_FOREST:
            for(int i=0; i<model->tree_count; ++i) {
                node_count += count_decision_tree_nodes(model->trees[i]);
            }
            break;
    }
    return 0;
}

/**
 * @brief predict_all_from_model predicts the class of all instances in a dataset
 * @param predict_from_model_config the configuration for the predict_from_model function
 * @param model the model
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* predict_all_from_model(PredictFromModelConfig *predict_from_model_config, Model *model, Dataset *data) {
    int *predictions = (int*) calloc(data->instance_count, sizeof(int));

    for(int i=0; i<data->instance_count; ++i) {
        const PredictFromModelArgs predict_from_model_args = {};
        predictions[i] = predict_from_model_config->predict_from_model_function(predict_from_model_config, &predict_from_model_args, model, &data->instances[i]);
    }
    return predictions;
}

/**
 * @brief evaluate_model evaluates the accuracy of a model on a dataset
 * @param predict_from_model_config the configuration for the predict_from_model function
 * @param model the model
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_model(PredictFromModelConfig *predict_from_model_config, Model *model, Dataset *data) {
    int *predictions = predict_all_from_model(predict_from_model_config, model, data);
    int correct = 0;
    for(int i=0; i<data->instance_count; ++i) {
        if(predictions[i] == data->instances[i].class_id) correct++;
    }
    free(predictions);
    return (float)correct / (float)data->instance_count;
}

/**
 * @brief destroy_model destroys a model
 * @param model the model
 */
void destroy_model(Model *model) {
    switch (model->mode) {
        case MODEL_MODE_RANDOM_FOREST:
            for(int i=0; i<model->tree_count; ++i) {
                destroy_decision_tree(model->trees[i]);
            }
            free(model->trees);
            free(model);
            break;
    }
}

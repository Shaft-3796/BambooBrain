#include "ModelTools.h"


/**
 * @brief Load a model, create it if persistence is not enabled or if the file does not exist.
 * Save it to the file if persistence is enabled.
 * @param config the configuration
 * @param train_data_path the path to the training data
 * @param model_path the path to enable persistence, NULL to disable persistence,
 * if the file does not exist, the model will be created and saved to the file
 */
Model *load_model(const Config *config, const char *train_data_path, const char *model_path) {
    if(model_path) {
        FILE *storage = fopen(model_path, "rb");
        if(storage) {
            printf("Model found at %s\n", model_path);
            Model *model = read_model(storage);
            fclose(storage);
            return model;
        }
    }

    Dataset *data = parse_dataset_from_file(train_data_path);
    Model *model = config->create_model(config, data);

    if(model_path) {
        FILE *storage = fopen(model_path, "wb");
        write_model(storage, model);
        fclose(storage);
        printf("Model saved to %s\n", model_path);
    }

    destroy_dataset(data);
    return model;
}

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
        case MODEL_MODE_TREE:
            node_count += count_decision_tree_nodes(model->tree);
            break;
    }
    return node_count;
}

/**
 * @brief predict_all_from_model predicts the class of all instances in a dataset
 * @param config the configuration
 * @param model the model
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* predict_all_from_model(const Config *config, const Model *model, const Dataset *data) {
    int *predictions = (int*) calloc(data->instance_count, sizeof(int));

    for(int i=0; i<data->instance_count; ++i) {
        if(i%1000 == 0) {printf("Predicted %d/%d instances\n", i, data->instance_count); fflush(stdout);}
        predictions[i] = config->predict_from_model(config, model, &data->instances[i]);
    }
    return predictions;
}

/**
 * @brief evaluate_model evaluates the accuracy of a model on a dataset
 * @param config the configuration
 * @param model the model
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_model(const Config *config, const Model *model, const Dataset *data) {
    int *predictions = predict_all_from_model(config, model, data);
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
        case MODEL_MODE_TREE:
            destroy_decision_tree(model->tree);
            free(model);
            break;
    }
}

/**
 * @brief create_model creates a model
 * @param config the configuration for the create_model function
 * @param args mode specific arguments for the create_model function
 * @param data the dataset
 * @return a pointer to the model
 */

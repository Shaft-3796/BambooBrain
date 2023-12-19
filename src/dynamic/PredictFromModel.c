#include "PredictFromModel.h"

/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority
 * @param predict_from_tree_args the arguments for the predict_from_tree function
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
int predict_from_random_forest_majority(PredictFromTreeArgs *predict_from_tree_args, const Model *model, Instance *instance) {
    int *votes = (int*) calloc(model->class_count, sizeof(int));

    for(int i=0; i<model->tree_count; ++i) {
        predict_from_tree_args->tree = model->trees[i];
        predict_from_tree_args->instance = instance;
        votes[predict_from_tree(predict_from_tree_args)]++;
    }

    int class_id = 0;
    for(int i=0; i<model->class_count; ++i) {
        if(votes[i] > votes[class_id]) class_id = i;
    }

    free(votes);
    return class_id;
}

/**
 * @brief predict_from_model predicts the class of an instance based on a model
 * @param args the arguments for the predict_from_model function
 * @return the class id
 */
int predict_from_model(PredictFromModelArgs *args){
    switch(args->mode) {
        case PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY:
            return predict_from_random_forest_majority(args->predict_from_tree_args, args->model, args->instance);
        default:
            printf("Error: unknown predict model mode\n");
        return -1;
    }
}
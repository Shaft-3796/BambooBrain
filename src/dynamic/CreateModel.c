#include "CreateModel.h"

/**
 * @brief create_random_forest creates a random forest model
 * @param data the dataset
 * @param bagging_args the arguments for the bagg_dataset function
 * @param create_tree_args the arguments for the create_tree function
 * @param tree_count the number of trees in the forest
 * @return a pointer to the model
 */
Model *create_random_forest(Dataset *data, BaggingArgs *bagging_args, CreateTreeArgs *create_tree_args, int tree_count) {
    // Create the random forest
    Model *rf = (Model*) calloc(1, sizeof(Model));
    rf->tree_count = tree_count; rf->class_count = data->class_count;
    rf->trees = (DecisionTreeNode**)calloc(tree_count, sizeof(DecisionTreeNode*));

    // Fill the random forest with trees
    bagging_args->bags = tree_count;
    bagging_args->data = data;
    Subproblem **subproblems = bagg_dataset(bagging_args);


    for(int i=0; i<tree_count; ++i) {
        create_tree_args->sp = subproblems[i];
        rf->trees[i] = create_tree(create_tree_args);
        destroy_subproblem(subproblems[i]);
    }

    return rf;
}

/**
 * @brief create_model creates a model
 * @param args the arguments for the model_create function
 * @return a pointer to the model
 */
Model *create_model(CreateModelArgs *args){
    switch(args->mode) {
        case MODEL_MODE_RANDOM_FOREST:
            return (Model*)create_random_forest(args->data, args->bagging_args, args->create_tree_args, args->tree_count);
        default:
            printf("Error: unknown model create mode mode\n");
        return NULL;
    }
}
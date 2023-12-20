#include "CreateModel.h"

/**
 * @brief create_random_forest creates a random forest model (MODEL_MODE_RANDOM_FOREST)
 * @param config the configuration for the create_model function
 * - bagging_config: the configuration for the bagging function
 * - create_tree_config: the configuration for the create_tree function
 * - tree_count: the number of trees in the forest
 * @param args mode specific arguments for the create_model function
 * No arguments are expected.
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_random_forest(const CreateModelConfig *config, const CreateModelArgs *args, const Dataset *data) {
    // Create the random forest
    Model *rf = (Model*) calloc(1, sizeof(Model));

    rf->tree_count = config->tree_count; rf->class_count = data->class_count;
    rf->trees = (DecisionTreeNode**) calloc(config->tree_count, sizeof(DecisionTreeNode*));

    // Fill the random forest with trees
    const BaggingArgs bagging_args = {};
    Subproblem **subproblems = config->bagging_config->bagging_function(config->bagging_config, &bagging_args, data, config->tree_count);


    for(int i=0; i<config->tree_count; ++i) {
        const CreateTreeArgs create_tree_args = {.current_depth=0};
        printf("Creating tree %d/%d\n", i+1, config->tree_count);
        fflush(stdout);
        rf->trees[i] = config->create_tree_config->create_tree_function(config->create_tree_config, &create_tree_args, subproblems[i]);
        destroy_subproblem(subproblems[i]);
    }

    return rf;
}

/**
 * @brief create_tree creates a simple decision tree model (MODEL_MODE_TREE)
 * @param config the configuration for the create_model function
 * - create_tree_config: the configuration for the create_tree function
 * @param args mode specific arguments for the create_model function
 * No arguments are expected.
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_tree(const CreateModelConfig *config, const CreateModelArgs *args, const Dataset *data) {
    Model *rf = (Model*) calloc(1, sizeof(Model));

    rf->class_count = data->class_count;

    const CreateTreeArgs create_tree_args = {.current_depth=0};
    Subproblem *subproblem = create_subproblem_from_dataset(data);
    rf->tree = config->create_tree_config->create_tree_function(config->create_tree_config, &create_tree_args, subproblem);

    destroy_subproblem(subproblem);
    return rf;
}

#include "CreateModel.h"

/**
 * @brief create_random_forest creates a random forest model (MODEL_MODE_RANDOM_FOREST)
 * @param config the configuration for the create_model function
 * - bagging_config: the configuration for the bagging function
 * - create_tree_config: the configuration for the create_tree function
 * - tree_count: the number of trees in the forest
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_random_forest(const Config *config, const Dataset *data) {
    // Create the random forest
    Model *rf = (Model*) calloc(1, sizeof(Model));

    rf->tree_count = config->tree_count; rf->class_count = data->class_count;
    rf->trees = (DecisionTreeNode**) calloc(config->tree_count, sizeof(DecisionTreeNode*));

    // Fill the random forest with trees
    Subproblem **subproblems = config->apply_bagging(config, data, config->tree_count);


    for(int i=0; i<config->tree_count; ++i) {
        printf("Creating tree %d/%d\n", i+1, config->tree_count);
        fflush(stdout);
        rf->trees[i] = config->create_tree(config, subproblems[i]);
        destroy_subproblem(subproblems[i]);
    }

    return rf;
}

/**
 * @brief create_tree creates a simple decision tree model (MODEL_MODE_TREE)
 * @param config the configuration for the create_model function
 * - create_tree_config: the configuration for the create_tree function
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_tree(const Config *config, const Dataset *data) {
    Model *rf = (Model*) calloc(1, sizeof(Model));

    rf->class_count = data->class_count;

    Subproblem *subproblem = create_subproblem_from_dataset(data);
    rf->tree = config->create_tree(config, subproblem);

    destroy_subproblem(subproblem);
    return rf;
}

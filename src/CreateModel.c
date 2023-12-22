#include "CreateModel.h"

static int* gen_random_integers(const int from, const int to, const int count) {
    int *integers = (int*)calloc(count, sizeof(int));
    for(int i=0; i<count; ++i) integers[i] = rand() % (to-from) + from;
    return integers;
}

/**
 * @brief generate feature bagging
 * @param feature_count the number of features
 * @param feature_bagging_proportion the proportion of features to select
 * @return a list of booleans
 */
static bool *generate_feature_bagging(const int feature_count, const float feature_bagging_proportion) {
    bool *feature_bagging = (bool*) calloc(feature_count, sizeof(bool));
    const int selected_count = feature_bagging_proportion * feature_count;

    int *indexes = calloc(feature_count, sizeof(int));
    for(int i=0; i<feature_count; ++i) indexes[i] = i;

    for(int i=0; i<selected_count; ++i) {
        int *integers = gen_random_integers(0, feature_count-i, 1);
        const int index = indexes[integers[0]];
        feature_bagging[index] = true;
        // memcpy to remove the index
        memcpy(indexes+integers[0], indexes+integers[0]+1, (feature_count-i-integers[0]-1)*sizeof(int));
        free(integers);
    }

    free(indexes);
    return feature_bagging;
}

/**
 * @brief create_random_forest creates a random forest model (MODEL_MODE_RANDOM_FOREST)
 * @param config the configuration for the create_model function
 * - bagging_config: the configuration for the bagging function
 * - create_tree_config: the configuration for the create_tree function
 * - tree_count: the number of trees in the forest
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_random_forest(Config *config, const Dataset *data) {
    // Create the random forest
    Model *rf = (Model*) calloc(1, sizeof(Model));
    rf->mode = MODEL_MODE_RANDOM_FOREST;

    rf->tree_count = config->tree_count; rf->class_count = data->class_count;
    rf->trees = (DecisionTreeNode**) calloc(config->tree_count, sizeof(DecisionTreeNode*));

    // Apply bagging
    Subproblem **subproblems = config->apply_bagging(config, data, config->tree_count);

    Progress progress;
    init_progress(&progress, config->tree_count, 0, "Random Forest Creation");
    update_progress(&progress, 0);

    for(int i=0; i<config->tree_count; ++i) {
        if(config->feature_bagging_proportion > 0) {
            config->feature_bagging = generate_feature_bagging(data->feature_count, config->feature_bagging_proportion);
            config->feature_bagging_count = data->feature_count;
        }

        rf->trees[i] = config->create_tree(config, subproblems[i]);

        free(config->feature_bagging);
        config->feature_bagging = NULL;
        destroy_subproblem(subproblems[i]);
        update_progress(&progress, i+1);
    }

    finalize_progress(&progress);

    free(subproblems);

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
    rf->mode = MODEL_MODE_TREE;

    rf->class_count = data->class_count;

    Progress progress;
    init_progress(&progress, config->tree_count, 0, "Random Forest Creation");
    update_progress(&progress, 0);

    Subproblem *subproblem = create_subproblem_from_dataset(data);
    rf->tree = config->create_tree(config, subproblem);

    finalize_progress(&progress);

    destroy_subproblem(subproblem);
    return rf;
}

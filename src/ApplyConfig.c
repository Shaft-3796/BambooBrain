#include "ApplyConfig.h"


/**
 * @brief apply_config applies a configuration
 * @param config the configuration
 */
void apply_config(Config *config) {
    // Model mode
    switch (config->model_mode) {
        case MODEL_MODE_RANDOM_FOREST:
            config->create_model = create_random_forest;
            config->predict_from_model = predict_from_random_forest_majority;
            break;
        case MODEL_MODE_TREE:
            config->create_model = create_tree;
            config->predict_from_model = predict_from_model_single_tree;
            break;
        default:
            printf("Error: unknown model mode\n");
            exit(1);
    }

    // Create tree mode
    switch (config->create_tree_mode) {
        case CREATE_TREE_MODE_PRUNNING_THRESHOLD:
            config->create_tree = create_decision_tree_from_prunning_threshold;
            break;
        default:
            printf("Error: unknown create tree mode\n");
            exit(1);
    }

    // Compute split mode
    switch (config->compute_split_mode) {
        case COMPUTE_SPLIT_MODE_PUREST_FEATURE:
            config->compute_split = compute_purest_feature_split;
            break;
        default:
            printf("Error: unknown compute split mode\n");
            exit(1);
    }

    // Threshold mode
    switch (config->threshold_mode) {
        case THRESHOLD_MODE_MID_MIN_MAX:
            config->get_thresholds = get_subproblem_threshold_min_max;
            break;
        case THRESHOLD_MODE_ALL_VALUES:
            config->get_thresholds = get_subproblem_threshold_all_values;
            break;
        default:
            printf("Error: unknown threshold mode\n");
            exit(1);
    }

    // Impurity mode
    switch (config->impurity_mode) {
        case IMPURITY_MODE_GINI:
            config->get_impurity = gini_impurity;
            break;
        default:
            printf("Error: unknown impurity mode\n");
            exit(1);
    }

    // Bagging mode
    switch (config->bagging_mode) {
        case BAGGING_MODE_PROPORTIONAL:
            config->apply_bagging = bagging_from_proportion;
            break;
        default:
            printf("Error: unknown bagging mode\n");
            exit(1);
    }

    // Predict from tree mode
    switch (config->predict_from_tree_mode) {
        case PREDICT_FROM_TREE_MODE_THRESHOLD:
            config->predict_from_tree = predict_from_tree_and_threshold;
            break;
        case PREDICT_FROM_TREE_MODE_SIGMOID_SCORE:
            config->predict_from_tree = predict_from_tree_and_sigmoid_score;
            break;
        default:
            printf("Error: unknown predict from tree mode\n");
            exit(1);
    }

}

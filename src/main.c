#include <time.h>

#include "Dataset.h"
#include "Config.h"
#include "ApplyConfig.h"
#include "Hyperopt.h"
#include "ModelTools.h"
#include "PreProcessing.h"
#include "UserInterface.h"
#include "Progress.h"


/**
 * @brief Validate moodle test, make sure to set VERBOSE to 0 in Settings.h
 */
void validate_moodle(int argc, char** argv) {
    /* ############################################################# */
    /* ####################### CONFIGURATION ####################### */
    /* ############################################################# */
    // The configuration of the algorithm
    Config config = {
        // MODEL_MODE_TREE: A single decision tree
        // MODEL_MODE_RANDOM_FOREST: A random forest
        .model_mode = MODEL_MODE_RANDOM_FOREST,

        // PREDICT_FROM_TREE_MODE_THRESHOLD: Predict the class based on the split of each node,
        // only one main prediction is made
        // PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: Predict the class based on the sigmoid score of each path,
        // a probabilistic prediction is made for each class
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_THRESHOLD,

        // THRESHOLD_MODE_MID_MIN_MAX: Compute the threshold to use for a given feature based on
        // (minFeatureValue + maxFeatureValue) / 2
        // THRESHOLD_MODE_ALL_VALUES: Compute all the thresholds to use for a given feature based on all values
        // between min and max with a step, used to reduce nodes count
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,

        // The following is not configurable for now
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        // If we have to create a random forest, the number of trees to create
        .tree_count = 110,

        // If we have to create a tree, the maximum depth of the tree and the prunning threshold
        .max_tree_depth = 150,
        .prunning_threshold = 1.0,

        // If we are predicting using the sigmoid score, the lambda value for the sigmoid function
        .sigmoid_lambda = 0.15,

        // If we are in THRESHOLD_MODE_ALL_VALUES, the step to use to compute the thresholds
        .threshold_step = 1,

        // If we are creating a random forest, the proportion of instances to use for each tree
        .bagging_proportion = 1.0,

        // If we are creating a random forest, the proportion of features to use for each tree
        .feature_bagging_proportion = 0.8,

        // For UI, don't touch
        .pencil_radius = 1.2,
        .tickrate = 100,
    };
    apply_config(&config);

    // Uncomment to add a pre processing step. The first parameter is the step to add, the second is the merge mode
    // Steps:
    // PP_STEP_BLACK_AND_WHITE: Convert the image to black and white
    // Merge modes:
    // PP_MERGE_MODE_REPLACE: replace the original features with the new ones
    // PP_MERGE_MODE_ADD_NOW: add the new features to the original ones directly
    // PP_MERGE_MODE_ADD_LATER: add the new features to the original ones when the preprocessing is done
    // add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_REPLACE);
    // #############################################################

    char *train = argv[1];
    char *test = argv[2];


    Dataset *ds = parse_dataset_from_file(train);
    Dataset *test_ds = parse_dataset_from_file(test);
    Model *model = load_model(&config, train, NULL);
    Predictions **predictions = predict_all_from_model(&config, model, test_ds);
    for(int i=0; i<test_ds->instance_count; ++i) {
        printf("%d\n", predictions[i]->main_prediction);
        destroy_predictions(predictions[i]);
    }

    destroy_dataset(ds);
    destroy_dataset(test_ds);
    destroy_model(model);
    free(predictions);
    exit(1);
}

/**
 * @brief Run the hyper optimizer
 */
void hyper_opt() {
    /* ############################################################# */
    /* ####################### CONFIGURATION ####################### */
    /* ############################################################# */
    // The path to the dataset to use to generate models
    char train_path[128] = "datasets/MNIST_train.txt";
    // The path to the dataset to use to evaluate models
    char test_path[128] = "datasets/MNIST_test.txt";
    // The path to the file to save the hyperopt results
    char opt_path[128] = "../datasets/hyperopt.bb";

    // The configuration of the algorithm
    Config config = {
        // MODEL_MODE_TREE: A single decision tree
        // MODEL_MODE_RANDOM_FOREST: A random forest
        .model_mode = MODEL_MODE_RANDOM_FOREST,

        // PREDICT_FROM_TREE_MODE_THRESHOLD: Predict the class based on the split of each node,
        // only one main prediction is made
        // PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: Predict the class based on the sigmoid score of each path,
        // a probabilistic prediction is made for each class
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,

        // THRESHOLD_MODE_MID_MIN_MAX: Compute the threshold to use for a given feature based on
        // (minFeatureValue + maxFeatureValue) / 2
        // THRESHOLD_MODE_ALL_VALUES: Compute all the thresholds to use for a given feature based on all values
        // between min and max with a step, used to reduce nodes count
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,

        // The following is not configurable for now
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        // If we have to create a random forest, the number of trees to create
        .tree_count = 110,

        // If we have to create a tree, the maximum depth of the tree and the prunning threshold
        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        // If we are predicting using the sigmoid score, the lambda value for the sigmoid function
        .sigmoid_lambda = 0.15,

        // If we are in THRESHOLD_MODE_ALL_VALUES, the step to use to compute the thresholds
        .threshold_step = 1,

        // If we are creating a random forest, the proportion of instances to use for each tree
        .bagging_proportion = 0.8,

        // If we are creating a random forest, the proportion of features to use for each tree
        .feature_bagging_proportion = 0.8,

        // For UI, don't touch
        .pencil_radius = 1.2,
        .tickrate = 100,
    };
    apply_config(&config);

    Hyperopt *hyperopt = create_hyperopt(&config);
    // Configure hyperopt parameters.
    // The second argument is the field in the struct Config to modify, the third is the type of the variable,
    // the fourth is the start value, the fifth is the step value, the sixth is the number of values to test
    // WARNING, call the function below only 2 times, no more no less.
    add_hypervar(hyperopt, &config.tree_count, HYPER_VARIABLE_TYPE_INT, 5, 15, 2);
    add_hypervar(hyperopt, (int*)&config.bagging_proportion, HYPER_VARIABLE_TYPE_FLOAT, 0.1, 0.1, 10);
    // #############################################################



    bbprintf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(train_path);
    Dataset *testData = parse_dataset_from_file(test_path);

    // Pre Processing
    apply_pp_steps_to_dataset(&config, trainData);
    apply_pp_steps_to_dataset(&config, testData);

    bbprintf("Running hyperopt...\n");
    run_hyperopt(opt_path, hyperopt, trainData, testData);

    destroy_dataset(trainData);
    destroy_dataset(testData);

    exit(1);
}

/**
 * @brief Launch the user interface to predict handwritten digits
 */
void user_interface() {
    /* ############################################################# */
    /* ####################### CONFIGURATION ####################### */
    /* ############################################################# */
    // The path to the model to load, we have provided a model using the MNIST dataset
    char model_path[128] = "../datasets/mnist_forest.bb";

    // The configuration of the algorithm
    Config config = {
        // MODEL_MODE_TREE: A single decision tree
        // MODEL_MODE_RANDOM_FOREST: A random forest
        .model_mode = MODEL_MODE_RANDOM_FOREST,

        // PREDICT_FROM_TREE_MODE_THRESHOLD: Predict the class based on the split of each node,
        // only one main prediction is made
        // PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: Predict the class based on the sigmoid score of each path,
        // a probabilistic prediction is made for each class
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,

        // THRESHOLD_MODE_MID_MIN_MAX: Compute the threshold to use for a given feature based on
        // (minFeatureValue + maxFeatureValue) / 2
        // THRESHOLD_MODE_ALL_VALUES: Compute all the thresholds to use for a given feature based on all values
        // between min and max with a step, used to reduce nodes count
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,

        // The following is not configurable for now
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        // If we have to create a random forest, the number of trees to create
        .tree_count = 110,

        // If we have to create a tree, the maximum depth of the tree and the prunning threshold
        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        // If we are predicting using the sigmoid score, the lambda value for the sigmoid function
        .sigmoid_lambda = 0.15,

        // If we are in THRESHOLD_MODE_ALL_VALUES, the step to use to compute the thresholds
        .threshold_step = 1,

        // If we are creating a random forest, the proportion of instances to use for each tree
        .bagging_proportion = 0.8,

        // If we are creating a random forest, the proportion of features to use for each tree
        .feature_bagging_proportion = 0.8,

        // For UI, don't touch
        .pencil_radius = 1.2,
        .tickrate = 100,
    };
    apply_config(&config);
    // #############################################################

    bbprintf("Loading model...\n");
    Model *model = load_model(&config, model_path, model_path);
    create_ui(&config, model);
    destroy_model(model);
    exit(1);
}

/**
 * @brief Create and evaluate model
 */
void evaluation() {
    /* ############################################################# */
    /* ####################### CONFIGURATION ####################### */
    /* ############################################################# */
    // Path to training dataset if we have to create a model
    char train_path[128] = "datasets/MNIST_train.txt";
    // Path to testing dataset to evaluate the model
    char test_path[128] = "datasets/MNIST_test.txt";

    // Uncomment only one of the following lines
    // First line to enable model persistence. If the file exists, the model will be loaded from it, otherwise
    // it will be created and saved to it. Second line to disable model persistence, model will always be created
    char model_path[128] = "../datasets/mnist_forest.bb";
    // char *model_path = NULL;
    // --------------------------------------------------

    // The configuration of the algorithm
    Config config = {
        // MODEL_MODE_TREE: A single decision tree
        // MODEL_MODE_RANDOM_FOREST: A random forest
        .model_mode = MODEL_MODE_RANDOM_FOREST,

        // PREDICT_FROM_TREE_MODE_THRESHOLD: Predict the class based on the split of each node,
        // only one main prediction is made
        // PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: Predict the class based on the sigmoid score of each path,
        // a probabilistic prediction is made for each class
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,

        // THRESHOLD_MODE_MID_MIN_MAX: Compute the threshold to use for a given feature based on
        // (minFeatureValue + maxFeatureValue) / 2
        // THRESHOLD_MODE_ALL_VALUES: Compute all the thresholds to use for a given feature based on all values
        // between min and max with a step, used to reduce nodes count
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,

        // The following is not configurable for now
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        // If we have to create a random forest, the number of trees to create
        .tree_count = 110,

        // If we have to create a tree, the maximum depth of the tree and the prunning threshold
        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        // If we are predicting using the sigmoid score, the lambda value for the sigmoid function
        .sigmoid_lambda = 0.15,

        // If we are in THRESHOLD_MODE_ALL_VALUES, the step to use to compute the thresholds
        .threshold_step = 1,

        // If we are creating a random forest, the proportion of instances to use for each tree
        .bagging_proportion = 0.8,

        // If we are creating a random forest, the proportion of features to use for each tree
        .feature_bagging_proportion = 0.8,

        // For UI, don't touch
        .pencil_radius = 1.2,
        .tickrate = 100,
    };
    apply_config(&config);

    // Uncomment to add a pre processing step. The first parameter is the step to add, the second is the merge mode
    // Steps:
    // PP_STEP_BLACK_AND_WHITE: Convert the image to black and white
    // Merge modes:
    // PP_MERGE_MODE_REPLACE: replace the original features with the new ones
    // PP_MERGE_MODE_ADD_NOW: add the new features to the original ones directly
    // PP_MERGE_MODE_ADD_LATER: add the new features to the original ones when the preprocessing is done
    // add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_REPLACE);
    // #############################################################

    Model *model = load_model(&config, train_path, model_path);

    Dataset *testData = parse_dataset_from_file(test_path);
    apply_pp_steps_to_dataset(&config, testData);

    float accuracy = evaluate_model(&config, model, testData);
    bbprintf("Accuracy: %.2f%%\n", accuracy*100);
    bbprintf("Model node count: %d\n", count_model_nodes(model));

    destroy_model(model);
    destroy_dataset(testData);
    exit(1);
}


int main(int argc, char** argv){
    srand(time(NULL));
    print_logo();

    // Uncomment only one of the following lines
    user_interface();  // Run the user interface
    // evaluation();  // Run the creating/evaluation of a model
    // hyper_opt();  // Run the hyper optimizer

    // validate_moodle(argc, argv);  // Validate moodle test, make sure to set VERBOSE to 0 in Settings.h
}

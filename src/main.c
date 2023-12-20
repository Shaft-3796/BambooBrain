#include <time.h>

#include "Dataset.h"
#include "DecisionTreeTools.h"

#include "Split.h"
#include "ModelTools.h"
#include "UserInterface.h"
#include "dynamic/CreateModel.h"


int main(int argc, char** argv){
    /* Algorithm configuration */
    // Creation

    CreateModelConfig create_model_config = {
        .mode = MODEL_MODE_RANDOM_FOREST,
        .create_model_function = create_random_forest,
        .tree_count = 2,
    };

    BaggingConfig bagging_config = {
        .mode = BAGGING_MODE_PROPORTIONAL,
        .bagging_function = bagging_from_proportion,
        .proportion = 0.5,
    };
    create_model_config.bagging_config = &bagging_config;

    /*
    CreateModelConfig create_model_config = {
        .mode = MODEL_MODE_TREE,
        .create_model_function = create_tree,
    };*/


    CreateTreeConfig create_tree_config = {
        .mode = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .create_tree_function = create_decision_tree_from_prunning_treshold,
        .max_depth = 30,
        .prunning_threshold = 1,
    };
    create_model_config.create_tree_config = &create_tree_config;

    ComputeSplitConfig compute_split_config = {
        .mode = COMPUTE_SPLIT_MODE_PUREST_THRESHOLD,
        .compute_split_function = compute_purest_threshold_split,
    };
    create_tree_config.compute_split_config = &compute_split_config;

    ThresholdConfig threshold_config = {
        .mode = THRESHOLD_MODE_MID_MIN_MAX,
        .threshold_function = get_subproblem_threshold_min_max,
    };
    compute_split_config.threshold_config = &threshold_config;

    ImpurityConfig impurity_config = {
        .mode = IMPURITY_MODE_GINI,
        .impurity_function = gini_impurity,
    };
    compute_split_config.impurity_config = &impurity_config;

    // Prediction
    PredictFromModelConfig predict_from_model_config = {
        .mode = PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY,
        .predict_from_model_function = predict_from_random_forest_majority,
    };

    PredictFromTreeConfig predict_from_tree_config = {
        .mode = PREDICT_FROM_TREE_MODE_THRESHOLD,
        .predict_from_tree_function = predict_from_tree_and_threshold,
    };
    predict_from_model_config.predict_from_tree_config = &predict_from_tree_config;


    srand(time(NULL));

    char path[128] = "datasets/MNIST_train.txt";
    char test_path[128] = "datasets/MNIST_test.txt";
    char model_path[128] = "../datasets/model.bb";

    printf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);

    printf("Loading model...\n");
    Model *model = load_model(&create_model_config, path, model_path);

    float train_accuracy = evaluate_model(&predict_from_model_config, model, trainData);
    float test_accuracy = evaluate_model(&predict_from_model_config, model, testData);

    create_ui(model, &predict_from_model_config);


    printf("Train accuracy: %.2f%%\n", train_accuracy*100);
    printf("Test accuracy: %.2f%%\n", test_accuracy*100);


    destroy_model(model);
    destroy_dataset(trainData);
    destroy_dataset(testData);
}
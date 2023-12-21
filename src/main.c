#include <time.h>

#include "Dataset.h"
#include "Config.h"
#include "ApplyConfig.h"
#include "ModelTools.h"
#include "DecisionTreeTools.h"


int main(int argc, char** argv){
    /* Algorithm configuration */
    Config config = {
        .model_mode = MODEL_MODE_TREE,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_THRESHOLD,
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        .tree_count = 5,

        .max_tree_depth = 30,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.2,

        .threshold_step = 1,

        .bagging_proportion = 0.5,

        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    apply_config(&config);


    srand(time(NULL));

    char path[128] = "datasets/PENDIGITS_train.txt";
    char test_path[128] = "datasets/PENDIGITS_test.txt";
    char model_path[128] = "../datasets/a.bb";

    printf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);


    printf("Loading model...\n");
    Model *model = load_model(&config, path, model_path);


    float train_accuracy = evaluate_model(&config, model, trainData);
    float test_accuracy = evaluate_model(&config, model, testData);

    printf("Train accuracy: %.2f%%\n", train_accuracy*100);
    printf("Test accuracy: %.2f%%\n", test_accuracy*100);
    printf("Nodes: %d\n", count_model_nodes(model));

    // create_ui(model, &predict_from_model_config);


    destroy_model(model);
    destroy_dataset(trainData);
    destroy_dataset(testData);
}
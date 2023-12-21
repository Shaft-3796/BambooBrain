#include <time.h>

#include "Dataset.h"
#include "Config.h"
#include "ApplyConfig.h"
#include "ModelTools.h"
#include "UserInterface.h"


int main(int argc, char** argv){

    Predictions *predictions = (Predictions *) calloc(1, sizeof(Predictions));

    /* Algorithm configuration */
    Config config = {
        .model_mode = MODEL_MODE_RANDOM_FOREST,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_THRESHOLD,
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        .tree_count = 50,

        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.2,

        .threshold_step = 1,

        .bagging_proportion = 0.75,

        .predictions=predictions,

        .pencil_radius = 1.2,
        .tickrate = 100,

        predictions, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    apply_config(&config);


    srand(time(NULL));

    char path[128] = "datasets/MNIST_train.txt";
    char test_path[128] = "datasets/MNIST_test.txt";
    char model_path[128] = "../datasets/mnist_forest.bb";

    printf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);


    printf("Loading model...\n");
    Model *model = load_model(&config, path, model_path);

    create_ui(&config, model);

    float train_accuracy = evaluate_model(&config, model, trainData);
    float test_accuracy = evaluate_model(&config, model, testData);

    printf("Train accuracy: %.2f%%\n", train_accuracy*100);
    printf("Test accuracy: %.2f%%\n", test_accuracy*100);
    printf("Nodes: %d\n", count_model_nodes(model));


    destroy_model(model);
    destroy_dataset(trainData);
    destroy_dataset(testData);
}

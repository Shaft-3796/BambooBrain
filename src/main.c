#include <time.h>

#include "Dataset.h"
#include "Config.h"
#include "ApplyConfig.h"
#include "ModelTools.h"
#include "PreProcessing.h"
#include "UserInterface.h"

void test_pre_processing() {
    Config config = {
        // Add eventual pre processing config here
    };
    // take the initial features and add at the end of the process these features converted to black and white
    add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_ADD_LATER);
    // Will be ran only on initial features because we choosed ADD_LATER mode so the result of the above step is
    // computed but not added to the features, initial converted features will be added to the features just after this step
    add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_ADD_NOW);
    // We are now working with initial features and converted initial features since whe choosed ADD_NOW mode
    // We will replace all of these features with the result of this step, so the result of tthe above step will be
    // included in this step but nothing will happen since these features are already converted however the initial features will be converted and replaced
    add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_REPLACE);
    // Result of initial step will be added to the features now

    Instance a = {
        .values = (int *) calloc(4, sizeof(int)),
        .class_id = 0,
    };
    // Test features here
    a.values[0] = 0; a.values[1] = 127; a.values[2] = 788; a.values[3] = 987;

    Dataset data = {
        .instances = calloc(1, sizeof(Instance)),
        .instance_count = 1,
        .feature_count = 4,
        .class_count = 1,
    };
    data.instances[0] = a;

    // print dataset
    printf("No steps:\n");
    for (int i = 0; i < data.instance_count; ++i) {
        printf("Instance %d: ", i);
        for (int j = 0; j < data.feature_count; ++j) {
            printf("%d ", data.instances[i].values[j]);
        }
    }
    printf("\n");

    apply_pp_steps_to_dataset(&config, &data);

    // print dataset
    printf("With steps:\n");
    for (int i = 0; i < data.instance_count; ++i) {
        printf("Instance %d: ", i);
        for (int j = 0; j < data.feature_count; ++j) {
            printf("%d ", data.instances[i].values[j]);
        }
    }
    printf("\n");

    exit(1);
}


int main(int argc, char** argv){
    //test_pre_processing();

    /* Algorithm configuration */
    Config config = {
        .model_mode = MODEL_MODE_RANDOM_FOREST,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        .tree_count = 5,

        .max_tree_depth = 10,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.2,

        .threshold_step = 1,

        .bagging_proportion = 0.5,

        .pencil_radius = 1.2,
        .tickrate = 100,

        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL,
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

    const float train_accuracy = evaluate_model(&config, model, trainData);
    const float test_accuracy = evaluate_model(&config, model, testData);

    printf("Train accuracy: %.2f%%\n", train_accuracy*100);
    printf("Test accuracy: %.2f%%\n", test_accuracy*100);
    printf("Nodes: %d\n", count_model_nodes(model));

    destroy_dataset(trainData);
    destroy_dataset(testData);
    destroy_model(model);
}

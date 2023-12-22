#include <time.h>

#include "Dataset.h"
#include "Config.h"
#include "ApplyConfig.h"
#include "Hyperopt.h"
#include "ModelTools.h"
#include "PreProcessing.h"
#include "UserInterface.h"
#include "Progress.h"

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
    bbprintf("No steps:\n");
    for (int i = 0; i < data.instance_count; ++i) {
        bbprintf("Instance %d: ", i);
        for (int j = 0; j < data.feature_count; ++j) {
            bbprintf("%d ", data.instances[i].values[j]);
        }
    }
    bbprintf("\n");

    apply_pp_steps_to_dataset(&config, &data);

    // print dataset
    bbprintf("With steps:\n");
    for (int i = 0; i < data.instance_count; ++i) {
        bbprintf("Instance %d: ", i);
        for (int j = 0; j < data.feature_count; ++j) {
            bbprintf("%d ", data.instances[i].values[j]);
        }
    }
    bbprintf("\n");

    exit(1);
}


void hyper_opt() {
    Config config = {
        .model_mode = MODEL_MODE_RANDOM_FOREST,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_THRESHOLD,
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        .tree_count = 5,

        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.15,

        .threshold_step = 1,

        .bagging_proportion = 0.5,

        .feature_bagging_proportion = 1.0,

        .pencil_radius = 1.2,
        .tickrate = 100,
    };
    apply_config(&config);
    add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_REPLACE);
    add_pp_step(&config, PP_STEP_CENTER, PP_MERGE_MODE_REPLACE);

    srand(time(NULL));

    char path[128] = "datasets/MNIST_train.txt";
    char test_path[128] = "datasets/MNIST_test.txt";
    char opt_path[128] = "../datasets/hyperopt.bb";

    bbprintf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);

    // Pre Processing
    apply_pp_steps_to_dataset(&config, trainData);
    apply_pp_steps_to_dataset(&config, testData);

    bbprintf("Loading hyperopt...\n");
    Hyperopt *hyperopt = create_hyperopt(&config);
    add_hypervar(hyperopt, &config.tree_count, HYPER_VARIABLE_TYPE_INT, 5, 15, 2);
    add_hypervar(hyperopt, (int*)&config.bagging_proportion, HYPER_VARIABLE_TYPE_FLOAT, 0.1, 0.1, 10);

    run_hyperopt(opt_path, hyperopt, trainData, testData);

    destroy_dataset(trainData);
    destroy_dataset(testData);

    exit(1);
}


void validate_moodle(int argc, char** argv) {
    char *train = argv[1];
    char *test = argv[2];

    /* Algorithm configuration */
    Config config = {
        .model_mode = MODEL_MODE_RANDOM_FOREST,
        .create_tree = CREATE_TREE_MODE_PRUNNING_THRESHOLD,
        .predict_from_tree_mode = PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,
        .compute_split_mode = COMPUTE_SPLIT_MODE_PUREST_FEATURE,
        .threshold_mode = THRESHOLD_MODE_MID_MIN_MAX,
        .impurity_mode = IMPURITY_MODE_GINI,
        .bagging_mode = BAGGING_MODE_PROPORTIONAL,

        .tree_count = 50,

        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.15,

        .threshold_step = 1,

        .bagging_proportion = 0.75,

        .feature_bagging_proportion = 0.75,

        .pencil_radius = 1.2,
        .tickrate = 100,
    };

    Dataset *ds = parse_dataset_from_file(train);
    Dataset *test_ds = parse_dataset_from_file(test);

    Model *model = load_model(&config, train, NULL);

    Predictions **predictions = predict_all_from_model(&config, model, test_ds);
    for(int i=0; i<test_ds->instance_count; ++i) {
        bbprintf("%d\n", predictions[i]->main_prediction);
        destroy_predictions(predictions[i]);
    }

    destroy_dataset(ds);
    destroy_dataset(test_ds);
    destroy_model(model);
    free(predictions);
}


int main(int argc, char** argv){
    print_logo();

    //test_pre_processing();

    //hyper_opt();

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

        .max_tree_depth = 200,
        .prunning_threshold = 1.0,

        .sigmoid_lambda = 0.2,

        .threshold_step = 1,

        .bagging_proportion = 0.5,

        .feature_bagging_proportion = 1.0,

        .pencil_radius = 1.2,
        .tickrate = 100,
    };

    apply_config(&config);
    //add_pp_step(&config, PP_STEP_BLACK_AND_WHITE, PP_MERGE_MODE_REPLACE);

    srand(time(NULL));

    char path[128] = "datasets/MNIST_train.txt";
    char test_path[128] = "datasets/MNIST_test.txt";
    char model_path[128] = "../datasets/mnist_forest.bb";

    bbprintf("Loading datasets...\n");
    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);

    // Pre Processing
    apply_pp_steps_to_dataset(&config, trainData);
    apply_pp_steps_to_dataset(&config, testData);

    bbprintf("Loading model...\n");
    Model *model = load_model(&config, path, model_path);

    //create_ui(&config, model);

    //const float train_accuracy = evaluate_model(&config, model, trainData);
    const float test_accuracy = evaluate_model(&config, model, testData);

    //bbprintf("Train accuracy: %.2f%%\n", train_accuracy*100);
    bbprintf("Test accuracy: %.2f%%\n", test_accuracy*100);
    bbprintf("Nodes: %d\n", count_model_nodes(model));

    destroy_dataset(trainData);
    destroy_dataset(testData);
    destroy_model(model);
}

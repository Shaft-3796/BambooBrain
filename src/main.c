#include <time.h>

#include "Dataset.h"
#include "DecisionTreeTools.h"

#include "Split.h"
#include "ModelTools.h"
#include "dynamic/CreateModel.h"


int main(int argc, char** argv){
    srand(time(NULL));

    char path[128] = "datasets/PENDIGITS_train.txt";
    char test_path[128] = "datasets/PENDIGITS_test.txt";

    Dataset *trainData = parse_dataset_from_file(path);
    Dataset *testData = parse_dataset_from_file(test_path);

    Subproblem *sp = create_subproblem_from_dataset(trainData);
    printf("------ Train data ------\n");
    print_subproblem(sp);

    Subproblem *sp_test = create_subproblem_from_dataset(testData);
    printf("------ Test data ------\n");
    print_subproblem(sp_test);

    printf("[---- Single Decision Tree ----]\n");
    ThresholdArgs ta = {THRESHOLD_MODE_MID_MIN_MAX};
    ImpurityArgs ia = {IMPURITY_MODE_GINI};
    ComputeSplitArgs csa = {COMPUTE_SPLIT_MODE_PUREST_THRESHOLD, .threshold_args=&ta, .impurity_args=&ia};
    CreateTreeArgs cta = {CREATE_TREE_MODE_PRUNNING_THRESHOLD, .sp=sp, .compute_split_args=&csa, .max_depth=15, .prunning_threshold=0.9};
    DecisionTreeNode *tree = create_tree(&cta);
    printf("Génération d'un arbre de %d nœuds\n", count_decision_tree_nodes(tree));

    PredictFromTreeArgs pfta = {PREDICT_FROM_TREE_MODE_THRESHOLD};
    printf("Train: %f, Test: %f\n", evaluate_decision_tree(&pfta, tree, trainData), evaluate_decision_tree(&pfta, tree, testData));

    destroy_decision_tree(tree);

    printf("[---- Random Forest ----]\n");
    BaggingArgs ba = {BAGGING_MODE_PROPORTION, .proportion=0.5};
    CreateModelArgs mca = {MODEL_MODE_RANDOM_FOREST, .data=trainData, .bagging_args=&ba, .create_tree_args=&cta, .tree_count=10};
    Model *model = create_model(&mca);
    printf("Génération d'une forêt de %d arbres de %d nœuds\n", model->tree_count, count_model_nodes(model));

    PredictFromModelArgs pfma = {PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY, .predict_from_tree_args=&pfta};
    printf("Train: %f, Test: %f\n", evaluate_model(&pfma, model, trainData), evaluate_model(&pfma, model, testData));

    destroy_subproblem(sp);
    destroy_subproblem(sp_test);
    destroy_model(model);
    destroy_dataset(trainData);
    destroy_dataset(testData);
}
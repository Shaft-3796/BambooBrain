#include <time.h>

#include "Test.h"
#include "Dataset.h"
#include "DecisionTree.h"
#include "Split.h"
#include "RandomForest.h"

int main(int argc, char** argv){
    srand(time(NULL));

    char path[128] = "PENDIGITS_train.txt";
    char test_path[128] = "PENDIGITS_test.txt";

    Dataset *trainData = Dataset_readFromFile(path);
    Dataset *testData = Dataset_readFromFile(test_path);

    Subproblem *sp = Dataset_getSubproblem(trainData);
    printf("------ Train data ------\n");
    Subproblem_print(sp);

    Subproblem *sp_test = Dataset_getSubproblem(testData);
    printf("------ Test data ------\n");
    Subproblem_print(sp_test);

    printf("[---- Single Decision Tree ----]\n");
    DecisionTreeNode *tree = DecisionTree_create(sp, 0, 30, 1);
    printf("Génération d'un arbre de %d nœuds\n", Decision_nodeCount(tree));

    printf("Train: %f, Test: %f\n", DecisionTree_evaluate(tree, trainData), DecisionTree_evaluate(tree, testData));

    DecisionTree_destroy(tree);

    printf("[---- Random Forest ----]\n");
    RandomForest *rf = RandomForest_create(10, trainData, 30, 0.5, 1);
    printf("Génération d'une forêt de %d arbres de %d nœuds\n", rf->treeCount, RandomForest_nodeCount(rf));

    printf("Train: %f, Test: %f\n", RandomForest_evaluate(rf, trainData), RandomForest_evaluate(rf, testData));

    Subproblem_destroy(sp);
    Subproblem_destroy(sp_test);
    RandomForest_destroy(rf);
    Dataset_destroy(trainData);
    Dataset_destroy(testData);
}
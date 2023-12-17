#include "Test.h"
#include "Dataset.h"
#include "DecisionTree.h"
#include "Split.h"


/*
char path[128] = "LETTER_train.txt";
char test_path[128] = "LETTER_test.txt";

Dataset *trainData = Dataset_readFromFile(path);
Subproblem *sp = Dataset_getSubproblem(trainData);
Subproblem_print(sp);

DecisionTreeNode *tree = DecisionTree_create(sp, 0, 30, 1);
printf("Génération d'un arbre de %d nœuds\n", Decision_nodeCount(tree));

Dataset *testData = Dataset_readFromFile(test_path);
const int instance = 2228;
const int classId = DecisionTree_predict(tree, &testData->instances[instance]);
printf("Test instance %d is of class %d\n", instance, classId);

printf("Train: %f, Test: %f\n", DecisionTree_evaluate(tree, trainData), DecisionTree_evaluate(tree, testData));

DecisionTree_destroy(tree);
Dataset_destroy(trainData);

return 0;
*/

int main(int argc, char** argv){
    char path[128] = "MNIST_train.txt";
    char test_path[128] = "MNIST_test.txt";

    Dataset *trainData = Dataset_readFromFile(path);
    Dataset *testData = Dataset_readFromFile(test_path);

    Subproblem *sp = Dataset_getSubproblem(trainData);
    printf("------ Train data ------\n");
    Subproblem_print(sp);

    Subproblem *sp_test = Dataset_getSubproblem(testData);
    printf("------ Test data ------\n");
    Subproblem_print(sp_test);

    DecisionTreeNode *tree = DecisionTree_create(sp, 0, 30, 1);
    printf("Génération d'un arbre de %d nœuds\n", Decision_nodeCount(tree));

    printf("Train: %f, Test: %f\n", DecisionTree_evaluate(tree, trainData), DecisionTree_evaluate(tree, testData));

    DecisionTree_destroy(tree);
    Subproblem_destroy(sp);
    Subproblem_destroy(sp_test);
    Dataset_destroy(trainData);
    Dataset_destroy(testData);
}
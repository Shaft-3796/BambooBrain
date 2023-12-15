#include "Dataset.h"
#include "DecisionTree.h"
#include "Split.h"

int main(int argc, char** argv){
    char path[128] = "PENDIGITS_train.txt";
    Dataset *trainData = Dataset_readFromFile(path);
    Subproblem *sp = Dataset_getSubproblem(trainData);
    DecisionTreeNode *tree = DecisionTree_create(sp, 0, 30, 1);
    printf("Génération d'un arbre de %d nœuds\n", Decision_nodeCount(tree));
    DecisionTree_destroy(tree);
    Dataset_destroy(trainData);
    return 0;
}
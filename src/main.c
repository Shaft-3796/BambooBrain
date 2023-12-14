#include "Dataset.h"

int main(int argc, char** argv){
    Dataset *trainData = Dataset_readFromFile("PENDIGITS_train.txt");
    Subproblem *subproblem = Dataset_getSubproblem(trainData);
    Subproblem_print(subproblem);
    Subproblem_destroy(subproblem);
    Dataset_destroy(trainData);
    return 0;
}
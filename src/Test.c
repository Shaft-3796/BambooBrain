#include "Test.h"

/**
* @brief test_dataset creates a datasets from a file, create a subproblem from it and print it.
*/
void test_dataset(char* filename) {
    Dataset *trainData = Dataset_readFromFile(filename);
    Subproblem *subproblem = Dataset_getSubproblem(trainData);
    Subproblem_print(subproblem);
    Subproblem_destroy(subproblem);
    Dataset_destroy(trainData);
}
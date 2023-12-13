#include "Dataset.h"

int main(){

    Dataset *data = Dataset_readFromFile("PENDIGITS_test.txt");

    Dataset_destroy(data);
    return 1;
}
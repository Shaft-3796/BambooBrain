#include "Dataset.h"

Dataset* Dataset_readFromFile(char* filename) {
    Dataset dataset;

    char path[100] = "../datasets/";
    strcat(path, filename);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error : Unable to open file");
        return NULL;
    }

    // Get dataset properties from its first line
    fscanf(file, "%d %d %d", &dataset.instanceCount, &dataset.classCount, &dataset.featureCount);

    Instance** instances = (Instance **) calloc(dataset.instanceCount, dataset.instanceCount * sizeof(Instance*));
    for (int i = 0; i < dataset.instanceCount; ++i) {
        instances[i] = (Instance*) calloc(1, sizeof(Instance));
        fscanf(file, "%d", &instances[i]->classID);

        // Insert values in the instance
        int *values = (int *) calloc(dataset.featureCount, sizeof(int));
        instances[i]->values = values;
        for (int j = 0; j < dataset.featureCount; ++j) {
            fscanf(file, "%d", &values[j]);
        }
    }

    fclose(file);

    return &dataset;
}

void Dataset_destroy(Dataset *data) {
    if (data == NULL) return;

    for (int i = 0; i < data->instanceCount; ++i) {
        free(data->instances[i].values);
    }

    free(data->instances);
    free(data);
}

Subproblem *Dataset_getSubproblem(Dataset *data) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem->instanceCount = data->instanceCount;
    subproblem->featureCount = data->featureCount;
    subproblem->instances = data->instances;
    subproblem->classCount = data->classCount;

    subproblem->instances = (Instance **) calloc(subproblem->instanceCount, sizeof(Instance*));
    for (int i = 0; i < subproblem->instanceCount; ++i) {
        subproblem->instances[i] = &data->instances[i];
    }
}
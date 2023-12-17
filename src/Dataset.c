#include "Dataset.h"


/* --- Dataset parsing --- */
FILE* open_dataset_file(char* filename) {
char path[100] = DATASETS_PATH;
    strcat(path, filename);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: Unable to open file\n");
        return NULL;
    }

    return file;
}

void parse_dataset_properties(FILE* file, Dataset* dataset) {
    fscanf(file, "%d %d %d", &dataset->instanceCount, &dataset->classCount, &dataset->featureCount);
}

void parse_instance(FILE* file, Instance* instance, const int featureCount) {
    // Features init
    instance->values = (int*)calloc(featureCount, sizeof(int));
    // Class id
    fscanf(file, "%d", &instance->classID);
    // features
    for (int j = 0; j < featureCount; ++j) {
        fscanf(file, "%d", &instance->values[j]);
    }
}

void parse_instances(FILE* file, Dataset* dataset) {
    dataset->instances = (Instance*)calloc(dataset->instanceCount, sizeof(Instance));

    for (int i = 0; i < dataset->instanceCount; ++i) {
        Instance *instance = &dataset->instances[i];
        parse_instance(file, instance, dataset->featureCount);
    }
}

/**
 * @brief Dataset_readFromFile reads a dataset from a file
 * @param filename the name of the file to read
 * @return a pointer to the dataset
 */
Dataset* Dataset_readFromFile(char* filename) {
    Dataset *dataset = (Dataset*)calloc(1, sizeof(Dataset));

    // Open the file
    FILE *file = open_dataset_file(filename);
    if (!file) return NULL;

    // Parse first line (dataset properties)
    parse_dataset_properties(file, dataset);

    // Parse instances
    parse_instances(file, dataset);

    fclose(file);
    return dataset;
}

/**
 * @brief Dataset_destroy frees the memory allocated for the dataset
 * @param dataset the dataset to free
 */
void Dataset_destroy(Dataset *dataset) {
    if (dataset == NULL) return;

    // Free instances features
    for (int i = 0; i < dataset->instanceCount; ++i) {
        free(dataset->instances[i].values);
    }

    free(dataset->instances);
    free(dataset);
}

/* --- Subproblem --- */

Subproblem *Dataset_getSubproblem(Dataset *data) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem->instanceCount = data->instanceCount;
    subproblem->capacity = data->instanceCount;
    subproblem->featureCount = data->featureCount;
    subproblem->instances = data->instances;
    subproblem->classCount = data->classCount;

    subproblem->instances = (Instance **) calloc(subproblem->instanceCount, sizeof(Instance*));
    for (int i = 0; i < subproblem->instanceCount; ++i) {
        subproblem->instances[i] = &data->instances[i];
    }

    subproblem->classes = (SubproblemClass*) calloc(subproblem->classCount, sizeof(SubproblemClass*));
    for (int i = 0; i < subproblem->classCount; ++i) {
        subproblem->classes[i] = *(SubproblemClass*) calloc(1, sizeof(SubproblemClass)); // !TODO Check if the cast is clean
    }

    int classID;
    for (int i = 0; i < subproblem->instanceCount; ++i) {
        classID = subproblem->instances[i]->classID;
        SubproblemClass *currentClass = &subproblem->classes[classID];

        // Resize the class instance and add it
        currentClass->instances = realloc(currentClass->instances,(currentClass->instanceCount) * sizeof(Instance));
        currentClass->instances[currentClass->instanceCount] = subproblem->instances[i];
        currentClass->instanceCount++;
    }

    return subproblem;
}

Subproblem *Subproblem_create(int maximumCapacity, int featureCount, int classCount) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem->featureCount = featureCount;
    subproblem->capacity = maximumCapacity;
    subproblem->classCount = classCount;

    subproblem->instances = (Instance **) calloc(maximumCapacity, sizeof(Instance*));
}

void Subproblem_destroy(Subproblem *subproblem) {
    if (subproblem == NULL) return;

    free(subproblem->instances);

    for (int i = 0; i < subproblem->classCount; ++i) {
        free(subproblem->classes[i].instances);
    }

    free(subproblem->classes);
    free(subproblem);
}

void Subproblem_insert(Subproblem *subproblem, Instance *instance) {
    int i=0;
    while (subproblem->instances[i] && i<subproblem->instanceCount) i++;
    subproblem->instances[i] = instance;
    subproblem->instanceCount ++;

    i=0;
    SubproblemClass *classes = subproblem->classes;
    while (classes->instances[i] && i<classes->instanceCount) i++;
    classes->instances[i] = instance;
    classes->instanceCount ++;

}

void Subproblem_print(Subproblem *subproblem) {
    printf("Dataset with %d classes of %d features\n", subproblem->classCount, subproblem->featureCount);
    printf("Size = %d, capacity = %d\n", subproblem->instanceCount, subproblem->capacity);

    for (int i = 0; i < subproblem->classCount; ++i) {
        printf("- class %d: %d instances\n", i, subproblem->classes[i].instanceCount);
    }

}
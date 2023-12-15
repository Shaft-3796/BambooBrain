#include "Dataset.h"

Dataset* Dataset_readFromFile(char* filename) {
    Dataset *dataset = (Dataset*)calloc(1, sizeof(Dataset));

    int instanceCount, classCount, featureCount;

    char path[100] = "../datasets/";
    strcat(path, filename);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: Unable to open file\n");
        return NULL;
    }

    // Get dataset properties from its first line
    fscanf(file, "%d %d %d", &instanceCount, &classCount, &featureCount);


    dataset->instanceCount = instanceCount;
    dataset->classCount = classCount;
    dataset->featureCount = featureCount;

    dataset->instances = (Instance*)calloc(instanceCount, sizeof(Instance));


    for (int i = 0; i < instanceCount; ++i) {
        Instance *instance = &dataset->instances[i];
        instance->values = (int*)calloc(featureCount, sizeof(int));
        fscanf(file, "%d", &instance->classID);

        // Insert values in the instance
        for (int j = 0; j < featureCount; ++j) {
            fscanf(file, "%d", &instance->values[j]);
        }
    }

    fclose(file);

    return dataset;
}


void Dataset_destroy(Dataset *data) {
    if (data == NULL) return;

    for (int i = 0; i < data->instanceCount; ++i) {
        free(data->instances[i].values);
    }

    // free(data->instances); !TODO
    free(data);
}

Subproblem *Dataset_getSubproblem(Dataset *data) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem->instanceCount = data->instanceCount;
    subproblem->capacity = data->instanceCount;
    subproblem->featureCount = data->featureCount;
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

    subproblem->instances = (Instance **)calloc(subproblem->capacity, sizeof(Instance*));

    subproblem->classes = (SubproblemClass *)calloc(classCount, sizeof(SubproblemClass));
    for (int i = 0; i < classCount; ++i) {
        subproblem->classes[i].instances = (Instance **)calloc(0, sizeof(Instance*));
    }

    return subproblem;
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
    subproblem->instances = realloc(subproblem->instances, (subproblem->instanceCount + 1) * sizeof(Instance*));
    subproblem->instances[subproblem->instanceCount] = instance;
    subproblem->instanceCount++;

    int classID = instance->classID;
    SubproblemClass *class = &subproblem->classes[classID];
    class->instances = realloc(class->instances, (class->instanceCount + 1) * sizeof(Instance*));
    class->instances[class->instanceCount] = instance;
    class->instanceCount++;
}


void Subproblem_print(Subproblem *subproblem) {
    printf("Dataset with %d classes of %d features\n", subproblem->classCount, subproblem->featureCount);
    printf("Size = %d, capacity = %d\n", subproblem->instanceCount, subproblem->capacity);

    for (int i = 0; i < subproblem->classCount; ++i) {
        printf("- class %d: %d instances\n", i, subproblem->classes[i].instanceCount);
    }

}
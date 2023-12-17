#include "Dataset.h"


/* --- Dataset parsing --- */
static FILE* open_dataset_file(char* filename) {
    char path[1024];
    snprintf(path, sizeof(path), "%s%s", DATASETS_PATH, filename);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: Unable to open file\n");
        return NULL;
    }

    return file;
}

static void parse_dataset_properties(FILE* file, Dataset* data) {
    fscanf(file, "%d %d %d", &data->instanceCount, &data->classCount, &data->featureCount);
}

static void parse_instance(FILE* file, Instance* instance, const int featureCount) {
    // Features init
    instance->values = (int*)calloc(featureCount, sizeof(int));
    // Class id
    fscanf(file, "%d", &instance->classID);
    // features
    for (int j = 0; j < featureCount; ++j) {
        fscanf(file, "%d", &instance->values[j]);
    }
}

static void parse_instances(FILE* file, Dataset* data) {
    data->instances = (Instance*)calloc(data->instanceCount, sizeof(Instance));

    for (int i = 0; i < data->instanceCount; ++i) {
        Instance *instance = &data->instances[i];
        parse_instance(file, instance, data->featureCount);
    }
}

/**
 * @brief Dataset_readFromFile reads a dataset from a file
 * @param filename the name of the file to read
 * @return a pointer to the dataset
 */
Dataset* Dataset_readFromFile(char* filename) {
    Dataset *data = (Dataset*)calloc(1, sizeof(Dataset));

    // Open the file
    FILE *file = open_dataset_file(filename);
    if (!file) return NULL;

    // Parse first line (dataset properties)
    parse_dataset_properties(file, data);

    // Parse instances
    parse_instances(file, data);

    fclose(file);
    return data;
}

/**
 * @brief Dataset_destroy frees the memory allocated for the dataset
 * @param data the dataset to free
 */
void Dataset_destroy(Dataset *data) {
    if (data == NULL) return;

    // Free instances features
    for (int i = 0; i < data->instanceCount; ++i) {
        free(data->instances[i].values);
    }

    free(data->instances);
    free(data);
}

/* --- Subproblem --- */
static void subproblem_copy_dataset_properties(Subproblem *subproblem, Dataset *data) {
    subproblem->instanceCount = data->instanceCount;
    subproblem->capacity = data->instanceCount;
    subproblem->featureCount = data->featureCount;
    subproblem->classCount = data->classCount;
}

static void subproblem_copy_dataset_instances(Subproblem *subproblem, Dataset *data) {
    subproblem->instances = (Instance **) calloc(subproblem->instanceCount, sizeof(Instance*));
    for (int i = 0; i < subproblem->instanceCount; ++i) {
        subproblem->instances[i] = &data->instances[i];
    }
}

static void subproblem_generate_classes(Subproblem *subproblem) {
    // Create the classes
    subproblem->classes = (SubproblemClass *) calloc(subproblem->classCount, sizeof(SubproblemClass));

    // Pre allocate the classes
    for (int i = 0; i < subproblem->classCount; ++i) {
        // We allocate the maximum possible size
        subproblem->classes[i].instanceCount = 0;
        subproblem->classes[i].instances = (Instance **) calloc(subproblem->instanceCount, sizeof(Instance*));
    }

    // For each instance
    for (int i = 0; i < subproblem->instanceCount; ++i) {
        const int classID = subproblem->instances[i]->classID;
        SubproblemClass *currentClass = &subproblem->classes[classID];

        // Add the instance to the class
        currentClass->instances[currentClass->instanceCount] = subproblem->instances[i];
        currentClass->instanceCount++;
    }

    // Reallocate the classes to the correct size
    for (int i = 0; i < subproblem->classCount; ++i) {
        subproblem->classes[i].instances = realloc(subproblem->classes[i].instances, subproblem->classes[i].instanceCount * sizeof(Instance*));
    }
}

/**
 * @brief Dataset_getSubproblem creates a subproblem from a dataset
 * @param data the dataset
 * @return a pointer to the subproblem
 */
Subproblem *Dataset_getSubproblem(Dataset *data) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem_copy_dataset_properties(subproblem, data);

    subproblem_copy_dataset_instances(subproblem, data);

    subproblem_generate_classes(subproblem);

    return subproblem;
}

/**
 * @brief Subproblem_create creates a subproblem from the given parameters
 * @param maximumCapacity the maximum capacity of the subproblem
 * @param featureCount the number of features
 * @param classCount the number of classes
 * @return a pointer to the subproblem
 */
Subproblem *Subproblem_create(int maximumCapacity, int featureCount, int classCount) {
    Subproblem *subproblem = (Subproblem *)calloc(1, sizeof(Subproblem));

    subproblem->featureCount = featureCount;
    subproblem->capacity = maximumCapacity;
    subproblem->classCount = classCount;


    subproblem->instances = (Instance **) calloc(subproblem->capacity, sizeof(Instance*));

    subproblem->classes = (SubproblemClass *)calloc(classCount, sizeof(SubproblemClass));
    for (int i = 0; i < classCount; ++i) {
        subproblem->classes[i].instances = (Instance **)calloc(maximumCapacity, sizeof(Instance*));
    }

    return subproblem;
}

static void subproblem_class_destroy(SubproblemClass *subproblemClass) {
    if (subproblemClass == NULL) return;

    free(subproblemClass->instances);
}

/**
 * @brief Subproblem_destroy frees the memory allocated for the subproblem
 * @param subproblem the subproblem to free
 */
void Subproblem_destroy(Subproblem *subproblem) {
    if (subproblem == NULL) return;

    free(subproblem->instances);

    for (int i = 0; i < subproblem->classCount; ++i) {
        subproblem_class_destroy(&subproblem->classes[i]);
    }

    free(subproblem->classes);
    free(subproblem);
}

/**
 * @brief Subproblem_insert inserts an instance into the subproblem
 * @param subproblem the subproblem
 * @param instance the instance to insert
 */
void Subproblem_insert(Subproblem *subproblem, Instance *instance) {
    // We do not need to reallocate since we allocated the maximum possible size
    subproblem->instances[subproblem->instanceCount] = instance;
    subproblem->instanceCount++;

    SubproblemClass *class = &subproblem->classes[instance->classID];
    // We do not need to reallocate since we allocated the maximum possible size
    class->instances[class->instanceCount] = instance;
    class->instanceCount++;
}

/**
 * @brief Subproblem_print prints the subproblem
 * @param subproblem the subproblem to print
 */
void Subproblem_print(Subproblem *subproblem) {
    printf("Dataset with %d classes of %d features\n", subproblem->classCount, subproblem->featureCount);
    printf("Size = %d, capacity = %d\n", subproblem->instanceCount, subproblem->capacity);

    for (int i = 0; i < subproblem->classCount; ++i) {
        printf("- class %d: %d instances\n", i, subproblem->classes[i].instanceCount);
    }

}
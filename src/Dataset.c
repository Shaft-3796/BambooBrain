#include "Dataset.h"


/* --- Dataset parsing --- */
/**
 * @brief open_dataset_file opens a dataset file, prints an error message if it fails
 * @param path the path to the file
 * @return returns a pointer to the file, or NULL if it fails
 */
static FILE* open_dataset_file(const char* path) {

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: Unable to open file\n");
        return NULL;
    }

    return file;
}

/**
 * @brief parse_dataset_properties parses the first line of a dataset file
 * @param file the file to parse
 * @param data the dataset to fill
 */
static void parse_dataset_properties(FILE* file, Dataset* data) {
    fscanf(file, "%d %d %d", &data->instance_count, &data->class_count, &data->feature_count);
}

/**
 * @brief parse_instance parses an instance from a line of a dataset file
 * @param file the file to parse
 * @param instance the instance to fill
 * @param feature_count the number of features
 */
static void parse_instance(FILE* file, Instance* instance, const int feature_count) {
    // Features init
    instance->values = (int*) calloc(feature_count, sizeof(int));

    // Class id
    fscanf(file, "%d", &instance->class_id);

    // features
    for (int j = 0; j < feature_count; ++j) {
        fscanf(file, "%d", &instance->values[j]);
    }
}

/**
 * @brief parse_instances parses the instances of a dataset file
 * @param file the file to parse
 * @param data the dataset to fill
 */
static void parse_instances(FILE* file, Dataset* data) {
    data->instances = (Instance*) calloc(data->instance_count, sizeof(Instance));

    for (int i = 0; i < data->instance_count; ++i) {
        Instance *instance = &data->instances[i];
        parse_instance(file, instance, data->feature_count);
    }
}

/**
 * @brief parse_dataset_from_file parses a dataset from a file
 * @param path the path to the file
 * @return a pointer to the dataset
 */
Dataset* parse_dataset_from_file(const char* path) {
    Dataset *data = (Dataset*) calloc(1, sizeof(Dataset));

    // Open the file
    FILE *file = open_dataset_file(path);
    if (!file) return NULL;

    // Parse first line (dataset properties)
    parse_dataset_properties(file, data);

    // Parse instances
    parse_instances(file, data);

    fclose(file);
    return data;
}

/**
 * @brief destroy_dataset frees the memory allocated for the dataset
 * @param data the dataset to free
 */
void destroy_dataset(Dataset *data) {
    if (data == NULL) return;

    // Free instances features
    for (int i = 0; i < data->instance_count; ++i) {
        free(data->instances[i].values);
    }

    free(data->instances);
    free(data);
}


/* --- Subproblem --- */
/**
 * @brief create_subproblem creates a subproblem from properties
 * @param max_capacity the maximum capacity of the subproblem
 * @param feature_count the number of features
 * @param class_count the number of classes
 */
Subproblem *create_subproblem(const int max_capacity, const int feature_count, const int class_count) {
    Subproblem *sp = (Subproblem*) calloc(1, sizeof(Subproblem));

    // Properties
    sp->capacity = max_capacity;
    sp->feature_count = feature_count;
    sp->class_count = class_count;

    // Instances
    sp->instances = (Instance**) calloc(max_capacity, sizeof(Instance*));

    // Classes
    sp->classes = (SubproblemClass*) calloc(class_count, sizeof(SubproblemClass));
    for (int i = 0; i < class_count; ++i) {
        sp->classes[i].instances = (Instance**) calloc(max_capacity, sizeof(Instance*));
    }

    return sp;
}

/**
 * @brief insert_subproblem_instance inserts an instance into a subproblem
 * @param sp the subproblem
 * @param instance the instance to insert
 */
void insert_subproblem_instance(Subproblem *sp, Instance *instance) {
    // We do not need to reallocate since we allocated the maximum possible size
    sp->instances[sp->instance_count] = instance;
    sp->instance_count++;

    SubproblemClass *class = &sp->classes[instance->class_id];
    // We do not need to reallocate since we allocated the maximum possible size
    class->instances[class->instance_count] = instance;
    class->instance_count++;

    // Update the majority class
    if (class->instance_count > sp->majority_class_instance_count) {
        sp->majority_class_instance_count = class->instance_count;
        sp->majority_class_id = instance->class_id;
    }
}

/**
 * @brief free_subproblem_excess_memory frees the memory allocated for the subproblem that is not used, RAM optimization
 * @param sp the subproblem
 */
void free_subproblem_excess_memory(Subproblem *sp) {
    // Reallocate the instances to the correct size
    sp->instances = realloc(sp->instances, sp->instance_count * sizeof(Instance*));

    // Reallocate the classes to the correct size
    for (int i = 0; i < sp->class_count; ++i) {
        sp->classes[i].instances = realloc(sp->classes[i].instances, sp->classes[i].instance_count * sizeof(Instance*));
    }
}

/**
 * @brief print_subproblem prints the subproblem
 * @param sp the subproblem to print
 */
void print_subproblem(Subproblem *sp) {
    printf("Dataset with %d classes of %d features\n", sp->class_count, sp->feature_count);
    printf("Size = %d, capacity = %d\n", sp->instance_count, sp->capacity);

    for (int i = 0; i < sp->class_count; ++i) {
        printf("- class %d: %d instances\n", i, sp->classes[i].instance_count);
    }

}

/**
 * @brief destroy_subproblem frees the memory allocated for the subproblem
 * @param sp the subproblem to free
 */
void destroy_subproblem(Subproblem *sp) {
    if (sp == NULL) return;

    free(sp->instances);

    for (int i = 0; i < sp->class_count; ++i) {
        free(sp->classes[i].instances);
    }

    free(sp->classes);
    free(sp);
}

/**
 * @brief create_subproblem_from_dataset creates a subproblem from a dataset
 * @param data the dataset
 * @return a pointer to the subproblem
 */
Subproblem *create_subproblem_from_dataset(const Dataset *data) {
    Subproblem *sp = create_subproblem(data->instance_count, data->feature_count, data->class_count);

    for (int i = 0; i < data->instance_count; ++i) {
        insert_subproblem_instance(sp, &data->instances[i]);
    }

    return sp;
}

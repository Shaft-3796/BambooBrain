#include "Bagging.h"

static int* gen_random_integers(const int from, const int to, const int count) {
    int *integers = (int*)calloc(count, sizeof(int));
    for(int i=0; i<count; ++i) integers[i] = rand() % (to-from) + from;
    return integers;
}

/**
 * @brief Generate a subproblem by randomly selecting a proportion of instances (BAGGING_MODE_PROPORTIONAL)
 * @param config the configuration for the bagging function
 * @param args mode specific arguments for the bagging function
 * No arguments are expected.
 * @param data the dataset
 * @param count the number of bags
 * @return a list of subproblems
 */
Subproblem **bagging_from_proportion(const BaggingConfig *config, const BaggingArgs *args, const Dataset *data, const int count) {
    Subproblem **subproblems = (Subproblem **) calloc(count, sizeof(Subproblem*));

    for(int i=0; i<count; i++) {
        int *integers = gen_random_integers(0, data->instance_count, (int)(data->instance_count * config->proportion));
        subproblems[i] = create_subproblem((int)(data->instance_count * config->proportion), data->feature_count, data->class_count);
        for(int j=0; j<(int)(data->instance_count * config->proportion); j++) insert_subproblem_instance(subproblems[i], &data->instances[integers[j]]);
        free(integers);  // Todo: potential segfault but why ??
    }

    return subproblems;
}

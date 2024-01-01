#include "Bagging.h"


static int* gen_random_integers(const int from, const int to, const int count) {
    int *integers = (int*)calloc(count, sizeof(int));
    for(int i=0; i<count; ++i) integers[i] = rand() % (to-from) + from;
    return integers;
}

/**
 * @brief Generate subproblems by randomly selecting a proportion of instances (BAGGING_MODE_PROPORTIONAL)
 * @param config the configuration for the bagging function
 * - proportion: the proportion of instances to select
 * @param data the dataset
 * @param count the number of bags
 * @return a list of subproblems
 */
Subproblem **bagging_from_proportion(const Config *config, const Dataset *data, int count) {
    Subproblem **subproblems = (Subproblem **) calloc(count, sizeof(Subproblem*));

    for(int i=0; i<count; i++) {
        int *integers = gen_random_integers(0, data->instance_count, (int)(data->instance_count * config->bagging_proportion));
        subproblems[i] = create_subproblem((int)(data->instance_count * config->bagging_proportion), data->feature_count, data->class_count);
        for(int j=0; j<(int)(data->instance_count * config->bagging_proportion); j++) insert_subproblem_instance(subproblems[i], &data->instances[integers[j]]);
        free(integers);
    }

    return subproblems;
}

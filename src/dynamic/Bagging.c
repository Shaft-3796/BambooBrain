#include "Bagging.h"

static int* gen_random_integers(const int from, const int to, const int count) {
    int *integers = (int*)calloc(count, sizeof(int));
    for(int i=0; i<count; ++i) integers[i] = rand() % (to-from) + from;
    return integers;
}

/**
 * @brief Generate a subproblem by randomly selecting a proportion of instances
 * @param data the dataset
 * @param bags the number of bags
 * @param proportion the proportion of instances to select
 * @return a list of subproblems
 */
Subproblem **bagging_from_proportion(const Dataset *data, const int bags, const float proportion) {
    Subproblem **subproblems = (Subproblem **)calloc(bags, sizeof(Subproblem*));

    for(int i=0; i<bags; i++) {
        int *integers = gen_random_integers(0, data->instance_count, (int)(data->instance_count * proportion));
        subproblems[i] = create_subproblem((int)(data->instance_count * proportion), data->feature_count, data->class_count);
        for(int j=0; j<(int)(data->instance_count * proportion); j++) insert_subproblem_instance(subproblems[i], &data->instances[integers[j]]);
        free(integers);  // Todo: potential segfault but why ??
    }

    return subproblems;
}

/**
 * @brief Generate subproblems by bagging a dataset
 * @param args the arguments for the bagging function
 * @return a list of subproblems
 */
Subproblem **bagg_dataset(const BaggingArgs *args) {

    switch(args->mode) {
        case BAGGING_MODE_PROPORTION:
            return bagging_from_proportion(args->data, args->bags, args->proportion);
        default:
            printf("Error: unknown bagging mode\n");
            return NULL;
    }
}

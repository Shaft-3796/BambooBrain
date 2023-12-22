#include "Hyperopt.h"

#include "ApplyConfig.h"


/**
 * @brief creates an hyperopt object
 * @param config the configuration
 * @return the hyperopt object
 */
Hyperopt *create_hyperopt(Config *config) {
    Hyperopt *hyperopt = (Hyperopt*) calloc(1, sizeof(Hyperopt));
    hyperopt->config = config;
    return hyperopt;
}

/**
 * @brief add an hypervariable to the hyperopt object
 * @param hyperopt the hyperopt object
 * @param variable a pointer to the variable of the configuration
 * @param type the type of the variable
 * @float start the start value
 * @float step the step value
 * @bool if it is the first variable or the second
 * @int count the number of values to test
 */
void add_hypervar(Hyperopt *hyperopt, void *variable, const HyperVariableType type, const float start, const float step, const int count) {
    if(hyperopt->v1 == NULL) {
        hyperopt->v1 = (HyperVariable*) calloc(1, sizeof(HyperVariable));
        hyperopt->v1->variable = variable;
        hyperopt->v1->type = type;
        hyperopt->v1->start = start;
        hyperopt->v1->step = step;
        hyperopt->v1->count = count;

        hyperopt->results = (float**) calloc(count, sizeof(float*));
    } else {
        hyperopt->v2 = (HyperVariable*) calloc(1, sizeof(HyperVariable));
        hyperopt->v2->variable = variable;
        hyperopt->v2->type = type;
        hyperopt->v2->start = start;
        hyperopt->v2->step = step;
        hyperopt->v2->count = count;

        for(int i=0; i<hyperopt->v1->count; ++i) {
            hyperopt->results[i] = (float*) calloc(count, sizeof(float));
        }
    }
}


/**
 * @brief destroy an hyperopt object
 * @param hyperopt the hyperopt object
 */
void destroy_hyperopt(Hyperopt *hyperopt) {
    if(hyperopt->v1 != NULL) {
        for(int i=0; i<hyperopt->v1->count; ++i) {
            free(hyperopt->results[i]);
        }
        free(hyperopt->results);
    }
    free(hyperopt->v1);
    free(hyperopt->v2);
    free(hyperopt);
}

/**
 * @brief run the hyperopt
 * @param path the path to the dataset
 * @param hyperopt the hyperopt object
 * @param train_data the training dataset
 * @param opt_data the testing dataset
 */
void run_hyperopt(const char* path, Hyperopt *hyperopt, Dataset *train_data, Dataset *opt_data) {
    assert(hyperopt->v1 && hyperopt->v2);

    printf("[--- RUNNING HYPEROPT ---]\n");

    FILE *file = fopen(path, "w");

    // Wi save into the file x_axis values and under it y_axis values
    for(float v2=hyperopt->v2->start; v2<hyperopt->v2->start+hyperopt->v2->step*hyperopt->v2->count; v2+=hyperopt->v2->step) {
        fprintf(file, "%f ", v2);
    }
    fprintf(file, "\n");
    for(float v1=hyperopt->v1->start; v1<hyperopt->v1->start+hyperopt->v1->step*hyperopt->v1->count; v1+=hyperopt->v1->step) {
        fprintf(file, "%f ", v1);
    }
    fprintf(file, "\n");

    int i=0;
    for(float v1=hyperopt->v1->start; v1<hyperopt->v1->start+hyperopt->v1->step*hyperopt->v1->count; v1+=hyperopt->v1->step, i++) {
        int j=0;
        for(float v2=hyperopt->v2->start; v2<hyperopt->v2->start+hyperopt->v2->step*hyperopt->v2->count; v2+=hyperopt->v2->step, j++) {
            printf("[HYPEROPT] %d/%d ", i*hyperopt->v2->count+j+1, hyperopt->v1->count*hyperopt->v2->count);
            switch(hyperopt->v1->type) {
                case HYPER_VARIABLE_TYPE_INT:
                    *(int*) hyperopt->v1->variable = (int) v1;
                printf("v1:%d ", *(int*) hyperopt->v1->variable);
                break;
                case HYPER_VARIABLE_TYPE_FLOAT:
                    *(float*) hyperopt->v1->variable = v1;
                printf("v1:%f ", *(float*) hyperopt->v1->variable);
                break;
            }

            switch(hyperopt->v2->type) {
                case HYPER_VARIABLE_TYPE_INT:
                    *(int*) hyperopt->v2->variable = (int) v2;
                    printf("v2:%d\n", *(int*) hyperopt->v2->variable);
                break;
                case HYPER_VARIABLE_TYPE_FLOAT:
                    *(float*) hyperopt->v2->variable = v2;
                    printf("v2:%f\n", *(float*) hyperopt->v2->variable);
                break;
            }

            apply_config(hyperopt->config);

            Model *model = hyperopt->config->create_model(hyperopt->config, train_data);
            const float test_accuracy = evaluate_model(hyperopt->config, model, opt_data);
            hyperopt->results[i][j] = test_accuracy;

            printf("Got %.2f%% for [%d]-[%d]\n", test_accuracy, i, j);

            fprintf(file, "%f ", test_accuracy);
            destroy_model(model);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
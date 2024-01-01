// Probably overfitted, but let's close our eyes on this one

#pragma once

#include "Config.h"
#include "Progress.h"
#include "ModelTools.h"

typedef enum eHyperVariableType {
    HYPER_VARIABLE_TYPE_INT,
    HYPER_VARIABLE_TYPE_FLOAT,
} HyperVariableType;

typedef struct sHyperVariable {
    HyperVariableType type;
    void *variable;

    float start;
    float step;
    int count;

} HyperVariable;

typedef struct sHyperopt {
    Config *config;

    HyperVariable *v1;
    HyperVariable *v2;

    float **results;
} Hyperopt;


/**
 * @brief creates an hyperopt object
 * @param config the configuration
 * @return the hyperopt object
 */
Hyperopt *create_hyperopt(Config *config);
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
void add_hypervar(Hyperopt *hyperopt, void *variable, const HyperVariableType type, const float start, const float step, const int count);


/**
 * @brief destroy an hyperopt object
 * @param hyperopt the hyperopt object
 */
void destroy_hyperopt(Hyperopt *hyperopt);

/**
 * @brief run the hyperopt
 * @param path the path to the dataset
 * @param hyperopt the hyperopt object
 * @param train_data the training dataset
 * @param opt_data the testing dataset
 */
void run_hyperopt(const char* path, Hyperopt *hyperopt, Dataset *train_data, Dataset *opt_data);
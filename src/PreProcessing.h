#pragma once

#include "Config.h"
#include "Dataset.h"

/**
 * @brief Apply pre processing steps to an instance
 * @param config the configuration
 * @param instance the instance
 * @param feature_count the number of features
 */
int apply_pp_steps_to_instance(const Config *config, Instance *instance, int feature_count);

/**
 * @brief Apply pre processing steps to a dataset
 * @param config the configuration
 * @param data the dataset
 */
void apply_pp_steps_to_dataset(const Config *config, Dataset *data);
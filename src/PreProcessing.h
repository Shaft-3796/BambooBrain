#pragma once

#include "Config.h"
#include "Dataset.h"


/**
 * @brief Apply pre processing steps to a dataset
 * @param config the configuration
 * @param data the dataset
 */
void apply_pp_steps_to_dataset(const Config *config, Dataset *data);
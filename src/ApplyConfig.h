#pragma once

#include "Config.h"
#include "dynamic/CreateModel.h"
#include "dynamic/CreateTree.h"
#include "dynamic/ComputeSplit.h"
#include "dynamic/Impurity.h"
#include "dynamic/Threshold.h"
#include "dynamic/Bagging.h"
#include "dynamic/PredictFromModel.h"
#include "dynamic/PredictFromTree.h"
#include "dynamic/PreProcessInstance.h"


/**
 * @brief apply_config applies a configuration
 * @param config the configuration
 */
void apply_config(Config *config);

/**
 * @brief Add a preprocessing step to the configuration
 * @param config the configuration
 * @param step the preprocessing step
 * @param merge_mode the merge mode
 */
void add_pp_step(Config *config, PreProcessingStep step, PreProcessingMergeMode merge_mode);
#pragma once

#include "Config.h"
#include "CreateModel.h"
#include "CreateTree.h"
#include "ComputeSplit.h"
#include "Impurity.h"
#include "Threshold.h"
#include "Bagging.h"
#include "PredictFromModel.h"
#include "PredictFromTree.h"
#include "PreProcessInstance.h"


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
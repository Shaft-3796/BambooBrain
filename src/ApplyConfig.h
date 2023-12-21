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


/**
 * @brief apply_config applies a configuration
 * @param config the configuration
 */
void apply_config(Config *config);
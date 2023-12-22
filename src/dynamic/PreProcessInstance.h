#pragma once

#include "../Config.h"

/**
 * @brief Preprocess an instance to set all features to only 0 or 255 (black or white)
 * @param config the configuration
 * @param features the features to preprocess
 * @param feature_count the number of features
 * @param new_features the new features
 * @param new_feature_count the new number of features
 */
void instance_to_black_and_white(const Config *config, const int *features, int feature_count, int **new_features, int *new_feature_count);

/**
 * @brief Preprocess an instance to center all features horizontally
 * @param config the configuration
 * @param features the features to preprocess
 * @param feature_count the number of features
 * @param new_features the new features
 * @param new_feature_count the new number of features
 */
void center_instance(const Config *config, const int *features, const int feature_count, int **new_features, int *new_feature_count);
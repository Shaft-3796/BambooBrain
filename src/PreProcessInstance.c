#include "PreProcessInstance.h"

/**
 * @brief Preprocess an instance to set all features to only 0 or 255 (black or white)
 * @param config the configuration
 * @param features the features to preprocess
 * @param feature_count the number of features
 * @param new_features the new features
 * @param new_feature_count the new number of features
 */
void instance_to_black_and_white(const Config *config, const int *features, const int feature_count, int **new_features, int *new_feature_count) {
    *new_feature_count = feature_count;
    *new_features = (int *) calloc(*new_feature_count, sizeof(int));

    for (int i = 0; i < feature_count; ++i) {
        (*new_features)[i] = features[i] > 0 ? 255 : 0;
    }
}
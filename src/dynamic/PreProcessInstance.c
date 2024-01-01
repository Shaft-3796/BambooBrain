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


/**
 * @brief Preprocess an instance to center all features horizontally
 * @param config the configuration
 * @param features the features to preprocess
 * @param feature_count the number of features
 * @param new_features the new features
 * @param new_feature_count the new number of features
 */
void center_instance(const Config *config, const int *features, const int feature_count, int **new_features, int *new_feature_count) {
    *new_feature_count = feature_count;
    *new_features = (int *) calloc(*new_feature_count, sizeof(int));

    bool found = false;
    const int width = (int)sqrtf(feature_count);
    int borderL = 0, borderR = width;

    // Get the left border
    for (int i = 0; i < width && !found; ++i) {
        for (int j = 0; j < width && !found; ++j) {
            int feature = features[j * width + i];
            if (feature) {
                borderL = i;
                found = true;
            }
        }
    }

    found = false;

    // Get the right border
    for (int i = width; i > 0 && !found; --i) {
        for (int j = 0; j < width && !found; ++j) {
            int feature = features[j * width + i];
            if (feature) {
                borderR = i;
                found = true;
            }
        }
    }


    // Calculate the correction factor
    float texture_mid = width/2;
    float drawing_mid = (borderL + borderR)/2;
    int correction = texture_mid - drawing_mid;


    int idx = 0;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < width; ++j, ++idx) {

            // Add the horizontal correction
            const int newj = fmax(0, fmin(width, j-correction));

            (*new_features)[idx] = features[i * width + newj];
        }
    }

}
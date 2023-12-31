#include "PreProcessing.h"

/**
 * @brief Apply pre processing steps to an instance
 * @param config the configuration
 * @param instance the instance
 * @param feature_count the number of features
 */
int apply_pp_steps_to_instance(const Config *config, Instance *instance, const int feature_count) {
    int *new_features;
    int new_feature_count;

    int final_feature_count = feature_count;

    // For features do add at the end of the preprocessing
    int add_later_count = 0;
    int *new_features_count_array = (int *) calloc(config->pre_processing_step_count, sizeof(int));
    int **new_features_array = (int **) calloc(config->pre_processing_step_count, sizeof(int *));


    for (int i = 0; i < config->pre_processing_step_count; ++i) {
        switch (config->pre_processing_merge_modes[i]) {
            case PP_MERGE_MODE_REPLACE:
                config->pre_processing_functions[i](config, instance->values, final_feature_count, &new_features, &new_feature_count);
                free( instance->values);
                instance->values = new_features;
                final_feature_count = new_feature_count;
                break;
            case PP_MERGE_MODE_ADD_NOW:
                config->pre_processing_functions[i](config,  instance->values, feature_count, &new_features, &new_feature_count);
                 instance->values = realloc( instance->values, (feature_count + new_feature_count) * sizeof(int));
                for (int j = 0; j < new_feature_count; ++j) {
                     instance->values[feature_count + j] = new_features[j];
                }
                final_feature_count += new_feature_count;
                break;
            case PP_MERGE_MODE_ADD_LATER:
                config->pre_processing_functions[i](config,  instance->values, feature_count, &new_features_array[add_later_count], &new_features_count_array[add_later_count]);
                add_later_count++;
                break;
        }
    }

    // Add features at the end
    for (int i = 0; i < add_later_count; ++i) {
        instance->values = realloc( instance->values, (final_feature_count + new_features_count_array[i]) * sizeof(int));
        for (int j = 0; j < new_features_count_array[i]; ++j) {
             instance->values[final_feature_count + j] = new_features_array[i][j];
        }
        final_feature_count += new_features_count_array[i];
    }

    free(new_features_count_array);
    free(new_features_array);

    return final_feature_count;
}


/**
 * @brief Apply pre processing steps to an instance
 * @param config the configuration
 * @param data the dataset
 * @param instance_id the instance id
 */
static int apply_pp_steps_to_dataset_instance(const Config *config, const Dataset *data, const int instance_id) {
    return apply_pp_steps_to_instance(config, &data->instances[instance_id], data->feature_count);
}

/**
 * @brief Apply pre processing steps to a dataset
 * @param config the configuration
 * @param data the dataset
 */
void apply_pp_steps_to_dataset(const Config *config, Dataset *data) {
    int new_feature_count = data->feature_count;
    for (int i = 0; i < data->instance_count; ++i) {
        new_feature_count = apply_pp_steps_to_dataset_instance(config, data, i);
    }
    data->feature_count = new_feature_count;
}
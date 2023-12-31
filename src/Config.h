#pragma once

#include "Settings.h"
#include "Dataset.h"
#include "Model.h"
#include "Split.h"
#include "Predictions.h"
#include "DecisionTree.h"


/**
 * @brief PredictFromTreeMode the mode of the predict_from_tree function
 * PREDICT_FROM_TREE_MODE_THRESHOLD: predict the class of an instance based on a threshold
 * PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: predict the class of an instance based on a sigmoid score
 */
typedef enum PredictFromTree {
    PREDICT_FROM_TREE_MODE_THRESHOLD,
    PREDICT_FROM_TREE_MODE_SIGMOID_SCORE,
} PredictFromTreeMode;

/**
 * @brief CreateTreeMode the mode of the create_tree function
 * CREATE_TREE_MODE_PRUNNING_THRESHOLD: create a tree based on a prunning threshold
 */
typedef enum CreateTree {
    CREATE_TREE_MODE_PRUNNING_THRESHOLD,
} CreateTreeMode;

/**
 * @brief ComputeSplitMode the mode of the split_compute function
 * COMPUTE_SPLIT_MODE_PUREST_FEATURE: the feature with the threshold that gives the purest split
 */
typedef enum ComputeSplitMode {
    COMPUTE_SPLIT_MODE_PUREST_FEATURE,
} ComputeSplitMode;

/**
 * @brief ThresholdMode the mode of the threshold function
 * THRESHOLD_MODE_MID_MIN_MAX: (minFeatureValue + maxFeatureValue) / 2
 * THRESHOLD_MODE_ALL_VALUES: all values between min and max with a step
 */
typedef enum Threshold {
    THRESHOLD_MODE_MID_MIN_MAX,
    THRESHOLD_MODE_ALL_VALUES,
} ThresholdMode;

/**
 * @brief ImpurityMode the mode of the impurity function
 * BAGGING_MODE_GINI: Gini impurity
 */
typedef enum ImpurityMode {
    IMPURITY_MODE_GINI,
} ImpurityMode;

/**
 * @brief BaggingMode the mode of the bagging function
 * BAGGING_MODE_PROPORTIONAL: Generate a subproblem by randomly selecting a proportion of instances
 */
typedef enum BaggingMode {
    BAGGING_MODE_PROPORTIONAL,
} BaggingMode;

/**
 * @brief PreProcessingStep steps of the preprocessing
 * PP_STEP_BLACK_AND_WHITE: convert the image to black and white
 * PP_STEP_CENTER: center the image horizontally
 */
typedef enum PreProcessingStep {
    PP_STEP_BLACK_AND_WHITE,
    PP_STEP_CENTER,
} PreProcessingStep;

/**
 * @brief PreProcessingMergeMode how new features will be merged with the original instance
 * PP_MERGE_MODE_REPLACE: replace the original features with the new ones
 * PP_MERGE_MODE_ADD_NOW: add the new features to the original ones directly
 * PP_MERGE_MODE_ADD_LATER: add the new features to the original ones when the preprocessing is done
 */
typedef enum PreProcessingMergeMode {
    PP_MERGE_MODE_REPLACE,
    PP_MERGE_MODE_ADD_NOW,
    PP_MERGE_MODE_ADD_LATER,
} PreProcessingMergeMode;

struct sConfig;  // Forward declaration

typedef void (PreProcessingFunction)(const struct sConfig *config, const int *features, int feature_count, int **new_features, int *new_feature_count);

/**
 * @brief The configuration for the BambouBrain Algorithm
 * @param model_mode the mode of the model
 * - MODEL_MODE_TREE: simple decision tree, require: create_tree_mode
 * - MODEL_MODE_RANDOM_FOREST: random forest, require: bagging_mode, create_tree_mode, tree_count
 * @param create_tree_mode the mode of the create_tree function
 * - CREATE_TREE_MODE_PRUNNING_THRESHOLD: create a tree based on a prunning threshold,
 * require: compute_split_mode, max_tree_depth, prunning_threshold
 * @param predict_from_tree_mode the mode of the predict_from_tree function
 * - PREDICT_FROM_TREE_MODE_THRESHOLD: predict the class of an instance based on a threshold
 * - PREDICT_FROM_TREE_MODE_SIGMOID_SCORE: predict the class of an instance based on a sigmoid score, require: sigmoid_lambda
 * @param compute_split_mode the mode of the compute_split function
 * - COMPUTE_SPLIT_MODE_PUREST_FEATURE: the feature with the threshold that gives the purest split, require: threshold_mode, impurity_mode
 * @param threshold_mode the mode of the threshold function
 * - THRESHOLD_MODE_MID_MIN_MAX: the threshold is the middle of the min and max values
 * - THRSHOLD_MODE_ALL_VALUES: all values between min and max with a step, require: threshold_step
 * @param impurity_mode the mode of the impurity function
 * - IMPURITY_MODE_GINI: Gini impurity
 * @param bagging_mode the mode of the bagging function
 * - BAGGING_MODE_PROPORTIONAL: proportional bagging, require: bagging_proportion
*/
typedef struct sConfig {
    const ModelMode model_mode;
    const CreateTreeMode create_tree_mode;
    const PredictFromTreeMode predict_from_tree_mode;
    const ComputeSplitMode compute_split_mode;
    const ThresholdMode threshold_mode;
    const ImpurityMode impurity_mode;
    const BaggingMode bagging_mode;

    /* ModelConfig */
    int tree_count; // Number of tree, for MODEL_MODE_RANDOM_FOREST

    /* CreateTreeConfig */
    int max_tree_depth;  // Max depth of the tree, for CREATE_TREE_MODE_PRUNNING_THRESHOLD
    float prunning_threshold;  // Prunning threshold, for CREATE_TREE_MODE_PRUNNING_THRESHOLD

    /* PredictFromTreeConfig */
    float sigmoid_lambda;  // Lambda parameter for the sigmoid function, for PREDICT_FROM_TREE_MODE_SIGMOID_SCORE

    /* ThresholdConfig */
    int threshold_step;  // Step to use when generating thresholds, for THRESHOLD_MODE_ALL_VALUES

    /* BaggingConfig */
    float bagging_proportion;  // Proportion of instances for each subproblem, for BAGGING_MODE_PROPORTIONAL

    /* FeatureBaggingConfig */
    float feature_bagging_proportion;

    /* UserInterfaceConfig */
    float pencil_radius; // Size of pencil to draw
    float tickrate; // Predicting frequency while drawing (in milliseconds)

    /* PreProcessingConfig */

    /* Additional arguments YOU DO NOT NEED TO CONFIGURE IT */
    bool *feature_bagging;  // Array of bool, true if we can use the feature at the index
    int feature_bagging_count;  // Number of features we can use

    /* Function Pointers & other AUTO GENERATED */
    Model* (*create_model)(const struct sConfig *config, const Dataset *data);
    DecisionTreeNode* (*create_tree)(const struct sConfig *config, const Subproblem *sp);
    Split (*compute_split)(const struct sConfig *config, const Subproblem *sp);
    float *(*get_thresholds)(const struct sConfig *config, const Subproblem *sp, int feature_id);
    float (*get_impurity)(const struct sConfig *config, const Subproblem *sp, int feature_id, int threshold);
    Subproblem **(*apply_bagging)(const struct sConfig *config, const Dataset *data, int count);
    Predictions *(*predict_from_tree)(const struct sConfig *config, const DecisionTreeNode *tree, const Instance *instance);
    Predictions *(*predict_from_model)(const struct sConfig *config, const Model *model, const Instance *instance);
    int pre_processing_step_count;
    PreProcessingStep *pre_processing_steps;
    PreProcessingFunction **pre_processing_functions;
    PreProcessingMergeMode *pre_processing_merge_modes;
} Config;

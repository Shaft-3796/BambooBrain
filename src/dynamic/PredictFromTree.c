#include "PredictFromTree.h"

/**
 * @brief predict_from_tree_and_threshold predicts the class of an instance based on a threshold (PREDICT_FROM_TREE_MODE_THRESHOLD)
 * @param config the configuration for the predict_from_tree function
 * @param args mode specific arguments for the predict_from_tree function
 * No arguments are expected.
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
float predict_from_tree_and_threshold(const PredictFromTreeConfig *config, const PredictFromTreeArgs *args, const DecisionTreeNode *tree, const Instance *instance) {
    if(tree->class_id != -1) return tree->class_id;  // Leaf node

    if(instance->values[tree->split.feature_id] <= tree->split.threshold)
        return predict_from_tree_and_threshold(config, args, tree->left, instance);

    return predict_from_tree_and_threshold(config, args, tree->right, instance);
}
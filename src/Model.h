#pragma once

#include "DecisionTree.h"

/**
 * @brief ModelMode the mode of the model
 * MODEL_MODE_RANDOM_FOREST: random forest
 */
typedef enum Model {
    MODEL_MODE_RANDOM_FOREST,
} ModelMode;

/**
 * @brief Model the model
 * Common:
 * @param class_count the number of classes in the model
 * Specific:
 * @param trees used for MODEL_MODE_RANDOM_FOREST to store the trees
 * @param tree_count used for MODEL_MODE_RANDOM_FOREST to store the number of trees
 */
typedef struct sModel{
    ModelMode mode;
    /* Common arguments */
    int class_count;
    /* Specific arguments */
    DecisionTreeNode** trees;
    int tree_count;
} Model;
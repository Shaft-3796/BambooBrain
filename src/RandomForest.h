#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H

#include "DecisionTree.h"

typedef struct sRandomForest{
    DecisionTreeNode** trees;
    int treeCount;
    int classCount;
} RandomForest;

/**
 * @brief RandomForest_create creates a random forest
 * @param numberOfTrees the number of trees in the forest
 * @param data the dataset
 * @param maxDepth the maximum depth of each tree
 * @param baggingProportion the proportion of instances to select for each tree
 * @param prunningThreshold the prunning threshold
 * @return a pointer to the random forest
 */
RandomForest *RandomForest_create(
    int numberOfTrees,
    Dataset *data,
    int maxDepth,
    float baggingProportion,
    float prunningThreshold
    );

/**
 * @brief RandomForest_predict predicts the class of an instance
 * @param rf the random forest
 * @param instance the instance
 * @return the class id
 */
int RandomForest_predict(RandomForest *rf, Instance *instance);

/**
 * @brief RandomForest_predictAll predicts the class of all instances in a dataset
 * @param rf the random forest
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* RandomForest_predictAll(RandomForest *rf, Dataset *data);

/**
 * @brief RandomForest_evaluate evaluates the accuracy of a random forest on a dataset
 * @param rf the random forest
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float RandomForest_evaluate(RandomForest *rf, Dataset *data);

/**
 * @brief RandomForest_nodeCount counts the number of nodes in a random forest
 * @param rf the random forest
 * @return the number of nodes
 */
int RandomForest_nodeCount(RandomForest *rf);

/**
 * @brief RandomForest_destroy destroys a random forest
 * @param rf the random forest
 */
void RandomForest_destroy(RandomForest *rf);

#endif //RANDOMFOREST_H

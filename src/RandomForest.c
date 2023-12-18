#include "RandomForest.h"

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
    ) {
    // Create the random forest
    RandomForest *rf = (RandomForest*)calloc(1, sizeof(RandomForest));
    rf->treeCount = numberOfTrees;
    rf->classCount = data->classCount;
    rf->trees = (DecisionTreeNode**)calloc(numberOfTrees, sizeof(DecisionTreeNode*));

    // Fill the random forest with trees
    for(int i=0; i<numberOfTrees; ++i) {

        Subproblem *sp = Dataset_bagging(data, baggingProportion);
        rf->trees[i] = DecisionTree_create(sp, 0, maxDepth, prunningThreshold);
        Subproblem_destroy(sp);
    }

    return rf;
}

/**
 * @brief RandomForest_predict predicts the class of an instance
 * @param rf the random forest
 * @param instance the instance
 * @return the class id
 */
int RandomForest_predict(RandomForest *rf, Instance *instance) {
    int *votes = (int*)calloc(rf->classCount, sizeof(int));

    for(int i=0; i<rf->treeCount; ++i) {
        votes[DecisionTree_predict(rf->trees[i], instance)]++;
    }

    int classId = 0;
    for(int i=0; i<rf->classCount; ++i) {
        if(votes[i] > votes[classId]) classId = i;
    }

    free(votes);

    return classId;
}

/**
 * @brief RandomForest_predictAll predicts the class of all instances in a dataset
 * @param rf the random forest
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* RandomForest_predictAll(RandomForest *rf, Dataset *data) {
    int *predictions = (int*)calloc(data->instanceCount, sizeof(int));
    for(int i=0; i<data->instanceCount; ++i) {
        predictions[i] = RandomForest_predict(rf, &data->instances[i]);
    }
    return predictions;
}

/**
 * @brief RandomForest_evaluate evaluates the accuracy of a random forest on a dataset
 * @param rf the random forest
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float RandomForest_evaluate(RandomForest *rf, Dataset *data) {
    int *predictions = RandomForest_predictAll(rf, data);
    int correct = 0;
    for(int i=0; i<data->instanceCount; ++i) {
        if(predictions[i] == data->instances[i].classID) correct++;
    }
    free(predictions);
    return (float)correct / (float)data->instanceCount;
}

/**
 * @brief RandomForest_nodeCount counts the number of nodes in a random forest
 * @param rf the random forest
 * @return the number of nodes
 */
int RandomForest_nodeCount(RandomForest *rf) {
    int nodeCount = 0;
    for(int i=0; i<rf->treeCount; ++i) {
        nodeCount += Decision_nodeCount(rf->trees[i]);
    }
    return nodeCount;
}

/**
 * @brief RandomForest_destroy destroys a random forest
 * @param rf the random forest
 */
void RandomForest_destroy(RandomForest *rf) {
    for(int i=0; i<rf->treeCount; ++i) {
        DecisionTree_destroy(rf->trees[i]);
    }
    free(rf->trees); free(rf);
}
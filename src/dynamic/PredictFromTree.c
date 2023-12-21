#include "PredictFromTree.h"

#include "../DecisionTreeTools.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

/**
 * @brief predict_from_tree_and_threshold predicts the class of an instance based on a threshold (PREDICT_FROM_TREE_MODE_THRESHOLD)
 * @param config the configuration for the predict_from_tree function
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
int predict_from_tree_and_threshold(const Config *config, const DecisionTreeNode *tree, const Instance *instance) {
    if(tree->class_id != -1) return tree->class_id;  // Leaf node

    if(instance->values[tree->split.feature_id] <= tree->split.threshold)
        return predict_from_tree_and_threshold(config, tree->left, instance);

    return predict_from_tree_and_threshold(config, tree->right, instance);
}


struct Scores {
    float *scores;  // [class_id]
};

/**
 * @brief sigmoid the sigmoid function, result between -1.0 and 1.0
 * @param x the input
 * @param lambda the lambda parameter
 * @param offset the offset parameter
 * @return the result of the sigmoid function between -1.0 and 1.0
 */
static float sigmoid(float x, float lambda, float offset) {
    return 1.0 / (1.0 + exp(-lambda * (x - offset)));
}

/**
 * @brief returns the number of classes in a tree
 * @param tree the tree
 * @return the number of classes -1
 */
static int count_classes_in_tree(const DecisionTreeNode *tree) {
    if(tree->class_id != -1) return tree->class_id;

    const int l_class_id = count_classes_in_tree(tree->left);
    const int r_class_id = count_classes_in_tree(tree->right);
    return l_class_id > r_class_id ? l_class_id : r_class_id;
}

/**
 * @brief fill the scores structure with the scores predicted by the tree
 * @param tree the tree
 * @param scores the scores structure
 * @param instance the instance
 * @param lambda the lambda parameter for the sigmoid function
 * @param score the current score
 */
static void fill_scores(const DecisionTreeNode *tree, struct Scores *scores, const Instance *instance, const float lambda, float score) {
    if(tree->class_id != -1) {
        scores->scores[tree->class_id] += score;
        return;
    }

    const float sig = sigmoid(instance->values[tree->split.feature_id], lambda, tree->split.threshold);
    const float l_score = score*(1.0-sig);
    const float r_score = score*sig;
    fill_scores(tree->left, scores, instance, lambda, l_score);
    fill_scores(tree->right, scores, instance, lambda, r_score);
}


/**
 * @brief predict_from_tree_and_sigmoid_scor predicts the class of an instance based on a sigmoid score (PREDICT_FROM_TREE_MODE_SIGMOID_SCORE)
 * @param config the configuration for the predict_from_tree function
 * - sigmoid_lambda the lambda parameter for the sigmoid function
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
int predict_from_tree_and_sigmoid_score(const Config *config, const DecisionTreeNode *tree, const Instance *instance) {
    const int class_count = count_classes_in_tree(tree)+1;

    struct Scores scores = {
        .scores = calloc(class_count, sizeof(float)),
    };

    fill_scores(tree, &scores, instance, config->sigmoid_lambda, 1.0);

    int max_class = -1;
    float max_score = -1.0;

    for(int i = 0; i < class_count; i++) {
        if(scores.scores[i] > max_score) {
            max_score = scores.scores[i];
            max_class = i;
        }
    }

    if(config->predictions) {
        Predictions *predictions = config->predictions;
        predictions->prediction_count = class_count;
        float total_score = 0.0; for(int i=0; i<class_count; ++i) total_score += scores.scores[i];
        predictions->predictions = (float*) calloc(class_count, sizeof(float));
        for(int i=0; i<class_count; ++i) predictions->predictions[i] = scores.scores[i] / total_score;
    }

    free(scores.scores);
    return max_class;
}
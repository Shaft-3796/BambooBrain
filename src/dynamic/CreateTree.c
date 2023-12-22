#include "CreateTree.h"

/**
 * @brief test_node_purity tests if a subproblem is pure enough
 * @param sp the subproblem
 * @param prunning_threshold the prunning threshold
 * @return the majority class id if the subproblem is pure enough, else -1
 */
static int test_node_purity(const Subproblem *sp, const float prunning_threshold) {
    return (float)sp->classes[sp->majority_class_id].instance_count / (float)sp->instance_count >= prunning_threshold ? sp->majority_class_id : -1;
}


static DecisionTreeNode* rec_create_decision_tree_from_prunning_threshold(const Config *config, const Subproblem *sp, const int current_depth) {
    DecisionTreeNode *node = (DecisionTreeNode*) calloc(1, sizeof(DecisionTreeNode));

    // Max depth reached
    if(current_depth >= config->max_tree_depth) { node->class_id = sp->majority_class_id; return node; }

    // Subproblem is pure enough
    node->class_id = test_node_purity(sp, config->prunning_threshold);  // Set the classID to -1 allow to easily check if the node is not a leaf
    if(node->class_id != -1) return node;

    // Compute the split
    node->split = config->compute_split(config, sp);

    // For instance bagging, unable to separate the subproblem
    if(node->split.feature_id == -1) { node->class_id = sp->majority_class_id; return node; }

    // Split the subproblem
    Subproblem *spl = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    Subproblem *spr = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    split_subproblem(sp, node->split.feature_id, node->split.threshold, spl, spr);

    // Create the children
    node->left = rec_create_decision_tree_from_prunning_threshold(config, spl, current_depth+1);
    node->right = rec_create_decision_tree_from_prunning_threshold(config, spr, current_depth+1);

    // Free the subproblems
    destroy_subproblem(spl);
    destroy_subproblem(spr);

    return node;
}

/**
 * @brief create_decision_tree_from_prunning_threshold creates a decision tree from a subproblem based on a prunning threshold (CREATE_TREE_MODE_PRUNNING_THRESHOLD)
 * @param config the configuration for the create_tree function
 * - compute_split_config: the configuration for the compute_split function
 * - max_depth: the maximum depth of the tree
 * - prunning_threshold: the prunning threshold
 * @param sp the subproblem
 * @return a pointer to the decision tree
 */
DecisionTreeNode* create_decision_tree_from_prunning_threshold(const Config *config, const Subproblem *sp) {
    return rec_create_decision_tree_from_prunning_threshold(config, sp, 0);
}

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

/**
 * @brief create_decision_tree_from_prunning_treshold creates a decision tree from a subproblem based on a prunning threshold (CREATE_TREE_MODE_PRUNNING_THRESHOLD)
 * @param config the configuration for the create_tree function
 * - compute_split_config: the configuration for the compute_split function
 * - max_depth: the maximum depth of the tree
 * - prunning_threshold: the prunning threshold
 * @param args mode specific arguments for the create_tree function
 * - current_depth: the current depth of the tree
 * @param sp the subproblem
 * @return a pointer to the decision tree
 */
DecisionTreeNode* create_decision_tree_from_prunning_treshold(
    const CreateTreeConfig *config,
    const CreateTreeArgs *args,
    const Subproblem *sp
    ) {
    DecisionTreeNode *node = (DecisionTreeNode*) calloc(1, sizeof(DecisionTreeNode));

    // Max depth reached
    if(args->current_depth >= config->max_depth) { node->class_id = sp->majority_class_id; return node; }

    // Subproblem is pure enough
    node->class_id = test_node_purity(sp, config->prunning_threshold);  // Set the classID to -1 allow to easily check if the node is not a leaf
    if(node->class_id != -1) return node;

    // Compute the split
    const ComputeSplitArgs compute_split_args = {};
    node->split = config->compute_split_config->compute_split_function(config->compute_split_config, &compute_split_args, sp);

    // Split the subproblem
    Subproblem *spl = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    Subproblem *spr = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    split_subproblem(sp, node->split.feature_id, node->split.threshold, spl, spr);

    // Create the children
    const CreateTreeArgs create_tree_args_l = {.current_depth=args->current_depth+1};
    node->left = create_decision_tree_from_prunning_treshold(config, &create_tree_args_l, spl);
    const CreateTreeArgs create_tree_args_r = {.current_depth=args->current_depth+1};
    node->right = create_decision_tree_from_prunning_treshold(config, &create_tree_args_r, spr);

    // Free the subproblems
    destroy_subproblem(spl);
    destroy_subproblem(spr);

    return node;
}

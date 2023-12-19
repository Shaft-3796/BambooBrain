#include "CreateTree.h"

/**
 * @brief test_node_purity tests if a subproblem is pure enough
 * @param sp the subproblem
 * @param prunning_threshold the prunning threshold
 * @return the majority class id if the subproblem is pure enough, else -1
 */
int test_node_purity(const Subproblem *sp, const float prunning_threshold) {
    return (float)sp->classes[sp->majority_class_id].instance_count / (float)sp->instance_count >= prunning_threshold ? sp->majority_class_id : -1;
}

/**
 * @brief create_decision_tree_from_prunning_treshold creates a decision tree from a subproblem based on a prunning threshold
 * @param sp the subproblem
 * @param compute_split_args the arguments for the compute_split function
 * @param current_depth the current depth of the tree
 * @param max_depth the maximum depth of the tree
 * @param prunning_threshold the prunning treshold
 * @return a pointer to the decision tree
 */
DecisionTreeNode* create_decision_tree_from_prunning_treshold(
    Subproblem *sp,
    ComputeSplitArgs *compute_split_args,
    const int current_depth,
    const int max_depth,
    const float prunning_threshold
    ) {
    DecisionTreeNode *node = (DecisionTreeNode*) calloc(1, sizeof(DecisionTreeNode));

    // Max depth reached
    if(current_depth >= max_depth) { node->class_id = sp->majority_class_id; return node; }

    // Subproblem is pure enough
    node->class_id = test_node_purity(sp, prunning_threshold);  // Set the classID to -1 allow to easily check if the node is not a leaf
    if(node->class_id != -1) return node;

    // Compute the split
    compute_split_args->sp = sp;
    node->split = compute_split(compute_split_args);

    // Split the subproblem
    Subproblem *spl = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    Subproblem *spr = create_subproblem(sp->instance_count, sp->feature_count, sp->class_count);
    split_subproblem(sp, node->split.feature_id, node->split.threshold, spl, spr);

    // Create the children
    node->left = create_decision_tree_from_prunning_treshold(spl, compute_split_args, current_depth+1, max_depth, prunning_threshold);
    node->right = create_decision_tree_from_prunning_treshold(spr, compute_split_args, current_depth+1, max_depth, prunning_threshold);

    // Free the subproblems
    destroy_subproblem(spl);
    destroy_subproblem(spr);

    return node;
}

/**
 * @brief create_tree creates a decision tree from a subproblem
 * @param args the arguments for the create_tree function
 * @return
 */
DecisionTreeNode *create_tree(CreateTreeArgs *args){
    switch(args->mode) {
        case CREATE_TREE_MODE_PRUNNING_THRESHOLD:
            return create_decision_tree_from_prunning_treshold(
                args->sp,
                args->compute_split_args,
                args->current_depth,
                args->max_depth,
                args->prunning_threshold);
        default:
            printf("Error: unknown split mode\n");
        return NULL;
    }
}
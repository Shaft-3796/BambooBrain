#pragma once

#include "../Dataset.h"
#include "../DecisionTreeTools.h"
#include "ComputeSplit.h"

/**
 * @brief CreateTreeMode the mode of the create_tree function
 * CREATE_TREE_MODE_PRUNNING_THRESHOLD: create a tree based on a prunning threshold
 */
typedef enum CreateTree {
    CREATE_TREE_MODE_PRUNNING_THRESHOLD,
} CreateTreeMode;

/**
 * @brief CreateTreeArgs the arguments for the create_tree function
 * Common:
 * @param subproblem the subproblem
 * Specific:
 * @param compute_split_args used for CREATE_TREE_MODE_PRUNNING_THRESHOLD to define the compute_split function
 * @param current_depth used for CREATE_TREE_MODE_PRUNNING_THRESHOLD, the current depth of the tree
 * @param max_depth used for CREATE_TREE_MODE_PRUNNING_THRESHOLD, the maximum depth of the tree
 * @param prunning_threshold used for CREATE_TREE_MODE_PRUNNING_THRESHOLD, the prunning threshold
 */
typedef struct CreateTreeArgs {
    CreateTreeMode mode;
    /* Common arguments */
    Subproblem *sp;
    /* Specific arguments */
    ComputeSplitArgs *compute_split_args;
    int current_depth;
    int max_depth;
    float prunning_threshold;
} CreateTreeArgs;

/**
 * @brief create_tree creates a decision tree from a subproblem
 * @param args the arguments for the create_tree function
 * @return
 */
DecisionTreeNode *create_tree(CreateTreeArgs *args);

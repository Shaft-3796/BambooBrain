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
 * @param current_depth the current depth of the tree
 * For CREATE_TREE_MODE_PRUNNING_THRESHOLD
 */
typedef struct CreateTreeArgs {
    int current_depth;
} CreateTreeArgs;

/**
 * @brief CreateTreeConfig the configuration for the CreateTree function
 * @param mode the mode of the create_tree function
 * @param create_tree_function the function to call to create the tree
 * @param compute_split_config the configuration for the compute_split function
 * For CREATE_TREE_MODE_PRUNNING_THRESHOLD
 * @param max_depth the maximum depth of the tree
 * For CREATE_TREE_MODE_PRUNNING_THRESHOLD
 * @param prunning_threshold the prunning threshold
 * For CREATE_TREE_MODE_PRUNNING_THRESHOLD
 */
typedef struct CreateTreeConfig {
    const CreateTreeMode mode;
    DecisionTreeNode* (*create_tree_function)(const struct CreateTreeConfig *config, const CreateTreeArgs *args, const Subproblem *sp);

    ComputeSplitConfig *compute_split_config;
    int max_depth;
    float prunning_threshold;
} CreateTreeConfig;


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
    );
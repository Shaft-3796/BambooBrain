#pragma once

#include "Config.h"
#include "Dataset.h"
#include "DecisionTreeTools.h"
#include "ComputeSplit.h"



/**
 * @brief create_decision_tree_from_prunning_threshold creates a decision tree from a subproblem based on a prunning threshold (CREATE_TREE_MODE_PRUNNING_THRESHOLD)
 * @param config the configuration for the create_tree function
 * - compute_split_config: the configuration for the compute_split function
 * - max_depth: the maximum depth of the tree
 * - prunning_threshold: the prunning threshold
 * @param sp the subproblem
 * @return a pointer to the decision tree
 */
DecisionTreeNode* create_decision_tree_from_prunning_threshold(const Config *config, const Subproblem *sp);
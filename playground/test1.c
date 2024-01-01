#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct sSplit{
    int feature_id;
    float threshold;
} Split;

typedef struct sDecisionTreeNode{
    struct sDecisionTreeNode* left;
    struct sDecisionTreeNode* right;
    Split split;
    int class_id;
} DecisionTreeNode;

typedef enum Model {
    MODEL_MODE_RANDOM_FOREST,
} ModelMode;

typedef struct sModel{
    ModelMode mode;
    /* Common arguments */
    int class_count;
    /* Specific arguments */
    DecisionTreeNode** trees;
    int tree_count;
} Model;


void write_split(FILE *storage, const Split *split){

}




void print_node(const DecisionTreeNode *node, int level){
    if(!node) {
        for(int i=0; i<level; ++i) bbprintf("\t");
        bbprintf("NULL\n");
    } else {
        // Class id
        for(int i=0; i<level; ++i) bbprintf("\t");
        bbprintf("%d\n", node->class_id);
        print_node(node->left, level+1);
        print_node(node->right, level+1);
    }
}



int main(){
    DecisionTreeNode d = {NULL, NULL, {4, 4}, 4};
    DecisionTreeNode c = {NULL, &d, {3, 3}, 3};
    DecisionTreeNode b = {NULL, NULL, {2, 2}, 2};
    DecisionTreeNode a = {&b, &c, {1, 1}, 1};

    // Extend again the tree
    DecisionTreeNode e = {NULL, NULL, {5, 5}, 5};
    DecisionTreeNode f = {NULL, NULL, {6, 6}, 6};
    DecisionTreeNode g = {&e, &f, {7, 7}, 7};
    DecisionTreeNode h = {NULL, NULL, {8, 8}, 8};
    DecisionTreeNode i = {&g, &h, {9, 9}, 9};
    DecisionTreeNode j = {NULL, NULL, {10, 10}, 10};
    DecisionTreeNode k = {&i, &j, {11, 11}, 11};
    // Attach to the existing tree
    a.left->left = &k;

    Model *model = (Model*) calloc(sizeof(Model), 1);
    model->mode = MODEL_MODE_RANDOM_FOREST;
    model->class_count = 12;
    model->tree_count = 10;
    model->trees = (DecisionTreeNode**) calloc(sizeof(DecisionTreeNode*), model->tree_count);
    for(int i=0; i<model->tree_count; i++) {
        model->trees[i] = &a;
    }

    // Print model
    bbprintf("Model:\n");
    bbprintf("Mode: %d\n", model->mode);
    bbprintf("Class count: %d\n", model->class_count);
    bbprintf("Tree count: %d\n", model->tree_count);
    for(int j=0; j<model->tree_count; j++) {
        bbprintf("Tree %d:\n", j);
        print_node(model->trees[j], 0);
    }

    FILE *storage = fopen("/home/x/Mount/XHome/Projects/Esiea/BambooBrain/playground/storage.bb", "wb");
    write_model(storage, model);
    fclose(storage);
    free(model);

    FILE *storage2 = fopen("/home/x/Mount/XHome/Projects/Esiea/BambooBrain/playground/storage.bb", "rb");
    model = read_model(storage2);
    fclose(storage2);

    // Print model
    bbprintf("Model 2:\n");
    bbprintf("Mode: %d\n", model->mode);
    bbprintf("Class count: %d\n", model->class_count);
    bbprintf("Tree count: %d\n", model->tree_count);
    for(int l=0; l<model->tree_count; l++) {
        bbprintf("Tree %d:\n", l);
        print_node(model->trees[l], 0);
    }

    return 0;
}

#include "Storage.h"

/**
 * @brief Recursively writes a decision tree not to a file in a binary format
 * @param storage the file to write to (must be opened in binary mode)
 * @param node the node to write
 */
void write_node(FILE *storage, const DecisionTreeNode *node){
    if(!node) {
        fwrite("\0", sizeof(char), 1, storage);
    } else {
        fwrite("\1", sizeof(char), 1, storage);
        write_node(storage, node->left);
        write_node(storage, node->right);
        fwrite(&node->split, sizeof(Split), 1, storage);
        fwrite(&node->class_id, sizeof(int), 1, storage);
    }
}

/**
 * @brief Recursively reads a decision tree from a file in a binary format
 * @param storage the file to read from (must be opened in binary mode)
 * @return the node, can be NULL
 */
DecisionTreeNode * read_node(FILE *storage){
    char flag; fread(&flag, sizeof(char), 1, storage);
    if(flag == '\0') {
        return NULL;
    }

    DecisionTreeNode *node = (DecisionTreeNode*) calloc(sizeof(DecisionTreeNode), 1);
    node->left = read_node(storage);
    node->right = read_node(storage);
    fread(&node->split, sizeof(Split), 1, storage);
    fread(&node->class_id, sizeof(int), 1, storage);
    return node;
}


/**
 * @brief Writes a model to a file in a binary format
 * @param storage the file to write to (must be opened in binary mode)
 * @param model the model to write
 */
void write_model(FILE *storage, const Model *model){
    fwrite(&model->mode, sizeof(ModelMode), 1, storage);
    fwrite(&model->class_count, sizeof(int), 1, storage);
    fwrite(&model->tree_count, sizeof(int), 1, storage);
    for(int i=0; i<model->tree_count; i++) {
        write_node(storage, model->trees[i]);
    }
}

/**
 * @brief Reads a model from a file in a binary format
 * @param storage the file to read from (must be opened in binary mode)
 * @return the model
 */
Model * read_model(FILE *storage) {
    Model *model = (Model*) calloc(sizeof(Model), 1);
    fread(&model->mode, sizeof(ModelMode), 1, storage);
    fread(&model->class_count, sizeof(int), 1, storage);
    fread(&model->tree_count, sizeof(int), 1, storage);
    model->trees = (DecisionTreeNode**) calloc(sizeof(DecisionTreeNode*), model->tree_count);
    for(int i=0; i<model->tree_count; i++) {
        model->trees[i] = read_node(storage);
    }
    return model;
}
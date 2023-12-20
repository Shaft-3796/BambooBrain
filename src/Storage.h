#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "Model.h"
#include "DecisionTree.h"


/**
 * @brief Writes a model to a file in a binary format
 * @param storage the file to write to (must be opened in binary mode)
 * @param model the model to write
 */
void write_model(FILE *storage, const Model *model);

/**
 * @brief Reads a model from a file in a binary format
 * @param storage the file to read from (must be opened in binary mode)
 * @return the model
 */
Model * read_model(FILE *storage);
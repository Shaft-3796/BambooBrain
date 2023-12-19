//
// Created by loe on 18/12/23.
//

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#endif //USERINTERFACE_H

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define TEXTURE_WIDTH 28
#define TEXTURE_HEIGHT 28

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <math.h>
#include "stdbool.h"
#include "Dataset.h"
#include "DecisionTree.h"

#include "dynamic/PredictFromTree.h"
#include "DecisionTreeTools.h"

int init_sdl();

int create_ui(DecisionTreeNode *tree, PredictFromTreeArgs *args);

void save_texture(const char* filename, SDL_Texture* texture, SDL_Window *window, DecisionTreeNode *tree, PredictFromTreeArgs *args);
void draw_pixel(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, bool add);
void draw_circle(SDL_Texture *texture, int x, int y, float radius);
void reset_drawing(SDL_Texture *texture);

void predict_drawing(DecisionTreeNode *tree, PredictFromTreeArgs *args);
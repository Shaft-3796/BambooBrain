#pragma once


#define WINDOW_WIDTH 1920/2
#define WINDOW_HEIGHT 1080/2
#define TEXTURE_WIDTH 28
#define TEXTURE_HEIGHT 28

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <math.h>
#include "stdbool.h"
#include "Dataset.h"
#include "DecisionTree.h"

#include "dynamic/PredictFromTree.h"
#include "Model.h"
#include "Predictions.h"
#include "ModelTools.h"
#include "DecisionTreeTools.h"

/**
 * @brief Initialize "SDL", "SDL image" and "SDL TTF" libraries
 * @return Exit code
 */
int init_sdl();

/**
 * @brief Create a window to allow user interact with
 * @config the config
 * @config the model
 * @return Exit code
 */
int create_ui(Config *config, Model *model);

/**
 * @brief Load a dataset and display it on the texture
 * @param instance the instance to load
 * @param texture the SDL image to import
 * @param window the SDL window
 * @return void
 */
void load_texture(Instance *instance, SDL_Texture* texture, SDL_Window *window);

/**
 * @brief Save a texture into a dataset of one instance
 * @param config the config
 * @param instance the instance of the output texture
 * @param texture the SDL image to export
 * @param window the SDL window
 * @return instance
 */
Instance *save_texture(Config *config, Instance *instance, SDL_Texture* texture, SDL_Window *window, Model *model);

/**
 * @brief Draw a pixel on a texture given colour and coordinates
 * @param texture The texture to draw on
 * @param r the red component
 * @param g the greend component
 * @param b the blue component
 * @param a the alpha component
 * @param x the x coordinate of the pixel
 * @param y the y coordinate of the pixel
 * @param overwrite true whether the new pixel be overwritten or added
 */
void draw_pixel(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, bool overwrite);

/**
 * @brief Draw a plein circle on a given position colorized gradiently from white to black
 * @param texture The SDL texture to draw on
 * @param x The x coordinate
 * @param y The y coordinate
 * @param radius The radius of the circle
 */
void draw_circle(SDL_Texture *texture, int x, int y, float radius, bool subtract);

/**
 * @brief Entirely colorize texture to black
 * @param texture The SDL texture to draw on
 */
void reset_drawing(SDL_Texture *texture);

/**
 * @brief Center the texture to make it fitting more with dataset
 * @param pixels the list of pixels
 * @param format the SDL format
 * @return int the horizontal correction factor (between -TEXTURE_WIDTH and TEXTURE_WIDTH)
 */
int center_texture(const Uint32 *pixels, const SDL_PixelFormat *format);

/**
 * @brief Update each statictic text with its prediction percentage
 * @param predictions The predictions
 * @param renderer The SDL renderer
 * @param font The text font
 * @param surface The text surface
 * @param texture The text texture
 */
void update_statistic_text(const Predictions *predictions, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface** surface, SDL_Texture** texture);

/**
 * @brief Update main prediction test
 * @param predictions The predictions
 * @param renderer The SDL renderer
 * @param font the text font
 * @param surface The text surface
 * @param texture The text texture
 */
void update_prediction_text(const Predictions *predictions, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface** surface, SDL_Texture** texture);

/**
 * @brief Render each statistic text
 * @param prediction_count The number of predictions
 * @param window The SDL window
 * @param renderer The SDL renderer
 * @param statistics_font The text font
 * @param statistic_textures The text surface
 */
void render_statistic_text(int prediction_count, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* statistics_font, SDL_Texture** statistic_textures);

/**
 * @brief Create the SDL window
 * @param window The SDL window
 * @param renderer The SDL renderer
 * @param texture The SDL main texture
 */
void create_SDL_window(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture);
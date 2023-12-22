#include "UserInterface.h"

#include "PreProcessing.h"


int max(int a, int b) {
    return a > b ? a : b;
}
int min(int a, int b) {
    return a < b ? a : b;
}

// Some useful colors
SDL_Color red = {255, 0, 0, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color white = {0, 0, 255, 255};
SDL_Color grey = {100, 100, 100, 255};

/**
 * @brief Initialize "SDL", "SDL image" and "SDL TTF" libraries
 * @return Exit code
 */
int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error while initializing SDL library - %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialize SDL2_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
        fprintf(stderr, "SDL_image initialization failed: %s\n", IMG_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        printf("Error while initializing SDL TTF library - %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

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
void draw_pixel(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, bool overwrite) {
    void* tmp;
    Uint32 *pixels;
    int pitch;

    SDL_PixelFormat *format = NULL;
    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_LockTexture(texture, NULL, &tmp, &pitch);
    pixels = tmp;

    // Check if the pixel isn't outside the texture
    if (x < 0 || x >= TEXTURE_WIDTH || y < 0|| y >= TEXTURE_HEIGHT) return;

    if (overwrite) {
        // Replace pixel
        pixels[y * TEXTURE_WIDTH + x] = SDL_MapRGBA(format,  r,  g,  b,  a);
    }
    else {
        // Get the old colour of the pixel
        Uint8 oldR, oldG, oldB, oldA;
        SDL_GetRGBA(pixels[y * TEXTURE_WIDTH + x], format, &oldR, &oldG, &oldB, &oldA);

        // Replace pixel if it is ligther than the old one
        pixels[y * TEXTURE_WIDTH + x] = SDL_MapRGBA(format,  max(r, oldR),  max(g, oldG),  max(b, oldB),  max(a, oldA));
    }



    SDL_FreeFormat(format);
    SDL_UnlockTexture(texture);
}

/**
 * @brief Save a texture into a dataset of one instance
 * @param config the config
 * @param filename the filename of the output file
 * @param texture the SDL image to export
 * @param window the SDL window
 * @return the instance
 */
Instance *save_texture(Config *config, const char* filename, SDL_Texture* texture, SDL_Window *window, Model *model) {
    void* tmp;
    Uint32 *pixels;
    int pitch;
    Uint8 r, g, b, a;

    SDL_PixelFormat *format = NULL;
    format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));
    SDL_LockTexture(texture, NULL, &tmp, &pitch);
    pixels = tmp;

    const int correction = center_texture(pixels, format);

    Instance *instance = (Instance*) calloc(1, sizeof(Instance));
    instance->values = (int*) calloc(TEXTURE_WIDTH*TEXTURE_HEIGHT, sizeof(int));
    int idx = 0;

    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j, ++idx) {
            // Add the horizontal correction
            const int newj = max(0, min(TEXTURE_HEIGHT, j-correction));

            Uint32 pixel = pixels[i * TEXTURE_WIDTH + newj];
            SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
            instance->values[idx] = r ? 255 : 0;
        }
    }

    SDL_UnlockTexture(texture);
    return instance;
}

/**
 * @brief Center the texture to make it fitting more with dataset
 * @param pixels the list of pixels
 * @param format the SDL format
 * @return int the horizontal correction factor (between -TEXTURE_WIDTH and TEXTURE_WIDTH)
 */
int center_texture(const Uint32 *pixels, const SDL_PixelFormat *format) {
    Uint8 r, g, b, a;
    int borderL, borderR;
    bool found = false;

    // Get the left border
    for (int i = 0; i < TEXTURE_HEIGHT && !found; ++i) {
        for (int j = 0; j < TEXTURE_WIDTH && !found; ++j) {
            Uint32 pixel = pixels[j * TEXTURE_WIDTH + i];
            SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
            if (r) {
                borderL = i;
                found = true;
            }
        }
    }

    found = false;

    // Get the right border
    for (int i = TEXTURE_HEIGHT; i > 0 && !found; --i) {
        for (int j = 0; j < TEXTURE_WIDTH && !found; ++j) {
            Uint32 pixel = pixels[j * TEXTURE_WIDTH + i];
            SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
            if (r) {
                borderR = i;
                found = true;
            }
        }
    }


    // Calculate the correction factor
    float texture_mid = TEXTURE_WIDTH/2;
    float drawing_mid = (borderL + borderR)/2;
    int correction = texture_mid - drawing_mid;

    return correction;
}


/**
 * @brief Load a dataset and display a random instance on the texture
 * @param filename the filename of the output file
 * @param texture the SDL image to export
 * @param window the SDL window
 * @return void
 */
void load_texture(const char* filename, SDL_Texture* texture, SDL_Window *window) {
    Dataset *trainData = parse_dataset_from_file(filename);
    Uint8 intensity;
    int r = rand() % trainData->instance_count;
    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j) {
            intensity = trainData->instances[r].values[j * TEXTURE_WIDTH + i];
            draw_pixel(texture, intensity, intensity, intensity, 255, i, j, true);
        }
    }
    return;
}


/**
 * @brief Draw a plein circle on a given position colorized gradiently from white to black
 * @param texture The SDL texture to draw on
 * @param x The x coordinate
 * @param y The y coordinate
 * @param radius The radius of the circle
 */
void draw_circle(SDL_Texture *texture, int x, int y, float radius, bool subtract) {
    // Loop in each pixels in the radius
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            // Get the distance with Pythagore theorem
            float distance = sqrt(dx * dx + dy * dy);
            // Make sure the pixel is inside the circle
            if (distance <= radius) {
                // Get colour intensity depending on the distance
                if (subtract) {
                    draw_pixel(texture, 0, 0, 0, 255, x + dx, y + dy, true);
                }
                else {
                    Uint8 intensity = (Uint8)(distance / radius * 255);
                    intensity = 255 - intensity;
                    draw_pixel(texture, intensity, intensity, intensity, 255, x + dx, y + dy, false);
                }
            }
        }
    }
}

/**
 * @brief Entirely colorize texture to black
 * @param texture The SDL texture to draw on
 */
void reset_drawing(SDL_Texture *texture) {
    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j) {
            draw_pixel(texture, 0, 0, 0, 255, i, j, true);
        }
    }
}

/**
 * @brief Update each statictic text with its prediction percentage
 * @param predictions The predictions
 * @param renderer The SDL renderer
 * @param font The text font
 * @param surface The text surface
 * @param texture The text texture
 */
void update_statistic_text(const Predictions *predictions, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface** surface, SDL_Texture** texture) {
    int indexes[predictions->prediction_count];
    // Sort predictions->predictions[i]*100 in descending order
    for (int i = 0; i < predictions->prediction_count; ++i) {
        indexes[i] = i;
    }
    for (int i = 0; i < predictions->prediction_count; ++i) {
        for (int j = i+1; j < predictions->prediction_count; ++j) {
            if (predictions->predictions[indexes[i]] < predictions->predictions[indexes[j]]) {
                int tmp = indexes[i];
                indexes[i] = indexes[j];
                indexes[j] = tmp;
            }
        }
    }


    for (int i = 0; i < predictions->prediction_count; ++i) {
        char text[128] = "";
        sprintf(text, "%d: %.2f%%", indexes[i], predictions->predictions[indexes[i]]*100);
        *surface = TTF_RenderText_Solid(font, text, grey);
        texture[i] = SDL_CreateTextureFromSurface(renderer, *surface);
    }
}

/**
 * @brief Update main prediction test
 * @param predictions The predictions
 * @param renderer The SDL renderer
 * @param font the text font
 * @param surface The text surface
 * @param texture The text texture
 */
void update_prediction_text(const Predictions *predictions, SDL_Renderer* renderer, TTF_Font* font, SDL_Surface** surface, SDL_Texture** texture) {
    int prediction = predictions->main_prediction;
    char text[12];
    if (prediction >= 0) {
        text[0] = prediction + '0';
        *surface = TTF_RenderText_Solid(font, text, red);
        *texture = SDL_CreateTextureFromSurface(renderer, *surface);
    }
}

/**
 * @brief Render each statistic text
 * @param prediction_count The number of predictions
 * @param window The SDL window
 * @param renderer The SDL renderer
 * @param statistics_font The text font
 * @param statistic_textures The text surface
 */
void render_statistic_text(const int prediction_count, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* statistics_font, SDL_Texture** statistic_textures) {
    int w, h;
    TTF_SizeText(statistics_font, "0: 00.00%", &w, &h);

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    for (int i = 0; i < prediction_count; ++i) {
        int y = (winH/2) + (i-prediction_count/2) * h; // Get y position of the text
        SDL_Rect statistics_rect = {10, y, w, h}; // Make it responsive
        SDL_RenderCopy(renderer, statistic_textures[i], NULL, &statistics_rect);
    }
}

/**
 * @brief Create the SDL window
 * @param window The SDL window
 * @param renderer The SDL renderer
 * @param texture The SDL main texture
 */
void create_SDL_window(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture) {
    if (init_sdl() != EXIT_SUCCESS) {
        exit(EXIT_FAILURE);
    }

    Uint32 flags = SDL_WINDOW_RESIZABLE;
    *window = SDL_CreateWindow("BambooBrain", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

    // Create the window
    if (!*window) {
        fprintf(stderr, "Error while creating window - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create the renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == *renderer)
    {
        fprintf(stderr,"Erreur while creating renderer %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create the texture
    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (*texture == NULL) {
        fprintf(stderr,"Erreur while creating texture - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return;
}

/**
 * @brief Create a window to allow user interact with
 * @config the config
 * @config the model
 * @return Exit code
 */
int create_ui(Config *config, Model *model) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    int w, h;

    create_SDL_window(&window, &renderer, &texture);


    // Import fonts
    TTF_Font *prediction_font = TTF_OpenFont("../fonts/fast99.ttf", 80);
    TTF_Font *statistics_font = TTF_OpenFont("../fonts/fast99.ttf", 45);

    if (prediction_font == NULL || statistics_font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    // Create prediction text
    char text[12] = "0";
    SDL_Surface *text_surface = TTF_RenderText_Solid(prediction_font, text, red);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    TTF_SizeText(prediction_font, "0", &w, &h);
    SDL_Rect text_rect = {10, 10, w, h};


    // Create statistic texts
    SDL_Surface *statistic_surfaces = TTF_RenderText_Solid(statistics_font, "0: 00.00%", grey);
    SDL_Texture **statistic_textures = (SDL_Texture **) calloc(model->class_count, sizeof(SDL_Texture*));
    for (int i = 0; i < model->class_count; ++i) {
        statistic_textures[i] = SDL_CreateTextureFromSurface(renderer, statistic_surfaces);
    }
    TTF_SizeText(prediction_font, "0: 00.00%", &w, &h);


    // Save current tick
    Uint32 lastTick = SDL_GetTicks();

    Instance *instance = NULL;
    Predictions *predictions = NULL;

    while (1) {
        SDL_Event evt;
        SDL_Scancode scanCode;

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
                case SDL_QUIT:
                    goto Quit;
                    break;

                case SDL_KEYDOWN:
                    scanCode = evt.key.keysym.scancode;
                    if (evt.key.repeat)
                        break;
                    switch (scanCode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            goto Quit;
                        break;
                        case SDL_SCANCODE_SPACE:
                            reset_drawing(texture);
                            break;
                        default:
                            break;
                    }
                    break;


                // Handle mouse movements
                case SDL_MOUSEMOTION :
                    // If click is pressed draw pixels
                    if (evt.button.button == SDL_BUTTON_RIGHT || evt.button.button == SDL_BUTTON_X1 ||
                        evt.button.button == SDL_BUTTON_LEFT) {
                        int w, h;
                        SDL_GetWindowSize(window, &w, &h);

                        int x = evt.motion.x*TEXTURE_WIDTH/w;
                        int y = evt.motion.y*TEXTURE_HEIGHT/h;

                        // Draw circle in additive or substractive mode depending on which mouse button is clicked
                        draw_circle(texture, x, y, config->pencil_radius, evt.button.button != SDL_BUTTON_LEFT);

                        // Stop there if we didn't reach the trickrate
                        if (lastTick + config->tickrate > SDL_GetTicks()) break;

                        // Update last tick
                        lastTick = SDL_GetTicks();

                        // Save texture and calculate prediction
                        instance = save_texture(config, "../datasets/test.txt", texture, window, model);
                        apply_pp_steps_to_instance(config, instance, TEXTURE_WIDTH*TEXTURE_HEIGHT);
                        predictions = config->predict_from_model(config, model, instance);

                        // Update texts on screen
                        update_prediction_text(predictions, renderer, prediction_font, &text_surface, &text_texture);
                        update_statistic_text(predictions, renderer, statistics_font, &statistic_surfaces, statistic_textures);


                    }

                default:
                    break;
            }
        }

        // Render each texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        render_statistic_text(model->class_count, window, renderer, statistics_font, statistic_textures);


        if(instance) {
            free(instance->values);
            free(instance);
            destroy_predictions(predictions);
            instance = NULL;
            predictions = NULL;
        }

        if (renderer) SDL_RenderPresent(renderer);
    }


    Quit:
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
    if(window != NULL)
            SDL_DestroyWindow(window);
    if (text_surface != NULL)
        SDL_FreeSurface(text_surface);
    if (text_texture != NULL)
        SDL_DestroyTexture(text_texture);

    SDL_Quit();
    IMG_Quit();

    free(statistic_textures);
    return EXIT_SUCCESS;
}


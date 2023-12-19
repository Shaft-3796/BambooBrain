//
// Created by loe on 18/12/23.
//

#include "UserInterface.h"


int max(int a, int b) {
    return a > b ? a : b;
}

/**
 * \brief Initialize "SDL" and "SDL image" libraries
 * \return Exit code
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

    return EXIT_SUCCESS;
}

/**
 * \brief Draw a pixel on a texture given colour and coordinates
 * \param texture The texture to draw on
 * \param r the red component
 * \param g the greend component
 * \param b the blue component
 * \param a the alpha component
 * \param x the x coordinate of the pixel
 * \param y the y coordinate of the pixel
 * \param add true if the new pixel should be added only if it's lighter than the old one, false if it should be just replaced
 */
void draw_pixel(SDL_Texture *texture, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int x, int y, bool add) {
    void* tmp;
    Uint32 *pixels;
    int pitch;

    SDL_PixelFormat *format = NULL;
    format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_LockTexture(texture, NULL, &tmp, &pitch);
    pixels = tmp;

    // Check if the pixel isn't outside the texture
    if (x < 0 || x > TEXTURE_WIDTH || y < 0|| y > TEXTURE_HEIGHT) return;

    if (add) {
        // Get the old colour of the pixel
        Uint8 oldR, oldG, oldB, oldA;
        SDL_GetRGBA(pixels[y * TEXTURE_WIDTH + x], format, &oldR, &oldG, &oldB, &oldA);

        // Replace pixel if it is ligther than the old one
        pixels[y * TEXTURE_WIDTH + x] = SDL_MapRGBA(format,  max(r, oldR),  max(g, oldG),  max(b, oldB),  max(a, oldA));
    }
    else {
        // Replace pixel
        pixels[y * TEXTURE_WIDTH + x] = SDL_MapRGBA(format,  r,  g,  b,  a);
    }



    SDL_FreeFormat(format);
    SDL_UnlockTexture(texture);
}

/**
 * \brief load saved drawing from file and predict its class from a given tree.
 * \param tree the tree
 * \param args the arguments for the predict_from_tree function
 */
void predict_drawing(DecisionTreeNode *tree, PredictFromTreeArgs *args) {
    char tmp_path[128] = "../datasets/test.txt";
    Dataset *tmpData = parse_dataset_from_file(tmp_path);

    Subproblem *sp_tmp = create_subproblem_from_dataset(tmpData);

    printf("------ TMP data ------\n");
    print_subproblem(sp_tmp);
    int *pred = predict_all_from_tree(args, tree, tmpData);

    for (int i = 0; i < sp_tmp->instance_count; ++i) {
        printf("%d: %d\n", i, pred[i]);
    }

    destroy_subproblem(sp_tmp);
}

/**
 * \brief Save a texture into a dataset of one instance
 * \param filename the filename of the output file
 * \param texture the SDL image to export
 * \param window the SDL window
 * \return void
 */
void save_texture(const char* filename, SDL_Texture* texture, SDL_Window *window, DecisionTreeNode *tree, PredictFromTreeArgs *args) {
    void* tmp;
    Uint32 *pixels;
    int pitch;
    Uint8 r, g, b, a;

    SDL_PixelFormat *format = NULL;
    format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));
    SDL_LockTexture(texture, NULL, &tmp, &pitch);
    pixels = tmp;


    FILE *out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "Erreur while opening/creating %s\n", filename);
        SDL_UnlockTexture(texture);
        return;
    }

    // Write the first line giving size of the dataset
    fprintf(out, "1 1 %d\n0	", TEXTURE_WIDTH*TEXTURE_HEIGHT);
    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j) {
            Uint32 pixel = pixels[i * TEXTURE_WIDTH + j];
            SDL_GetRGBA(pixel, format, &r, &g, &b, &a);
            fprintf(out, "%d ", r ? 255 : 0); // Write each feature
        }
    }

    fclose(out);
    SDL_UnlockTexture(texture);
    printf("File created with success!\n");
    predict_drawing(tree, args);
    return;
}


/**
 * \brief Load a dataset and display it on the texture
 * \param filename the filename of the output file
 * \param texture the SDL image to export
 * \param window the SDL window
 * \return void
 */
void load_texture(const char* filename, SDL_Texture* texture, SDL_Window *window) {
    Dataset *trainData = parse_dataset_from_file(filename);
    Uint8 intensity;
    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j) {
            intensity = trainData->instances[0].values[j * TEXTURE_WIDTH + i];
            draw_pixel(texture, intensity, intensity, intensity, 255, i, j, false);
        }
    }
    return;
}


/**
 * \brief Draw a plein circle on a given position colorized gradiently from white to black
 * \param texture The SDL texture to draw on
 * \param x The x coordinate
 * \param y The y coordinate
 * \param radius The radius of the circle
 */
void draw_circle(SDL_Texture *texture, int x, int y, float radius) {
    // Loop in each pixels in the radius
    for (int dx = -radius; dx <= radius; dx++) {
        for (int dy = -radius; dy <= radius; dy++) {
            // Get the distance with Pythagore theorem
            float distance = sqrt(dx * dx + dy * dy);
            // Make sure the pixel is inside the circle
            if (distance <= radius) {
                // Get colour intensity depending on the distance
                Uint8 intensity = 255 - (Uint8)(distance / radius * 255);
                draw_pixel(texture, intensity, intensity, intensity, 255, x + dx, y + dy, true);
            }
        }
    }
}

/**
 * \brief Entirely colorize texture to black
 * \param texture The SDL texture to draw on
 */
void reset_drawing(SDL_Texture *texture) {
    for (int i = 0; i < TEXTURE_WIDTH; ++i) {
        for (int j = 0; j < TEXTURE_HEIGHT; ++j) {
            draw_pixel(texture, 0, 0, 0, 255, i, j, false);
        }
    }
}


/**
 * @brief Create a window to allow user interact with
 * @return Exit code
 */
int create_ui(DecisionTreeNode *tree, PredictFromTreeArgs *args) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

    if (init_sdl() != EXIT_SUCCESS) return EXIT_FAILURE;

    Uint32 flags = SDL_WINDOW_RESIZABLE;
    window = SDL_CreateWindow("BambooBrain", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, flags);

    // Create the window
    if (!window) {
        printf("Error while creating window - %s\n", SDL_GetError());
        goto Quit;
    }

    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(NULL == renderer)
    {
        printf("Erreur while creating renderer %s\n", SDL_GetError());
        goto Quit;
    }

    // Create the texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    if (texture == NULL) {
        printf("Erreur while creating texture - %s\n", SDL_GetError());
    }

    int quit = 0;
    while (!quit) {
        SDL_Event evt;
        SDL_Scancode scanCode;

        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_KEYDOWN:
                    scanCode = evt.key.keysym.scancode;
                    if (evt.key.repeat)
                        break;
                    switch (scanCode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                            quit = 1;
                        break;
                        case SDL_SCANCODE_SPACE:
                            reset_drawing(texture);
                            break;
                        case SDL_SCANCODE_KP_ENTER:
                            save_texture("../datasets/test.txt", texture, window, tree, args);
                            break;
                        case SDL_SCANCODE_T:
                            load_texture("../datasets/test.txt", texture, window);
                        default:
                            break;
                    }
                    break;


                // Handle mouse movements
                case SDL_MOUSEMOTION :
                    // If left click is pressed draw pixels
                    if (evt.button.button == SDL_BUTTON_LEFT) {
                        int w, h;
                        SDL_GetWindowSize(window, &w, &h);

                        int x = evt.motion.x*TEXTURE_WIDTH/w;
                        int y = evt.motion.y*TEXTURE_HEIGHT/h;

                        draw_circle(texture, x, y, 1.2);
                    }

                default:
                    break;
            }
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }


    return EXIT_SUCCESS;

    Quit:
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
    if(window != NULL)
            SDL_DestroyWindow(window);
    SDL_Quit();
    IMG_Quit();
    return EXIT_FAILURE;
}


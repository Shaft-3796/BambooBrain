#include "Progress.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Initialize the progress
 * @param progress The progress to initialize
 * @param max The maximum value
 * @param current The current value
 * @param task The task
 */
void init_progress(Progress *progress, const int max, const int current, char *task) {
    progress->max = max;
    progress->current = current;
    progress->task = task;
    progress->running = true;
    progress->last_clock = 0;
    progress->last_current = current;
}

/**
 * @brief Update the progress
 * @param progress The progress to update
 * @param current The current value
 */
void update_progress(Progress *progress, const int current) {
    assert(progress->running);

    progress->current = current;
    clock_t clock_now = clock();
    double time_elapsed = (double) (clock_now - progress->last_clock) / CLOCKS_PER_SEC;
    int current_diff = progress->current - progress->last_current;
    double eta = current_diff * time_elapsed ? (double) (progress->max - progress->current) / current_diff * time_elapsed : 0;

    char str_eta[128];
    // If eta is nan
    if(progress->last_clock == 0) {
        snprintf(str_eta, 128, "???");
    } else if (eta < 60) {
        snprintf(str_eta, 128, "%ds", (int) eta);
    } else if (eta < 3600) {
        snprintf(str_eta, 128, "%dm %ds", (int) eta / 60, (int) eta % 60);
    } else {
        snprintf(str_eta, 128, "%dh %dm %ds", (int) eta / 3600, (int) eta % 3600 / 60, (int) eta % 60);
    }

    progress->last_clock = clock_now;
    progress->last_current = progress->current;

    bbprintf("\r\033[0;35m\033[1m[%s]\033[0m\033[0;33m %d/%d ≈ %s\033[0m\033[0;32m〘", progress->task, progress->current, progress->max, str_eta);

    int fill_num = progress->current * 100 / progress->max;
    int space_num = 100 - fill_num;

    for (int i = 0; i < fill_num; i++) {
        bbprintf("▉");
    }
    for (int i = 0; i < space_num; i++) {
        bbprintf("░");
    }

    bbprintf("〙%.f%%\033[0m", (float) progress->current / (float) progress->max * 100.0f);

    fflush(stdout);
}


/**
 * @brief Finish the progress
 * @param progress The progress to finish
 */
void finalize_progress(Progress *progress) {
    assert(progress->running);

    progress->current = progress->max;

    update_progress(progress, progress->current);

    progress->running = false;

    bbprintf("\n");
}

/**
 * @brief Print the bamboubrain logo
 */
/*
██████╗  █████╗ ███╗   ███╗██████╗  ██████╗  ██████╗     ██████╗ ██████╗  █████╗ ██╗███╗   ██╗
██╔══██╗██╔══██╗████╗ ████║██╔══██╗██╔═══██╗██╔═══██╗    ██╔══██╗██╔══██╗██╔══██╗██║████╗  ██║
██████╔╝███████║██╔████╔██║██████╔╝██║   ██║██║   ██║    ██████╔╝██████╔╝███████║██║██╔██╗ ██║
██╔══██╗██╔══██║██║╚██╔╝██║██╔══██╗██║   ██║██║   ██║    ██╔══██╗██╔══██╗██╔══██║██║██║╚██╗██║
██████╔╝██║  ██║██║ ╚═╝ ██║██████╔╝╚██████╔╝╚██████╔╝    ██████╔╝██║  ██║██║  ██║██║██║ ╚████║
╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═════╝  ╚═════╝  ╚═════╝     ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝
*/
void print_logo(){
    bbprintf("\033[0;32m\033[1m");
    char **logo = (char**) calloc(6, sizeof(char*));
    logo[0] = "██████╗  █████╗ ███╗   ███╗██████╗  ██████╗  ██████╗     ██████╗ ██████╗  █████╗ ██╗███╗   ██╗";
    logo[1] = "██╔══██╗██╔══██╗████╗ ████║██╔══██╗██╔═══██╗██╔═══██╗    ██╔══██╗██╔══██╗██╔══██╗██║████╗  ██║";
    logo[2] = "██████╔╝███████║██╔████╔██║██████╔╝██║   ██║██║   ██║    ██████╔╝██████╔╝███████║██║██╔██╗ ██║";
    logo[3] = "██╔══██╗██╔══██║██║╚██╔╝██║██╔══██╗██║   ██║██║   ██║    ██╔══██╗██╔══██╗██╔══██║██║██║╚██╗██║";
    logo[4] = "██████╔╝██║  ██║██║ ╚═╝ ██║██████╔╝╚██████╔╝╚██████╔╝    ██████╔╝██║  ██║██║  ██║██║██║ ╚████║";
    logo[5] = "╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═════╝  ╚═════╝  ╚═════╝     ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝";

    // Transition line by line
    clock_t clock_now = clock();
    for(int i=0; i<6; ++i){
        // Wait 0.1s
        while(clock() - clock_now < CLOCKS_PER_SEC/4);
        clock_now = clock();

        bbprintf("%s\n", logo[i]);
        fflush(stdout);
    }

    bbprintf("\033[0m");
    free(logo);
}
#include "Dynamic.h"

typedef struct GenericArgs{
    int mode;
} GenericArgs;


/**
 * @brief Checks if the given modular function mode is compatible with the given compatibilities
 * @param args the argument structure
 * @param compatibilities the compatibilities
 * @param count the number of compatibilities
 * @return true if the mode is compatible, false otherwise
 */
bool check_mode_compatibility(void* args, const int* compatibilities, const int count){
    GenericArgs* gargs = (GenericArgs*)args;
    int mode = gargs->mode;

    for(int i = 0; i < count; ++i){
        if(compatibilities[i] == mode){
            return true;
        }
    }

    return false;
}
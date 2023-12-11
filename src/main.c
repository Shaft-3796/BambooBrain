#include "SDL.h"
#include "stdio.h"

int main(){
    FILE* file = fopen("FASION_test.txt", "r");
    if(file == NULL){
        printf("Error: File not found\n");
        return 1;
    }
    return 1;
}
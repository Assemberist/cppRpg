#include "parser_lib.h"

int main(int argc, char** argv){
    FILE* file = fopen("effect_calculator.cpp", "w");

    if(!file){
        perror("can't create main file");
        return -1;
    }

    while(--argc)
        if(!add_effect(file, argv[argc]))
            return -1;

    fclose(file);
}
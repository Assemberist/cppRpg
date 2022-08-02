#include "parser_lib.h"
#include "standard_lexems.h"

int add_effect(FILE* output, char* filename){
    token* effects = new_token();
    token* propertyes = new_token();

    FILE* file = fopen(filename, "r");
    if(!file){
        perror("Can't open .eff file");
        return NULL;
    }

    FILE* tmp = fopen(TMP_FILE, "rw");
    if(!tmp){
        perror("Can't create tmp file");
        fclose(file);
        return NULL;
    }

    int rezult = 1;
    if(!parse_file(file, tmp, effects, propertyes)){
        char buffer[256];
        sprintf(buffer, "Can't parse file %s", filename);
        perror(buffer);

        rezult = NULL;
        goto final;
    }

    add_effect_ptrs(output, effects);
    add_property_ptrs(output, propertyes);

    rewind(tmp);
    char buffer[100];
    while(fgets(buffer, 100, tmp))
        fputs(buffer, output);

final:
    fclose(tmp);
    remove(TMP_FILE);

    fclose(file);
    return file;
}

int parse_file(FILE* input, FILE* output, token* effects, token* propertyes){
    size_t string_num = 0;
    size_t level = 0;
    char buffer[1024];

    while(fgets(buffer, 1024, input)){
        string_num++;

        size_t current_level = check_level(buffer);

        if(current_level > level + 1){
            perror("Next level large than current+1");
            return NULL;
        }

        if(current_level == 0 && level == 0)
            new_element(buffer);

        char* src = buffer;
        while(*src == ' ' || *src == '\t') src++;

        l
    }
}

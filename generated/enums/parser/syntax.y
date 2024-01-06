%{
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void yyerror(const char *str){ fprintf(stderr,"ошибка: %s\n",str);}

int yywrap(){ return 1; }

char* name;
char prefix[32];
char values[2048][128];
char (*current_value)[128] = values;

FILE* header;
FILE* source;

void create_enum(){
    fprintf(header, "typedef enum %s_t {\n", name);
    size_t i = 0;
    while(values[i+1] < current_value){
        fprintf(header, "\t%s,\n", values[i]);
        i++;
    }
    fprintf(header, "\t%s\n} %s_t;\n\n", values[i], name);
}

char* replace_(char* src){
    char* ptr = src;
    while(*src){
        if(*src == '_')
            *(src) = ' ';
        src++;
    }
    return ptr;
}

void create_output_fun(){
    fprintf(source, "#include \"%s.hpp\"\n\n", name);
    fprintf(source, "const char* get_enum_name(%s_t val){\n\tswitch(val){\n", name);
    size_t i = 0;
    while(values[i] < current_value){
        fprintf(source, "\t\tcase %s: ", values[i]);
        fprintf(source, "return \"%s\";\n", replace_(values[i]));
        i++;
    }
    fputs("\t\tdefault: return \"\";\n\t}\n}", source);
}

int main(int argc, char** argv){
    memset(prefix, 0, 32);
    memset(values, 0, 2048*128);

    name = argv[1];
    char buffer[256];

    strcpy(buffer, name);
    strcat(buffer, ".hpp");
    header = fopen(buffer, "w");

    strcpy(buffer + strlen(name), ".cpp");
    source = fopen(buffer, "w");

    yyparse();
}

%}

%union {
    char* ch;
}

%token INDENT END
%token <ch> VALUE PREFIX

%%
logic: sentences END { create_enum(); create_output_fun(); };

sentences:
        | sentences sentence
        | sentences indents;

indents: INDENT
       | indents INDENT;

sentence: VALUE { current_value++; }
        | PREFIX;

%%

%{
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void yyerror(const char *str){ fprintf(stderr,"ошибка: %s\n",str);}

int yywrap(){ return 1; }

char* name;
char value[2048];

FILE* source;

char* replace_(char* src){
    char* ptr = src;
    while(*src){
        if(*src == '_')
            *(src) = ' ';
        src++;
    }
    return ptr;
}

void put_literal(){
    fprintf(source, "\t\tcase %s: ", value);
    fprintf(source, "return \"%s\";\n", replace_(value));
}

int main(int argc, char** argv){
    memset(value, 0, 2048);

    name = argv[1];
    char buffer[256];

    strcpy(buffer, name);
    strcat(buffer, ".cpp");
    source = fopen(buffer, "w");

    fprintf(source, "#include \"%s.hpp\"\n\n", name);
    fprintf(source, "const char* get_enum_name(%s_t val){\n\tswitch(val){\n", name);

    yyparse();

    fputs("\t\tdefault: return \"\";\n\t}\n}", source);
}

%}

%token SOME START INDENT LITERAL COMMA END

%%
logic: trash START INDENT values END trash;

trash:
     | SOME
     | trash SOME;

values: literal
      | values literal
      | values INDENT;

literal: LITERAL { put_literal(); }
       | LITERAL COMMA { put_literal(); };

%%

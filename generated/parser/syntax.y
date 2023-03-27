%{
#include <stdio.h>
#include <string.h>

char* high_buffer[512];
size_t tabs = 0;
size_t oldTabs = 0;

void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
}

int yywrap()
{
        return 1;
}

int main()
{
        high_buffer[0] = '\0';
        yyparse();
}

%}

%union {
    int num;
    char* ch;
}

%token IF FOUND EXIT
%token INDENT QUOTE NEWLINE TAB
%token MARK_ANY_PERMANENT MARK_PERMANENT MARK_ANY_SHARED MARK_PARMSHARED MARK_PURE MARK_SHARED
%token <num> NUMBER
%token <ch> EFFECT OUTPUT

%%

items: tabs condition
        | items NEWLINE tabs condition;

newCondition: NEWLINE NEWLINE

indents : INDENT
        | indents INDENT

item: effect
        {
                printf("It is item\n");
        } | property;

condition: IF indents FOUND indents item indents single_action
        {
                puts("The item should be found");
        }
         | tabs IF indents FOUND indents item
        {

        };

single_action: EXIT
        | EXIT indents OUTPUT
        {
                printf("output %s before exit\n", high_buffer);
        };

property: QUOTE EFFECT QUOTE
        {
                printf("Property: %s\n", high_buffer);
        };

tabs: 
        { oldTabs = tabs; tabs = 0; }
| tabs TAB
        { tabs++; };

effect: EFFECT
        {
                printf("effect %s definition\n", high_buffer);
        }
      | EFFECT MARK_PERMANENT
        {
                printf("permanent effect %s definition\n", high_buffer);
        };


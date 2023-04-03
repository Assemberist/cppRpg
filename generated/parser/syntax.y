%{
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
        puts("Done");
}

%}

%union {
    int num;
    char* ch;
}

%token IF THEN FOUND NOTFOUND ELSE END
%token EXIT PUT SET DELETE
%token LESS MORE EQ EQ_L EQ_M NONEQ MUCHMORE MUCHLESS
%token SUMM SUB
%token PLUS MINUS DIV MUL ASSUM
%token INDENT NEWLINE TAB FINAL QUOTE BRACE_OPEN BRACE_CLOSE
%token MARK_ANY_PERMANENT MARK_PERMANENT MARK_ANY_SHARED MARK_PARMSHARED MARK_PURE MARK_SHARED MARK_ANY
%token THIS GET_TIME GET_VALUE
%token <num> NUMBER
%token <ch> EFFECT OUTPUT

%%
logic: sentences END;

sentences:
        | sentences sentence
        | sentences NEWLINE;

sentence: single_expression
        | tabs single_expression;

single_expression   : action
                    | condition indents action
                    | condition;

condition   : IF indents FOUND indents item
            | IF indents NOTFOUND indents item
            | IF indents matan indents matan
            | ELSE;

item: effect
    | effect field
    | THIS
    | property;

effect: EFFECT
      | EFFECT mark;

property: QUOTE EFFECT QUOTE;

expr: item
    | NUMBER
    | expr indents op indents item
    | expr indents op indents NUMBER
    | BRACE_OPEN expr BRACE_CLOSE;

matan: expr indents sign
     | expr indents THEN;

action  : set_value
        | put_effect
        | delete_effect
        | EXIT
        | comment
        | action indents comment;

set_value: SET indents item indents matan
         | item indents item_mod indents expr FINAL;

put_effect: PUT indents effect BRACE_OPEN NUMBER BRACE_CLOSE
          | PUT indents effect BRACE_OPEN NUMBER indents NUMBER BRACE_CLOSE
          | PUT indents mark indents effect BRACE_OPEN NUMBER BRACE_CLOSE
          | PUT indents mark indents effect BRACE_OPEN NUMBER indents NUMBER BRACE_CLOSE;

delete_effect: DELETE indents effect;
comment : OUTPUT
        | OUTPUT '%' item;

indents: INDENT | indents INDENT;
tabs: TAB | tabs TAB;
mark: MARK_ANY_PERMANENT | MARK_PERMANENT | MARK_ANY_SHARED | MARK_PARMSHARED | MARK_PURE | MARK_SHARED | MARK_ANY;
field: GET_VALUE | GET_TIME;
item_mod: SUB | SUMM | ASSUM

sign: LESS | MORE | EQ | EQ_L | EQ_M | NONEQ | MUCHMORE | MUCHLESS;
op: PLUS| MINUS | DIV | MUL;

%%

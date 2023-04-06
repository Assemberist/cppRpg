%{
#include <stdint.h>
#include "effect_sup.h"

size_t tabs = 0;
size_t oldTabs = 0;

effect_stack e_stack;
e_stack.current = 0;

main_buffer m_buff;
m_buff.tail = m_buff.buffer;

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
    item_t mark_t;
    int num;
    char* ch;
}

%token IF THEN FOUND NOTFOUND ELSE END
%token EXIT PUT SET DELETE
%token SUMM SUB ASSUM
%token INDENT NEWLINE TAB FINAL QUOTE BRACE_OPEN BRACE_CLOSE
%token MARK
%token THIS GET_TIME GET_VALUE
%token <ch> EFFECT OUTPUT NUMBER OP SIGN

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

effect: EFFECT { effectFound(&e_stack, $1, EFF_PURE); }
      | EFFECT MARK { effectFound(&e_stack, $1, $2); };

property: QUOTE EFFECT QUOTE;

expr: item
    | NUMBER
    | expr indents OP indents item
    | expr indents OP indents NUMBER
    | BRACE_OPEN expr BRACE_CLOSE;

matan: expr indents SIGN
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
          | PUT indents MARK indents effect BRACE_OPEN NUMBER BRACE_CLOSE
          | PUT indents MARK indents effect BRACE_OPEN NUMBER indents NUMBER BRACE_CLOSE;

delete_effect: DELETE indents effect;
comment : OUTPUT
        | OUTPUT '%' item;

indents: INDENT | indents INDENT;
tabs: TAB | tabs TAB;
field: GET_VALUE | GET_TIME;
item_mod: SUB | SUMM | ASSUM

%%

%{
#include <stdint.h>
#include "effect_sup.h"

size_t tabs = 0;
size_t oldTabs = 0;

effect_s current_effect;

char m_buff[2048];

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
        m_buff[0] = '\0';
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
%token INDENT NEWLINE TAB FINAL QUOTE BRACE_OPEN BRACE_CLOSE COMMENT_BREAK
%token <mark_t> MARK
%token THIS GET_TIME GET_VALUE
%token <ch> EFFECT OUTPUT NUMBER OP SIGN

%%
logic: sentences END { checkTabs(0, oldTabs);};

sentences:
        | sentences sentence
        | sentences NEWLINE { checkTabs(tabs, oldTabs); oldTabs = tabs; tabs = 0; };

sentence: single_expression
        | tabs single_expression;

single_expression   : action { printf("ACTION\n"); }
                    | condition indents action { printf(" ACTION }\n"); }
                    | condition { puts(""); };

condition   : IF indents FOUND indents item { checkTabs(tabs, oldTabs); impl_found(); }
            | IF indents NOTFOUND indents item { checkTabs(tabs, oldTabs); impl_notfound(); }
            | IF indents matan indents matan { checkTabs(tabs, oldTabs); impl_matan(); }
            | ELSE { checkTabs(tabs, oldTabs); impl_else(); };

item: effect
    | effect field
    | THIS { current_effect = (effect_s){"CRUSH_ATTACK_TEST", EFF_PURE}; }
    | THIS field
    | property;

effect: EFFECT { current_effect = (effect_s){$1, EFF_PURE}; }
      | EFFECT MARK { current_effect = (effect_s){$1, $2}; };

property: QUOTE EFFECT QUOTE { current_effect = (effect_s){$2, EFF_PROPERTY}; };

expr: item { build_expr(NULL); }
    | NUMBER { current_effect = {$1. EFF_NUMBER}; build_expr(NULL); }
    | expr indents OP indents item { build_expr($3); }
    | expr indents OP indents NUMBER { build_expr($3); }
    | BRACE_OPEN expr BRACE_CLOSE { add_braces(); };

matan: expr indents SIGN {}
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
        | OUTPUT COMMENT_BREAK item;

indents: INDENT | indents INDENT;
tabs: TAB {tabs++;} | tabs TAB {tabs++;};
field: GET_VALUE | GET_TIME;
item_mod: SUB | SUMM | ASSUM

%%

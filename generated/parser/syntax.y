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
logic: sentences END { checkTabs(0, oldTabs); };

sentences:
        | sentences sentence
        | sentences NEWLINE { checkTabs(tabs, oldTabs); oldTabs = tabs; tabs = 0; };

sentence: single_expression
        | tabs single_expression;

single_expression   : action { put_comment(); printf("ACTION\n"); clean_buffers(); }
                    | condition indents action { put_comment(); printf(" ACTION }\n"); clean_buffers(); }
                    | condition { puts(""); clean_buffers(); };

condition   : IF indents FOUND indents item { checkTabs(tabs, oldTabs); impl_found(); m_buff[0] = '\0'; }
            | IF indents NOTFOUND indents item { checkTabs(tabs, oldTabs); impl_notfound(); m_buff[0] = '\0'; }
            | IF indents matan indents matan { checkTabs(tabs, oldTabs); impl_matan(); m_buff[0] = '\0'; }
            | ELSE { checkTabs(tabs, oldTabs); impl_else(); };

item: effect
    | effect field
    | THIS { current_effect = (effect_s){NULL, EFF_THIS}; }
    | THIS field
    | property;

effect: EFFECT { current_effect = (effect_s){$1, EFF_PURE}; }
      | EFFECT MARK { current_effect = (effect_s){$1, $2}; };

property: QUOTE EFFECT QUOTE { current_effect = (effect_s){$2, EFF_PROPERTY}; };

expr: item { build_expr(NULL); }
    | NUMBER { current_effect = (effect_s){$1, EFF_NUMBER}; build_expr(NULL); }
    | expr indents OP indents item { build_expr($3); }
    | expr indents OP indents NUMBER { build_expr($3); }
    | BRACE_OPEN expr BRACE_CLOSE { add_braces(); };

matan: expr indents SIGN { strcat(m_buff, $3); }
     | expr indents THEN;

action  : set_value
        | put_effect
        | delete_effect
        | EXIT
        | comment
        | action indents comment;

set_value: SET indents item indents expr { m_buff[0] = '\0'; }
         | item indents item_mod indents expr FINAL { m_buff[0] = '\0'; };

put_effect: PUT indents effect BRACE_OPEN NUMBER BRACE_CLOSE
          | PUT indents effect BRACE_OPEN NUMBER indents NUMBER BRACE_CLOSE
          | PUT indents MARK indents effect BRACE_OPEN NUMBER BRACE_CLOSE
          | PUT indents MARK indents effect BRACE_OPEN NUMBER indents NUMBER BRACE_CLOSE;

delete_effect: DELETE indents effect;
comment : OUTPUT { write_comment($1); }
        | OUTPUT COMMENT_BREAK item { write_comment_and_item($1); };

indents: INDENT | indents INDENT;
tabs: TAB {tabs++;} | tabs TAB {tabs++;};
field: GET_VALUE | GET_TIME;
item_mod: SUB | SUMM | ASSUM

%%

%{
#include <stdint.h>
#include "effect_sup.h"

size_t tabs = 0;
size_t oldTabs = 0;

effect_s current_effect;
effect_s help_effect;

char m_buff[2048];

void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
}

int yywrap()
{
        return 1;
}

int main(int argc, char** argv)
{
        printf("case %s:{\n", argv[1]);
        m_buff[0] = '\0';
        yyparse();
        puts("} break;\n");
}

%}

%union {
    item_t mark_t;
    int num;
    char* ch;
    const char* cchar;
}

%token IF THEN FOUND NOTFOUND ELSE END RAISE CLEAR NOT
%token EXIT PUT SET DELETE
%token SUMM SUB ASSUM MOD_DIV MOD_MUL
%token INDENT TAB
%token <mark_t> MARK
%token THIS GET_TIME GET_VALUE
%token <ch> EFFECT OUTPUT NUMBER OP SIGN FLAG

%type <cchar> item_mod

%%
logic: sentences END { checkTabs(0, oldTabs); };

sentences:
        | sentences sentence
        | sentences '\n' { checkTabs(tabs, oldTabs); oldTabs = tabs; tabs = 0; };

sentence: single_expression
        | tabs single_expression;

single_expression   : action { put_comment(); printf(m_buff); clean_buffers(); }
                    | condition indents action { put_comment(); printf("%s}\n", m_buff); clean_buffers(); }
                    | condition { puts(""); clean_buffers(); };

condition   : IF { checkTabs(tabs, oldTabs); } indents condition_body { m_buff[0] = '\0'; }
            | ELSE { checkTabs(tabs, oldTabs); impl_else(); m_buff[0] = '\0'; };

condition_body: FOUND indents item { impl_found(); }
              | NOTFOUND indents item { impl_notfound(); }
              | FLAG { impl_if_flag($1); }
              | NOT indents FLAG { impl_if_flag_neg($3); }
              | matan indents matan { impl_matan(); };

item: effect
    | effect field
    | THIS { current_effect = (effect_s){NULL, EFF_THIS}; }
    | THIS field
    | property;

effect: EFFECT { current_effect = (effect_s){$1, EFF_PURE}; }
      | EFFECT MARK { current_effect = (effect_s){$1, $2}; };

property: '\'' EFFECT '\'' { current_effect = (effect_s){$2, EFF_PROPERTY}; };

expr: item { build_expr(NULL); }
    | NUMBER { current_effect = (effect_s){$1, EFF_NUMBER}; build_expr(NULL); }
    | expr indents OP indents item { build_expr($3); }
    | expr indents OP indents NUMBER { build_expr($3); }
    | '(' expr ')' { add_braces(); };

matan: expr indents SIGN { strcat(m_buff, $3); }
     | expr indents THEN;

action  : set_value
        | put_effect
        | delete_effect { impl_delete(); }
        | EXIT { strcpy(m_buff, "return;\n"); }
        | comment
        | raise_flag
        | clear_flag
        | action indents comment;

raise_flag: RAISE indents FLAG { impl_raise_flag($3); };

clear_flag: CLEAR indents FLAG { impl_clear_flag($3); };

set_value: SET indents item indents expr { impl_set(help_effect, "=", tabs); m_buff[0] = '\0'; }
         | item indents item_mod indents expr ';' { impl_set(help_effect, $3, tabs); m_buff[0] = '\0'; };

put_effect: PUT indents effect '(' NUMBER ')' { current_effect.mark = EFF_PERMANENT; impl_put_permanent($5, tabs); }
          | PUT indents effect '(' NUMBER indents NUMBER ')' { current_effect.mark = EFF_PURE; impl_put($5, $7, tabs); }
          | PUT indents MARK indents effect '(' NUMBER ')' { current_effect.mark = $3; impl_put_permanent($7, tabs); }
          | PUT indents MARK indents effect '(' NUMBER indents NUMBER ')' { current_effect.mark = $3; impl_put($7, $9, tabs); };

delete_effect: DELETE indents effect;
comment : OUTPUT { write_comment($1); }
        | OUTPUT '%' item { write_comment_and_item($1); };

indents: INDENT | indents INDENT;
tabs: TAB {tabs++;} | tabs TAB {tabs++;};
field: GET_VALUE | GET_TIME;
item_mod: SUB { help_effect = current_effect; $$ = "-="; }
        | SUMM { help_effect = current_effect; $$ = "+="; }
        | ASSUM { help_effect = current_effect; $$ = "="; }
        | MOD_DIV { help_effect = current_effect; $$ = "/="; }
        | MOD_MUL { help_effect = current_effect; $$ = "*="; };

%%

%{
#include "y.tab.h"

extern char value[2048];

static int is_started = 0;

%}
%%

enum.*\{            is_started = 1; return START;
[ \t\n]+            return is_started ? INDENT : SOME;
,                   return is_started ? COMMA : SOME;
[0-9a-zA-Z_]+       if(is_started){strcpy(value, yytext); return LITERAL;} else return SOME;
\}                  if(is_started){is_started = 0; return END;} else return SOME;
[^e \t\n,\}]+       return SOME;

%%

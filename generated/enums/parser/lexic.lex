%{
#include "y.tab.h"
#include <stdbool.h>

extern char prefix[32];
extern char values[2048][128];
extern char (*current_value)[128];

bool term = false;
#define yyterminate() return (term = !term) ? END : YY_NULL

%}
%%
-prefix\ [a-zA-Z_]+     strcpy(prefix, yytext+sizeof("-prefix")); return PREFIX;
[ \t\n]                 return INDENT;
[a-zA-Z_]+              if(prefix[0]) {strcpy(current_value, prefix); strcat(current_value, "_");} strcat(current_value, yytext); return VALUE;

%%

%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "y.tab.h"

bool term = false;
#define yyterminate() return (term = !term) ? END : YY_NULL

extern char high_buffer[512];

%}
%%
if              return IF;
found           return FOUND;
exit            return EXIT;
put             return PUT;
set             return SET;
delete          return DELETE;
then            return THEN;
this            return THIS;
notfound        return NOTFOUND;
else            return ELSE;
\<              return LESS;
>               return MORE;
'               return QUOTE;
==              return EQ;
=               return ASSUM;
\<=             return EQ_L;
>=              return EQ_M;
!=              return NONEQ;
\<\<            return MUCHLESS;
>>              return MUCHMORE;
\+              return PLUS;
\-              return MINUS;
\*              return MUL;
\/              return DIV;
\+\+            return SUMM;
--              return SUB;
\.value         return GET_VALUE;
;               return FINAL;
\(              return BRACE_OPEN;
\)              return BRACE_CLOSE;
\.time          return GET_TIME;
[A-Z_]+         strcpy(high_buffer, yytext); return EFFECT;
:[^\n]+         strcpy(high_buffer, yytext+1); return OUTPUT;
[0-9]+          yylval.num=atoi(yytext); return NUMBER;
\(\)            return MARK_PURE;
\(P\)           return MARK_PERMANENT;
\(P\+\)         return MARK_ANY_PERMANENT;
\(PS\)          return MARK_PARMSHARED;
\(S\)           return MARK_SHARED;
\(S\+\)         return MARK_ANY_SHARED;
\(PS\+\)        return MARK_ANY;
(\t|\ \ \ \ )   return TAB;
[ \t]           return INDENT;
\n              return NEWLINE;
%%

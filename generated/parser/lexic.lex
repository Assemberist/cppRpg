%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"

extern char high_buffer[512];

%}

%%
if              return IF;
then            return THEN;
notfound        return NOTFOUND;
found           return FOUND;
this            return THIS;
exit            return EXIT;
<               return LESS;
>               return MORE;
=               return EQ;
<=              return EQ_L;
>=              return EQ_M;
!=              return NONEQ;
<<              return MUCHLESS;
>>              return MUCHMORE;
++              return SUMM;
--              return SUB;
\.value         return GET_VALUE;
\.time          return GET_TIME;
[A-Z]+          strcpy(high_buffer, yytext); return EFFECT;
:[^\n]+         strcpy(high_buffer, yytext+1); return OUTPUT;
[0-9]+          yylval.num=atoi(yytext); return NUMBER;
\(\)            return MARK_PURE;
\(P\)           return MARK_PERMANENT;
\(P\+\)         return MARK_ANY_PERMANENT;
\(PS\)          return MARK_PARMSHARED;
\(S\)           return MARK_SHARED;
\(S\+\)         return MARK_ANY_SHARED;
'               return QUOTE;
(\t|\ \ \ \ )   return TAB;
[ \t]           return INDENT;
\n              return NEWLINE;
%%
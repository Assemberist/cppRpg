%{
#include "effect_sup.h"
#include "y.tab.h"

bool term = false;
#define yyterminate() return (term = !term) ? END : YY_NULL
#define returnMark(A) yylval.mark_t=(A); return MARK;

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
\.value         return GET_VALUE;
\.time          return GET_TIME;

(\t|\ \ \ \ )   return TAB;
[ \t]           return INDENT;
\n              return NEWLINE;
'               return QUOTE;
%               return COMMENT_BREAK;
;               return FINAL;
\(              return BRACE_OPEN;
\)              return BRACE_CLOSE;

\+\+            return SUMM;
--              return SUB;
=               return ASSUM;

(\<|>|==|\<=|>=|!=|\<\<|>>)     yylval.ch=strdup(yytext); return SIGN;
[\+\-\*\/]                      yylval.ch=strdup(yytext); return OP;

[A-Z_]+         yylval.ch=strdup(yytext); return EFFECT;
:[^\n%]+        yylval.ch=strdup(yytext); return OUTPUT;
[0-9]+          yylval.ch=strdup(yytext); return NUMBER;

\(\)            returnMark(EFF_PURE);
\(P\)           returnMark(EFF_PERMANENT);
\(P\+\)         returnMark(EFF_PERMANENT_ANY);
\(PS\)          returnMark(EFF_PERMASHARED);
\(S\)           returnMark(EFF_SHARED);
\(S\+\)         returnMark(EFF_SHARED_ANY);
\(PS\+\)        returnMark(EFF_ANY);
%%

%option noyywrap nodefault yylineno

%{
#include "helper.h"
#include "tree_gen.tab.h"
%}

%%

"[" |
"]" { return yytext[0]; }

\n { return EOL; }

[ \t]+ { }

[^ \[\]\t\n]+ {
    yylval.text_key = get_key_from_text(yytext);
    return TEXT_KEY;
}

%%

#pragma once

extern int yylineno;
void yyerror(const char *s, ...);
extern "C" int yylex();
extern "C" int yyparse();

typedef struct node
{
    int text_key;
    int num_children;
    struct node *first_child;
    struct node *next_sibling;
} Node;

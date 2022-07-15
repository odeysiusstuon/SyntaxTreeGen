%{
#include "helper.h"
%}

%union {
    int text_key;
    Node *node;
}

%token EOL
%token <text_key> TEXT_KEY

%type <node> children
%type <node> node

%start tree

%%

children:
    node {
        // This should be the first child
        $$ = $1;
    }
|   children node {
        insert_sibling($1, $2);
        // This should be the first child
        $$ = $1;
    }
;

node:
    '[' TEXT_KEY TEXT_KEY ']' {
        Node *lexical_node = new_node($3, nullptr, nullptr);
        $$ = new_node($2, lexical_node, nullptr);
    }
|   '[' TEXT_KEY children ']' { $$ = new_node($2, $3, nullptr); }
;

tree: /* nothing */
|   tree node EOL {
        GVC_t *gvc = gvContext();
        Agraph_t *g = agopen(strdup("g"), Agundirected, 0);

        node_to_graphviz(g, $2);
        
        gvLayout(gvc, g, "dot");

        gvRender(gvc, g, "dot", stdout);

        gvFreeLayout(gvc, g);

        agclose(g);

        gvFreeContext(gvc);
        
        node_free($2);
        graph_strings_free();
    }
|   tree EOL { }
;

%%

#pragma once

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <gvc.h>
#include <string.h>
#include <map>
#include <stdarg.h>
#include "tree_gen.h"

int get_key_from_text(char *text);

const std::string *get_text_from_key(int key);

void free_text_key(int key);

Node *new_node(int text_key, Node *first_child, Node *next_sibling);

int count_children(Node *parent);

void insert_sibling(Node *first_child, Node *new_sibling);

char *get_next_node_name();

char *get_next_subgraph_name();

Agnode_t *node_to_graphviz(Agraph_t *g, Node *node);

void node_free(Node *node);

void yyerror(const char *s, ...);

int main();

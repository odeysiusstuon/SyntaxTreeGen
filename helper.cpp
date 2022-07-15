#include "helper.h"

std::vector<std::string> texts;

char *empty_str = strdup("");
char *label_str = strdup("label");
char *shape_str = strdup("shape");
char *style_str = strdup("style");
char *weight_str = strdup("weight");
char *ten_weight_str = strdup("10");
char *invis_str = strdup("invis");
char *none_str = strdup("none");

std::map<int, char *> num_strs;

int get_key_from_text(char *text)
{
    auto result = std::find(texts.begin(), texts.end(), text);

    if (result == texts.end())
    {
        result = texts.insert(texts.end(), text);
    }

    return result - texts.begin();
}

char *num_to_str(int num)
{
    auto num_pos = num_strs.find(num);

    if (num_pos == num_strs.end())
    {
        std::string num_str = std::to_string(num);
        num_pos = num_strs.insert(num_pos, std::make_pair(num, strdup(num_str.c_str())));
    }

    return num_pos->second;
}

const std::string *get_text_from_key(int key)
{
    if (key >= texts.size())
    {
        return nullptr;
    }

    return (const std::string *)(&texts[key]);
}

void free_text_key(int key)
{
    if (key >= texts.size())
    {
        return;
    }

    texts.erase(texts.begin() + key);
}

Node *new_node(int text_key, Node *first_child, Node *next_sibling)
{
    Node *node = new Node;

    if (!node) { yyerror("out of memory\n"); exit(1); }

    node->text_key = text_key;
    node->first_child = first_child;
    node->next_sibling = next_sibling;
    node->num_children = count_children(node);

    return node;
}

int count_children(Node *parent)
{
    int num_children = 0;
    Node *curr = parent->first_child;
    while (curr != nullptr)
    {
        ++num_children;
        curr = curr->next_sibling;
    }
    return num_children;
}

void insert_sibling(Node *first_child, Node *new_sibling)
{
    Node *curr = first_child->next_sibling;
    if (curr == nullptr)
    {
        first_child->next_sibling = new_sibling;
    }
    else
    {
        while (curr->next_sibling != nullptr)
        {
            curr = curr->next_sibling;
        }
        curr->next_sibling = new_sibling;
    }
}

// NOTE: What is being done with the strings here is usually dangerous.
// However, I am assuming gvc does not actually do anything with these "non-const"
// strings, so I just simply convert const strings to a normal "char *".
// I unfortunately cannot just pass "const char *" into gvc functions, since
// they all take "char *" and 
char *get_next_node_name()
{
    static int curr = 0;
    return num_to_str(curr++);
}

Agnode_t *node_to_graphviz(Agraph_t *g, Node *node)
{
    char *node_name = get_next_node_name();
    Agnode_t *g_node = agnode(g, node_name, 1);

    Node *curr = node->first_child;
    Agnode_t *g_curr = nullptr;
    int current_child_index = 1;
    bool should_create_middle = curr != nullptr && node->num_children > 0 && node->num_children % 2 == 0;

    // Draw all the children
    while (curr != nullptr)
    {
        g_curr = node_to_graphviz(g, curr);
        Agedge_t *node_child_edge = agedge(g, g_node, g_curr, get_next_node_name(), 1);
        // agsafeset(node_child_edge, weight_str, ten_weight_str, empty_str);

        // "Invisible middle" is a hacky (but apparently officially given as a solution,
        // see https://graphviz.org/faq/#FaqBalanceTree) way of balancing the tree, such 
        // that parents will be centered above its children.
        if (should_create_middle && current_child_index == node->num_children / 2)
        {
            Agnode_t *g_invisible_middle = nullptr;
            char *middle_name = get_next_node_name();
            g_invisible_middle = agnode(g, middle_name, 1);
            agsafeset(g_invisible_middle, label_str, empty_str, empty_str);
            agsafeset(g_invisible_middle, style_str, invis_str, empty_str);
            agsafeset(g_invisible_middle, shape_str, none_str, empty_str);
            // agsafeset(g_invisible_middle, width_str, zero_width_str, empty_str);

            Agedge_t *middle_edge = agedge(g, g_node, g_invisible_middle, get_next_node_name(), 1);
            agsafeset(middle_edge, style_str, invis_str, empty_str);
            agsafeset(middle_edge, weight_str, ten_weight_str, empty_str);
        }

        curr = curr->next_sibling;
        ++current_child_index;
    }

    // Set some attributes for the node
    char *label = (char *)(get_text_from_key(node->text_key)->c_str());
    agsafeset(g_node, "label", label, empty_str);
    agsafeset(g_node, shape_str, none_str, empty_str);

    return g_node;
}

void node_free(Node *node)
{
    if (node->first_child != nullptr)
    {
        node_free(node->first_child);
    }

    if (node->next_sibling != nullptr)
    {
        node_free(node->next_sibling);
    }

    free_text_key(node->text_key);

    free(node);
}

void graph_strings_free()
{
    for (auto it = num_strs.begin(); it != num_strs.end(); ++it)
    {
        delete (*it).second;
    }
    num_strs.clear();
}

void yyerror(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vprintf(s, args);
    fprintf(stderr, "\n");
}

int main()
{
    return yyparse();
}

#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include "token.h"

using namespace std;

typedef struct node{

    string literal;
    int tok_length;
    int level;
    struct node *child_1, *child_2, *child_3, *child_4;
    vector<Token> all_toks;
} node;

#endif

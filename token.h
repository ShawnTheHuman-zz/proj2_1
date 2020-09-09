#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <string>

using namespace std;

extern int line;
extern int line_pos;

typedef enum Token_types{
    ID_tk, INT_tk, OP_tk,
    KEYW_tk, EOF_tk
} token_types;

const string token_names[5] = {"Identifier", "Integer", "Operator", "Keyword", "ENDofFILE"};

const string operators[19] = {":=", "<", ">", "==", ":", "+", "-", "*", "/", "%", ".", "(", ")",
                      ",", "{", "}", ";", "[", "]"};

const string keywords[13] = {"label", "goto", "loop", "void", "declare", "return", "in", "out",
                    "program", "iffy", "then", "assign", "data"};

typedef struct token{
    token_types ID;
    string tok_string;
    int line_number;
} Token;

void display(Token);
#endif // TREE_H

#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "node.h"
#include <string>

using namespace std;

extern Token t0k;

//main parser function
node *parser();

//The BNF for the program.
node *program();
node *block();
node *vars();
node *expr();
node *N();
node *A();
node *M();
node *R();
node *stats();
node *mStat();
node *stat();
node *in();
node *out();
node *_if();
node *loop();
node *assign();
node *label();
node *_goto();
node *RO();

//Print an error
void error();

//Create a new node.
node *create_node(string);

#endif

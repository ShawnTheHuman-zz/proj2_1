#ifndef SCANNER_H
#define SCANNER_H
#include <fstream>
#include <stdio.h>
#include <string>
#include<vector>
#include "token.h"

//Driver program
int scanner(Token &);

void load_vector(fstream &);

void filter(string &);

int valid_operators(char, char);

int valid_keyword(string);

int final_states(Token &, int, string);

extern vector<string> vector_string;

//All states in the FSA table
const int S0 = 0;
const int S1 = 1;
const int S2 = 2;
const int S3 = 3;

//All FINAL states in the FSA table
const int final_S = 1000;
const int IDENT_final_S = 1001;
const int INT_final_S = 1002;
const int OPER_final_S = 1003;
const int EOF_final_S = 1004;
const int ERROR_S = -1;

const int FSA_table[4][6] = {{S1, S1, S2, S0, S3, EOF_final_S},
                {S1, S1, S1, IDENT_final_S, IDENT_final_S, IDENT_final_S},
                {ERROR_S, ERROR_S, S2, INT_final_S, INT_final_S},
                {OPER_final_S, OPER_final_S, OPER_final_S, OPER_final_S, S3, OPER_final_S}
};

/*
    What the FSA table looks like on paper!
       UPPER   lower   digit   space   operator    EOF
S0 |    S1      S1      S2      S0      S3       EOF_tk
S1 |    S1      S1      S1      ID_tk   ID_tk    ID_tk
S2 |    ERROE   ERROR   S2      INT_tk  INT_tk   INT_tk
S3 |    OP_tk   OP_tk   OP_tk   OP_tk   S3       OP_tk
*/


#endif //SCANNER_H

#include <iostream>
#include <stdlib.h>
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "node.h"

using namespace std;

Token t0k;
static string ex_tok;
static Token EMPTY_TOKEN;

node *parser(){

    EMPTY_TOKEN.instance = "EMPTY";

    //scanner driver
    scanner(t0k);

    //Create the root node
    node *root = NULL;

    root = program();

    if(token_names[t0k.ID] != "ENDofFILE"){
        ex_tok.assign("EOF");
        error();
    }
    else{
        //Indicate that parsing was successful.
        cout << "+-+-+- Parse OK +-+-+-\n";
        return root;
    }
}

node *program(){

    node *tree = create_node("<program>");


    if((t0k.instance == "Keyword data")||(t0k.instance == "Keyword main")){
        tree->child_1 = vars();
        scanner(t0k);
        if((t0k.instance == "Keyword main")|| (t0k.instance == "Keyword begin")) {


            tree->child_2 = block();
        }


    return tree;
}
    else{
        ex_tok.assign("Declare or {");
        error();
    }
}

//<vars> => empty | data Identifier := Integer ; <vars>
node *vars(){


    node *tree = create_node("<vars>");


    if((token_names[t0k.ID] == "Keyword") && (t0k.instance == "Keyword data")){


        scanner(t0k);

        if (token_names[t0k.ID] == "Identifier"){


            tree->all_toks.push_back(t0k);


            scanner(t0k);


            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator :=")){

                scanner(t0k);

                if (token_names[t0k.ID] == "Integer"){

                    tree->all_toks.push_back(t0k);

                    scanner(t0k);

                    if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){

                        tree->all_toks.push_back(t0k);

                        scanner(t0k);


                        tree->child_1 = vars();

                        return tree;
                    }
                    else{
                        ex_tok.assign(";");
                        error();
                    }
                }
                else{
                    ex_tok.assign("Integer");
                    error();
                }
            }
            else{
                ex_tok.assign(":=");
                error();
            }
        }
        else{
            ex_tok.assign("Identifier");
            error();
        }
    }
    else{

        tree->all_toks.push_back(EMPTY_TOKEN);
        return tree;
    }
}

//<block> => { <vars> <stats> }
node *block(){


    node *tree = create_node("<block>");


    if((token_names[t0k.ID] == "Keyword") && (t0k.instance == "Keyword begin")){

        scanner(t0k);

        tree->child_1 = vars();
        tree->child_2 = stats();

        if ((token_names[t0k.ID] == "Keyword") && (t0k.instance == "Keyword end")){

            scanner(t0k);

            return tree;
        }
        else{
            ex_tok.assign("end");
            error();
        }
    }
    else{
        ex_tok.assign("begin");
        error();
    }
}

//<expr> => <N> - <expr> | <N>
node *expr(){

    node *tree = create_node("<expr>");


    tree->child_1 = N();


    if((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator -")){


        tree->all_toks.push_back(t0k);


        scanner(t0k);


        tree->child_2 = expr();
    }

    return tree;
}

//<N> => <A> / <N> | <A> * <N> | <A>
node *N(){


    node *tree = create_node("<expr>");


    tree->child_1 = A();


    if (token_names[t0k.ID] == "Operator"){

        if (t0k.instance == "Operator /"){
            //Predicts <expr> -> <A> / <N>

            tree->all_toks.push_back(t0k);


            scanner(t0k);


            tree->child_2 = N();
        }
        else if(t0k.instance == "Operator *"){

            tree->all_toks.push_back(t0k);

            scanner(t0k);

            tree->child_2 = N();
        }
    }

    return tree;
}

//A => <M> + <A> | <M>
node *A(){


    node *tree = create_node("<A>");

    tree->child_1 = M();

    if((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator +")){
        //Predicts the A -> <M> + <A> production
        //Store the '+' operator in the <A> node
        tree->all_toks.push_back(t0k);

        scanner(t0k);

        tree->child_2 = A();
    }

    return tree;
}

//<M> => * <M> | <R>
node *M(){


    node *tree = create_node("<M>");

    if((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator *")){

        tree->all_toks.push_back(t0k);

        scanner(t0k);

        tree->child_1 = M();
        return tree;
    }

    tree->child_1 = R();
    return tree;
}

//<R> => ( <expr> ) | Identifier | Integer
node *R(){

    node *tree = create_node("<R>");


    if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator (")){
        scanner(t0k);

        tree->child_1 = expr();

        if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator )")){
            scanner(t0k);
            return tree;
        }
        else{
            ex_tok.assign(")");
            error();
        }
    }
    else if(token_names[t0k.ID] == "Identifier"){
        tree->all_toks.push_back(t0k);

        scanner(t0k);
        return tree;
    }
    else if(token_names[t0k.ID] == "Integer"){
        tree->all_toks.push_back(t0k);

        scanner(t0k);
        return tree;
    }
    else{
        ex_tok.assign("( or Identifier or Integer");
        error();
    }
}

//<stats> => <stat> <mStat>
node *stats(){

    node *tree = create_node("<stats>");

    if((token_names[t0k.ID] == "Identifier") || (t0k.instance == "Keyword getter") || (t0k.instance == "Keyword outter") || (t0k.instance == "Keyword begin")
        || (t0k.instance == "Keyword if") || (t0k.instance == "Keyword loop") || (t0k.instance == "Keyword data")
       || (t0k.instance == "Keyword proc") || (t0k.instance == "Keyword void")){

        tree->child_1 = stat();
        tree->child_2 = mStat();

        return tree;
    }
}

//<mStat> => empty | <stat> <mStat>
node *mStat(){

    node *tree = create_node("<mStat>");

    if((t0k.instance == "Keyword getter") || (t0k.instance == "Keyword outter") || (t0k.instance == "Keyword begin")
        || (t0k.instance == "Keyword if") || (t0k.instance == "Keyword loop") || (t0k.instance == "Keyword data")
       || (t0k.instance == "Keyword proc") || (t0k.instance == "Keyword void")){

        tree->child_1 = stat();
        tree->child_2 = mStat();

        return tree;
    }
    else{
        tree->all_toks.push_back(EMPTY_TOKEN);
        return tree;
    }
}

node *stat(){

    node *tree = create_node("<stat>");

    if ((t0k.instance == "Keyword getter")){

        tree->child_1 = in();

        return tree;
    }
    else if ((t0k.instance == "Keyword outter")){

        tree->child_1 = out();

        return tree;
    }
    else if ((t0k.instance == "Keyword begin")){

        tree->child_1 = block();

        return tree;
    }
    else if ((t0k.instance == "Keyword if")){
        scanner(t0k);

        tree->child_1 = _if();
        return tree;
    }
    else if ((t0k.instance == "Keyword loop")){
      scanner(t0k);

      tree->child_1 = loop();
      return tree;
    }
    else if ((token_names[t0k.ID] == "Identifier")){

        Token temp_token = t0k;

        scanner(t0k);

        tree->child_1 = assign();

        tree->child_1->all_toks.push_back(temp_token);
        return tree;
    }
    else if ((t0k.instance == "Keyword proc")){
        scanner(t0k);

        tree->child_1 = _goto();
        return tree;
    }
    else if ((t0k.instance == "Keyword void")){
        scanner(t0k);

        tree->child_1 = label();
        return tree;
    }
    else if((t0k.instance == "Keyword end")){
        scanner(t0k);
        return tree;
    }
  else
  {
    ex_tok.assign("Too Many different options");
    error();
  }
}

//<in> =>  getter Identifier
node *in(){

    node *tree = create_node("<in>");

    if ((t0k.instance == "Keyword getter")){

        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            tree->all_toks.push_back(t0k);

            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
          ex_tok.assign("identifier");
          error();
        }
      }else{
        ex_tok.assign("getter");
        error();
      }
}

//<out> => outter <expr>
node *out(){

    node *tree = create_node("<out>");

    if((t0k.instance == "Keyword outter")){
        scanner(t0k);

        tree->child_1 = expr();

        if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
            scanner(t0k);
            return tree;
        }
        else{
            ex_tok.assign(";");
            error();
        }

    }else{
        ex_tok.assign("Keyword outter");
        error();
    }
}

//<if> => if [ <expr> <RO> <expr>] then <stat>
node *_if(){
    node *tree = create_node("<if>");

    if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator [")){
        scanner(t0k);

        tree->child_1 = expr();
        tree->child_2 = RO();
        tree->child_3 = expr();
    //Check that the token returned by the scanner is a ']' operator
    if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ]")){
        scanner(t0k);

        if((token_names[t0k.ID] == "Keyword") && (t0k.instance == "Keyword then")){

            scanner(t0k);

            tree->child_4 = stat();

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
            ex_tok.assign("Keyword then");
            error();
        }
    }else{
      ex_tok.assign("]");
      error();
    }
  }else{
    ex_tok.assign("[");
    error();
  }
}

//<loop> => loop [ <expr> <RO> <expr> ] <stat>
node *loop(){
    node *tree = create_node("<loop>");

    if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator [")){
        scanner(t0k);

        tree->child_1 = expr();
        tree->child_2 = RO();
        tree->child_3 = expr();

        if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ]")){
            scanner(t0k);

            tree->child_4 = stat();

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
            ex_tok.assign("]");
            error();
        }
    }else{
        ex_tok.assign("[");
        error();
    }
}

//<assign> => Identifier := <expr>
node *assign(){

    //cout << "In <assign> with " << t0k.instance << endl;
    //Create the node for the <assign> production.
    node *tree = create_node("<assign>");

    //Check that the token returned by the scanner is a ':=' operator
    if((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator :=")){
        scanner(t0k);

        tree->child_1 = expr();

        //scanner(t0k);

        if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
            scanner(t0k);
            return tree;
        }
        else{
            ex_tok.assign(";");
            error();
        }
    }else{
        ex_tok.assign(":=");
        error();
    }
}

//<label> => label Identifier
node *label(){
    node *tree = create_node("<label>");

    if ((t0k.instance == "Keyword void")){

        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            tree->all_toks.push_back(t0k);

            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
          ex_tok.assign("label");
          error();
        }
      }else{
        ex_tok.assign("Identifier");
        error();
      }
}

//<goto => goto Identifier
node *_goto(){
    node *tree = create_node("<goto>");

    //Check if the token returned by the scanner is a '(' operator
    if ((t0k.instance == "Keyword proc")){

        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            tree->all_toks.push_back(t0k);

            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
          ex_tok.assign("proc");
          error();
        }
      }else{
        ex_tok.assign("Identifier");
        error();
      }
}


node *RO() {


    node *tree = create_node("<RO>");


    if ((token_names[t0k.ID] == "Operator")) {
        if ((t0k.instance == "Operator =")) {

            tree->all_toks.push_back(t0k);


            scanner(t0k);


            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator <")) {

                tree->all_toks.push_back(t0k);


                scanner(t0k);
                return tree;
            } else if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator >")) {

                tree->all_toks.push_back(t0k);


                scanner(t0k);
                return tree;
            }
            if ((token_names[t0k.ID] == "Operator") && (t0k.instance != "Operator <") &&
                (t0k.instance != "Operator >")) {

                ex_tok.assign("=< or =>");
                error();
            }
            else{
                return tree;
            }
        } else if ((t0k.instance == "Operator [")) {

            tree->all_toks.push_back(t0k);

            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ==")) {

                tree->all_toks.push_back(t0k);

                scanner(t0k);
                if ((token_names[t0k.ID] == "Operator") && (t0k.instance == "Operator ]")) {
                    scanner(t0k);
                    return tree;
                }
            }


        } else {
            ex_tok.assign("[ == ]");
            error();
        }
    }
}

node *create_node(string name_me)
{
    node *tree = new node();
    tree->child_1 = NULL;
    tree->child_2 = NULL;
    tree->child_3 = NULL;
    tree->child_4 = NULL;

    tree->literal = name_me;

    return tree;
}

void error(){
    cout << "ErRoR: Looking for " << ex_tok << " but found " << t0k.instance <<  endl;
    exit(0);
}

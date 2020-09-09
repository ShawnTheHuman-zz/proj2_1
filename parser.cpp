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

    EMPTY_TOKEN.tok_string = "EMPTY";

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

    //cout << "First TOKEN = " << token_names[t0k.ID] << endl;

    if((t0k.tok_string == "Keyword declare") || (t0k.tok_string == "Operator {")){

        //
        tree->child_1 = vars();
        tree->child_2 = block();


        return tree;
    }else{
        ex_tok.assign("Declare or {");
        error();
    }
}

//<vars> => empty | declare Identifier := Integer ; <vars>
node *vars(){

    //Create the node associated with the <vars> production
    node *tree = create_node("<vars>");

    //Token string returned declare
    if((token_names[t0k.ID] == "Keyword") && (t0k.tok_string == "Keyword declare")){

        //Get the next token
        scanner(t0k);

        if (token_names[t0k.ID] == "Identifier"){

            //Store the identifier in this node's vector of tokens
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            //Check that the token returned by the scanner is the ':='
            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator :=")){
                //Get the next token
                scanner(t0k);

                //Check that the token returned by the scanner is an integer
                if (token_names[t0k.ID] == "Integer"){
                    //Store the integer in this node's vector of tokens
                    tree->all_toks.push_back(t0k);

                    //Get the next token from the scanner
                    scanner(t0k);

                    if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){

                        tree->all_toks.push_back(t0k);

                        scanner(t0k);

                        //The child_1 of this node will be <vars>
                        tree->child_1 = vars();

                        return tree;
                    }//end operator if
                    else{
                        ex_tok.assign(";");
                        error();
                    }
                }//end integer if
                else{
                    ex_tok.assign("Integer");
                    error();
                }
            }//end operator if
            else{
                ex_tok.assign(":=");
                error();
            }
        }//end indentifier if
        else{
            ex_tok.assign("Identifier");
            error();
        }
    }//end keyword if
    else{
        //empty
        tree->all_toks.push_back(EMPTY_TOKEN);
        //cout << "vars EMPTY" << endl;
        return tree;
    }
}

//<block> => { <vars> <stats> }
node *block(){

    //Create the node for the <block> production
    node *tree = create_node("<block>");

    //Check  '{' token
    if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator {")){
        //tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
        scanner(t0k);

        //<vars>, and set the child_1
        //<stats>, and set the child_2 of <block>
        tree->child_1 = vars();
        tree->child_2 = stats();

        //Check for '}' token
        if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator }")){
            //Get the next token from the scanner
            scanner(t0k);

            return tree;
        }//end inner operator if
        else{
            ex_tok.assign("}");
            error();
        }
    }//end outer operator if
    else{
        ex_tok.assign("{");
        error();
    }
}//end <block>

//<expr> => <N> - <expr> | <N>
node *expr(){

    //Create the node for the <expr> production
    node *tree = create_node("<expr>");

    //Invoke N(), and set the child_1 of the <expr> node to the
    //node returned by <N>
    tree->child_1 = N();

    //Check that the token returned by the scanner is an operator
    if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator -")){

        //Store the '-' operator in the <expr> node
        tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
        scanner(t0k);

        //Invoke expr(), and set the child_2 of the <expr> node
        //to the node returned by <expr>
        tree->child_2 = expr();
    }

    return tree;
}

//<N> => <A> / <N> | <A> * <N> | <A>
node *N(){

    //Create the node for the <expr> production
    node *tree = create_node("<expr>");

    //Invoke A(), and set the child_1 of the <expr> node to the
    //node returned by <A>
    tree->child_1 = A();

    //Check that the token returned by the scanner is an operator
    if (token_names[t0k.ID] == "Operator"){
        //Check if the operator is a '/' or '*'
        if (t0k.tok_string == "Operator /"){
            //Predicts <expr> -> <A> / <N>

            //Store the '/' operator in the <expr> node
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            //N(), and set the child_2 of the <N> node
            tree->child_2 = N();
        }
        else if(t0k.tok_string == "Operator *"){
            //Predicts <expr> -> <A> * <N>
            //Store the '*' operator in the <N> node
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            //expr(), set the child_2 of the <expr> node
            tree->child_2 = N();
        }
    }

    return tree;
}

//A => <M> + <A> | <M>
node *A(){

    //Create the node for the <A>
    node *tree = create_node("<A>");

    //Invoke M(), and set the child_1 node of the <A> production
    //to the node returned by <M>
    tree->child_1 = M();

    //Check that the token returned by the scanner is an operator
    if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator +")){
        //Predicts the A -> <M> + <A> production
        //Store the '+' operator in the <A> node
        tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
        scanner(t0k);

        //Set the child_2 node of <A> to the node returned by A()
        tree->child_2 = A();
    }

    return tree;
}

//<M> => * <M> | <R>
node *M(){

    //Create the node for the <M>
    node *tree = create_node("<M>");

    //Check for unary '*'; then invoke M()
    //Otherwise R().
    if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator *")){
        //Store the unary '*' in the <M> production
        tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
        scanner(t0k);

        //Set the child_1 node of <M> to the node returned by <M>
        tree->child_1 = M();
        return tree;
    }

    //Set the child_1 node of <M> to the node returned by <R>
    tree->child_1 = R();
    return tree;
}

//<R> => ( <expr> ) | Identifier | Integer
node *R(){

    //Create the node for the <R> production
    node *tree = create_node("<R>");

    //if '(' present or
    //if it is an identifier or
    //if it is an integer.
    if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator (")){
        //Get the next token from the scanner
        scanner(t0k);

        //Set the child_1 node of <R> to the node returned by <expr>
        tree->child_1 = expr();

        //Check that the token returned by the scanner is a ')' operator
        if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator )")){
            //Get the next token from the scanner
            scanner(t0k);
            return tree;
        }
        else{
            ex_tok.assign(")");
            error();
        }
    }
    else if(token_names[t0k.ID] == "Identifier"){
        //If the token returned by the scanner is an identifier, store
        //the identifier in the node
        tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
        scanner(t0k);
        return tree;
    }
    else if(token_names[t0k.ID] == "Integer"){
        //If the token returned by the scanner is an integer, store
        //the integer in the node
        tree->all_toks.push_back(t0k);

        //Get the next token from the scanner
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

    //Create the node for the <stats>
    node *tree = create_node("<stats>");

    //Invoke <stat> <mStat>, and set the child1_1 and child_2 nodes
    //to the node(s)
    if((token_names[t0k.ID] == "Identifier") || (t0k.tok_string == "Keyword in") || (t0k.tok_string == "Keyword out") || (t0k.tok_string == "Operator {")
        || (t0k.tok_string == "Keyword iffy") || (t0k.tok_string == "Keyword loop") || (t0k.tok_string == "Keyword assign")
       || (t0k.tok_string == "Keyword goto") || (t0k.tok_string == "Keyword label")){

        tree->child_1 = stat();
        tree->child_2 = mStat();

        return tree;
    }
}

//<mStat> => empty | <stat> <mStat>
node *mStat(){

    //Create the node for the <mStat> production
    node *tree = create_node("<mStat>");

    //Invoke stat() mStat() or empty
    //Also check if the token id is an identifier for <assign>
    if((t0k.tok_string == "Keyword in") || (t0k.tok_string == "Keyword out") || (t0k.tok_string == "Operator {")
        || (t0k.tok_string == "Keyword iffy") || (t0k.tok_string == "Keyword loop") || (t0k.tok_string == "Keyword assign")
       || (t0k.tok_string == "Keyword goto") || (t0k.tok_string == "Keyword label")){

        tree->child_1 = stat();
        tree->child_2 = mStat();

        return tree;
    }
    else{
        //Empty production
        //cout << "****Empty MSTAT" << endl;
        tree->all_toks.push_back(EMPTY_TOKEN);
        return tree;
    }
}

//<stat> => <in>; | <out>; | <block> | <if>; |
//<loop>; | <assign>; | <goto>; | <label>;
node *stat(){

    //Create the node for the <stat> production
    node *tree = create_node("<stat>");

    if ((t0k.tok_string == "Keyword in")){

        //Set the child_1 of <stat> to the <in> production
        tree->child_1 = in();

        return tree;
    }
    else if ((t0k.tok_string == "Keyword out")){

        //Set the child_1 of <stat> to the <in> production
        tree->child_1 = out();

        return tree;
    }
    else if ((t0k.tok_string == "Operator {")){

        //Set the child_1 of <stat> to the <block> production
        tree->child_1 = block();

        return tree;
    }
    else if ((t0k.tok_string == "Keyword iffy")){
        scanner(t0k);

        //Set the child_1 of <stat> to the <if> production
        tree->child_1 = _if();
        return tree;
    }
    else if ((t0k.tok_string == "Keyword loop")){
      scanner(t0k);

      //Set the child_1 of <stat> to the <loop> production
      tree->child_1 = loop();
      return tree;
    }
    else if ((token_names[t0k.ID] == "Identifier")){

        Token temp_token = t0k;

        scanner(t0k);

        //Set the child_1 of <stat> to the <assign> production
        tree->child_1 = assign();

        //Push the identifier token into the tokens of the <assign> production.
        tree->child_1->all_toks.push_back(temp_token);
        return tree;
    }
    else if ((t0k.tok_string == "Keyword goto")){
        scanner(t0k);

        //Set the child_1 of <stat> to the <goto> production
        tree->child_1 = _goto();
        return tree;
    }
    else if ((t0k.tok_string == "Keyword label")){
        scanner(t0k);

        //Set the child_1 of <stat> to the <label> production
        tree->child_1 = label();
        return tree;
    }
    else if((t0k.tok_string == "Operator }")){
        scanner(t0k);
        return tree;
    }
  else
  {
    ex_tok.assign("Too Many different options");
    error();
  }
}

//<in> =>  in Identifier
node *in(){

    //Create the node for <in>
    node *tree = create_node("<in>");

    if ((t0k.tok_string == "Keyword in")){

        //Get the next token
        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            //If the token returned by the scanner is an identifier, store
            //the identifier in the <in> node vector of tokens.
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
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
        ex_tok.assign("in");
        error();
      }
}

//<out> => out <expr>
node *out(){

    //Create the node for the <out>
    node *tree = create_node("<out>");

    if((t0k.tok_string == "Keyword out")){
        //next token
        scanner(t0k);

        tree->child_1 = expr();

        if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
            scanner(t0k);
            return tree;
        }
        else{
            ex_tok.assign(";");
            error();
        }

    }else{
        ex_tok.assign("Keyword out");
        error();
    }
}

//<if> => iffy [ <expr> <RO> <expr>] then <stat>
node *_if(){
    //node for the <if>
    node *tree = create_node("<if>");

    //Check that the token returned by the scanner is a '[' operator
    if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator [")){
        //next token
        scanner(t0k);

        //Invoke <expr> <RO> <expr>. Set the nodes returned by these
        //productions to the child_1, child_2, and child_3 node(s) of
        //the <if> node, respectively.
        tree->child_1 = expr();
        tree->child_2 = RO();
        tree->child_3 = expr();

    //Check that the token returned by the scanner is a ']' operator
    if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ]")){
        //next token
        scanner(t0k);

        if((token_names[t0k.ID] == "Keyword") && (t0k.tok_string == "Keyword then")){

            //next token
            scanner(t0k);

            //Invoke <stat>. Set the child_4 node of the <if> production to
            //the node returned by <stat>.
            tree->child_4 = stat();

            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
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
    //node for the <loop>
    node *tree = create_node("<loop>");

    //Check that the token returned by the scanner is a '[' operator
    if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator [")){
        //next token
        scanner(t0k);

        //Invoke <expr> <RO> <expr>. Set the nodes returned by these
        //productions to the child_1, child_2, and child_3 node(s) of
        //the <if> node, respectively.
        tree->child_1 = expr();
        tree->child_2 = RO();
        tree->child_3 = expr();

        //Check that the token returned by the scanner is a ']' operator
        if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ]")){
            //next token
            scanner(t0k);

            //Invoke <stat>. Set the child_4 node of the <if> production to
            //the node returned by <stat>.
            tree->child_4 = stat();

            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
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
}//end loop

//<assign> => Identifier := <expr>
node *assign(){

    //cout << "In <assign> with " << t0k.tok_string << endl;
    //Create the node for the <assign> production.
    node *tree = create_node("<assign>");

    //Check that the token returned by the scanner is a ':=' operator
    if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator :=")){
        //next token
        scanner(t0k);

        //Invoke <expr>. Set the child_1 node of <assign>
        tree->child_1 = expr();

        //scanner(t0k);

        if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
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
    //Create the node for <label>
    node *tree = create_node("<label>");

    //Check if the token returned by the scanner is a '(' operator
    if ((t0k.tok_string == "Keyword label")){

        //Get the next token from the scanner
        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            //If the token returned by the scanner is an identifier, store
            //the identifier in the <in> node vector of tokens.
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
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
    //Create the node for <goto>
    node *tree = create_node("<goto>");

    //Check if the token returned by the scanner is a '(' operator
    if ((t0k.tok_string == "Keyword goto")){

        //Get the next token from the scanner
        scanner(t0k);

        if ((token_names[t0k.ID] == "Identifier")){
            //If the token returned by the scanner is an identifier, store
            //the identifier in the <in> node vector of tokens.
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator ;")){
                scanner(t0k);
                return tree;
            }
            else{
                ex_tok.assign(";");
                error();
            }
        }else{
          ex_tok.assign("goto");
          error();
        }
      }else{
        ex_tok.assign("Identifier");
        error();
      }
}

//<RO> => < | < < (two tokens >) | > | > > (two tokens) | == (one token==)
node *RO(){

    //Create the node for the <RO> production
    node *tree = create_node("<RO>");

        //Check that the token returned by the scanner is an operator
    if ((token_names[t0k.ID] == "Operator")){
        if((t0k.tok_string == "Operator <")){
            //Insert the '<' token into the <RO> node's tokens
            tree->all_toks.push_back(t0k);

            //Get the next token
            scanner(t0k);

            //If the token returned by the scanner is an operator, check that
            //it is a '<' operator.
            if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator <")){
                //Insert the '<' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

                //Get the next token from the scanner
                scanner(t0k);
                return tree;
            }
            else if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator >")){
                //Insert the '>' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

                //Get the next token from the scanner
                scanner(t0k);
                return tree;
            }
            else if((token_names[t0k.ID] == "Operator") && (t0k.tok_string != "Operator <") && (t0k.tok_string != "Operator >")){

                ex_tok.assign("< < or < >");
                error();
            }
            else{
                return tree;
            }
        }
        else if((t0k.tok_string == "Operator >")){
            //Insert the '>' token into the <RO> node's tokens
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            //If the token returned by the scanner is an operator, check
            //that it is the '>' operator
            if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator >")){
                //Insert the '>' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

                //Get the next token from the scanner
                scanner(t0k);
                return tree;
            }
            else if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator <")){
                //Insert the '=' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

//                if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator <")){
//                    //Insert the '=' token into the <RO> node's tokens
//                    tree->all_toks.push_back(t0k);
//
//                    //Get the next token from the scanner
//                    scanner(t0k);
//                    return tree;
//                }

                //Get the next token from the scanner
                scanner(t0k);
                return tree;
            }
            else{
                return tree;
            }
        }
        else if((t0k.tok_string == "Operator =")){
            //Insert the '=' token into the <RO> node's tokens
            tree->all_toks.push_back(t0k);

            //Get the next token from the scanner
            scanner(t0k);

            if((token_names[t0k.ID] == "Operator") && (t0k.tok_string == "Operator =")){
                //Insert the '=' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

                //Get the next token
                scanner(t0k);
                return tree;
            }

            //If the next token returned from the scanner is an operator,
            //check that it is an '=' operator
            if((token_names[t0k.ID] == "Operator") && (t0k.tok_string != "Operator >")){
                //Insert the '=' token into the <RO> node's tokens
                tree->all_toks.push_back(t0k);

                //Get the next token from the scanner
                scanner(t0k);
                return tree;
            }
            else if ((token_names[t0k.ID] == "Operator") && (t0k.tok_string != "Operator =")){
                ex_tok.assign("= =");
                error();
            }
            else
                return tree;
            }
            else{
                ex_tok.assign("< | < < | > | > > | == ");
                error();
            }
    }
    else{
        ex_tok.assign(" Operator < | > | =");
        error();
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
    //cout << "TOK in Error = " << t0k.tok_string << endl;
    cout << "ErRoR: Looking for " << ex_tok << endl;
}

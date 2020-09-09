#include "test_tree.h"
#include <string>
#include <iostream>

void print_preorder(node *tree, int level){

    string line;

    if (tree == NULL)
        return;

    for (int i = 0; i < level; i++){
        line.append("  ");
    }

    line.append(tree->literal);
    line.append(" ");

    //Append the tokens of a node to the output line.
    for (int i = 0; i < tree->all_toks.size(); i++){
        line.append(tree->all_toks[i].tok_string);

        //Separate multiple tokens on a node by a comma, as long
        //as the tokens are integers or identifiers.
        if( ((i + 1) != tree->all_toks.size()) &&
          (tree->all_toks[i].tok_string != "Operator")){

            line.append(",");
            line.append(" ");
        }
    }

  cout << line << "\n";

  //Print the children of the node, where the children go from
  //left to right; that is, child_1 is the leftmost child node, child_2
  //is the second leftmost child node, child_3 is the child node next to the
  //rightmost child node, and child_4 is the rightmost child node.
  print_preorder(tree->child_1, level + 1);
  print_preorder(tree->child_2, level + 1);
  print_preorder(tree->child_3, level + 1);
  print_preorder(tree->child_4, level + 1);
}

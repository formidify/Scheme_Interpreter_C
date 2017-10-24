#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
// when do you put <> and when "" ?
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
#include <string.h>
// got rid of main_tokenizer in head - may cause problems?

void addToParseTree(Value *tree, int *depth, Value *token){
	
}


/*
* Takes a linked list of tokens from a Scheme program (as in
* the output of your tokenizer), and it should return a pointer
* to a parse tree representing that program.
*/
Value *parse(Value *tokens){
   Value *tree = makeNull();
   int depth = 0;
   Value *current = tokens;
   assert(current != NULL && "Error (parse): null pointer");
   while (current->type != NULL_TYPE) {
      Value *token = car(current);
      tree = addToParseTree(tree, &depth, token);
      current = cdr(current);
   }
   if (depth != 0) {
      syntaxError();
   }
}


/*
* display a parse tree to the screen, using parentheses to denote
* tree structure. (In other words, the output of printTree should
* visually look identical to legal Scheme code.)
*/
void printTree(Value *tree){
	
}
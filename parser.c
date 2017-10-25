#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
#include <string.h>

Value *addToParseTree(Value *tree, int *depth, Value *token){
	if(token->type == CLOSE_TYPE){
		Value *newList = makeNull();
		Value *current = tree;

		while(current->type != NULL_TYPE){
			if(car(current)->type == OPEN_TYPE){
				tree = cdr(current);
				tree = cons(newList, tree);
				depth--;
				return tree;
			} else{
				newList = cons(car(current), newList);
				current = cdr(current);
			}
		}
		printf("Error (parse): too many close parens");
		texit(0);
	} else{
		if(token->type == OPEN_TYPE){
			depth++;
		}
		tree = cons(token, tree);
	}
	return tree;
}

void syntaxError(){
	printf("Syntax error");
	texit(0);
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
   return tree;
}


/*
* display a parse tree to the screen, using parentheses to denote
* tree structure. (In other words, the output of printTree should
* visually look identical to legal Scheme code.)
*/
void printTree(Value *tree){
}

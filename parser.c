#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
#include <string.h>

/*
* Takes in a pre-existing tree, a token to add to it, and a pointer
* to an integer depth, which is updated to represent the number of
* unclosed open parentheses in the parse tree. The parse tree is
* complete if (and only if) depth == 0 when the parse function returns.
*/

Value *addToParseTree(Value *tree, int *depth, Value *token){
	if(token->type == CLOSE_TYPE){
		Value *newList = makeNull();
		Value *current = tree;
		while(current->type != NULL_TYPE){
			if(car(current)->type == OPEN_TYPE){
				tree = cdr(current);
				tree = cons(newList, tree);
				(*depth)--;
				return tree;
			} else{
				newList = cons(car(current), newList);
				current = cdr(current);
			}
		}
		printf("Syntax error: too many close parentheses\n");
		texit(0);
	} else{
		if(token->type == OPEN_TYPE){
			(*depth)++;
		}
		tree = cons(token, tree);
	}
	return tree;
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
		printf("Syntax error: not enough close parentheses\n");
		texit(0);
	}
	return reverse(tree);
}

void printTreeConsType(Value *tree){
	if(car(tree)->type == CONS_TYPE){
		printf("(");
		printTree(car(tree));
		if(cdr(tree)->type == NULL_TYPE){
			printf(")");
		} else{
			printf(") ");
		}
		printTree(cdr(tree));
	} else {
		if (car(tree)->type == NULL_TYPE){
			printf("()");
			if(cdr(tree)->type == NULL_TYPE){
				printf("");
			} else{
				printf(" ");
			}
		}
		else{
			printTree(car(tree));
			if(cdr(tree)->type != NULL_TYPE){
				printf(" ");
			}
		}
		printTree(cdr(tree));
	}
}

void printTree(Value *tree){
	assert(tree != NULL);
	switch(tree->type) {
		case NULL_TYPE:
			break;
		case BOOL_TYPE:
			if(tree->b){
				printf("#t");
			} else{
				printf("#f");
			}
			break;
		case SYMBOL_TYPE:
			printf("%s", tree->s);
			break;
		case INT_TYPE:
			printf("%i", tree->i);
			break;
		case DOUBLE_TYPE:
			printf("%f", tree->d);
			break;
		case STR_TYPE:
			printf("\"%s\"", tree->s);
			break;
		case CONS_TYPE:
			printTreeConsType(tree);
			break;
		default:
			printf("Syntax error: unexpected value type in parse tree.\n");
			texit(0);
			break;
	}
}

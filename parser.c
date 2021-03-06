/*
* By Chae Kim, Tina Liu, James Yang
* A program that parses Scheme tokens and constructs a parse tree
* Valid tokens are boolean, integer, float, string, symbol, open, close
*/
#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
#include <string.h>

static bool quoteSugar = false;
static int quoteDepth = 0;

/*
* Prints a given error message and safely exits the program
*/
void syntaxError(char* errorMsg){
	printf("Syntax Error: %s\n", errorMsg);
	texit(0);
}

/*
* Handles parsing ' as syntactic sugar for quote
*/
Value *parseQuote(Value *tree, Value *token){
	if(quoteSugar){
		if(token->type == CLOSE_TYPE){
			quoteDepth--;
		}
		if(quoteDepth == 0){
			Value *quoteSymbol = makeNull();
			quoteSymbol->type = SYMBOL_TYPE;
			quoteSymbol->s = "quote";
			Value *quoteExpr = makeNull();
			quoteExpr = cons(car(tree), quoteExpr);
			quoteExpr = cons(quoteSymbol, quoteExpr);
			tree = cdr(tree);
			tree = cons(quoteExpr, tree);
			quoteSugar = false;
			quoteDepth = 0;
		}
	}
	return tree;
}

/*
* Takes in a pre-existing tree, a token to add to it, and a pointer
* to an integer depth, which is updated to represent the number of
* unclosed open parentheses in the parse tree. The parse tree is
* complete if (and only if) depth == 0 when the parse function returns.
*/
Value *addToParseTree(Value *tree, int *depth, Value *token){
	if(token->type == QUOTE_TYPE && (!quoteSugar)){
		quoteSugar = true;
		return tree;
	}
	if(token->type == CLOSE_TYPE){
		Value *newList = makeNull();
		Value *current = tree;
		while(current->type != NULL_TYPE){
			if(car(current)->type == OPEN_TYPE){
				tree = cdr(current);
				tree = cons(newList, tree);
				(*depth)--;
				return parseQuote(tree, token);
			} else{
				if(cdr(current)->type != NULL_TYPE
					&& car(cdr(current))->type == DOT_TYPE){
					Value *last = car(current);
					current = cdr(cdr(current));
					if(current->type != NULL_TYPE){
						newList = cons(car(current), last);
					} else{
						syntaxError("Invalid dotted pair.");
					}
				} else if(car(current)->type != DOT_TYPE){
					newList = cons(car(current), newList);
				} else{
					syntaxError("Invalid dotted pair.");
				}
				current = cdr(current);
			}
		}
		syntaxError("Syntax error: too many close parentheses.");
	} else{
		if(token->type == OPEN_TYPE){
			(*depth)++;
			if(quoteSugar){
				quoteDepth++;
			}
		}
		tree = cons(token, tree);
	}
	return parseQuote(tree, token);
}

/*
* Takes a linked list of tokens from a Scheme program (as in
* the output of tokenizer), and returns a pointer
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
		syntaxError("Syntax error: not enough close parentheses.");
	}
	return reverse(tree);
}

/*
* A helper method for printTree handling printing a value that is CONS_TYPE
*/
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

void printStr(char *s);

/*
* Prints a representation of a valid parse tree.
*/
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
			printStr(tree->s);
			break;
		case CONS_TYPE:
			printTreeConsType(tree);
			break;
		case QUOTE_TYPE:
			printf("'");
			break;
		default:
			syntaxError("Syntax error: unexpected value type in parse tree.");
			break;
	}
}

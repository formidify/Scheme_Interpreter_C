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
				// changed here to match Jed's image in instructions.
				if (*depth != 1){
					tree = cdr(current);
					tree = cons(newList, tree);
					depth--;
					return tree;
				}
				depth--;
				return newList;
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


void displayStrP(Value *list){
    int i = 0;
    printf("%c", '"');
    while (list->s[i] != '\0'){
        if (list->s[i] == '\n'){
            printf("\\n");
        }else if (list->s[i] == '\t'){
            printf("\\t");
        }else if (list->s[i] == '\''){
            // originally thought this part should be printf("\\'")
            // but when testing out on Dr.Racket input "\'" printed "'"
            // and so modified to follow Dr.Racket
            printf("\'");
        }else if (list->s[i] == '\"'){
            printf("\\\"");
        }else if (list->s[i] == '\\'){
            printf("\\\\");
        }else{
            printf("%c", list->s[i]);
        }
        i++;
    }
    printf("%c", '"');
//    printf(":string");
}


/*
* display a parse tree to the screen, using parentheses to denote
* tree structure. (In other words, the output of printTree should
* visually look identical to legal Scheme code.)
*/
void printTree(Value *tree){
    assert(tree != NULL);
	if(car(tree)->type == CONS_TYPE){
		printf("%s\n", "yes");
	}else{
		printf("%s\n", "no");
	}
	switch(car(tree)->type) {
		case BOOL_TYPE:
			if(car(tree)->b){
               	printf("#t");
           	} else{
				printf("#f");
			}
			break;
       	case SYMBOL_TYPE:
           	printf("%s", car(tree)->s);
           	break;
       	case INT_TYPE:
           	printf("%d", car(tree)->i);
           	break;
       	case DOUBLE_TYPE:
           	printf("%f", car(tree)->d);
           	break;
		case STR_TYPE:
			displayStrP(car(tree));
			break;
		case CONS_TYPE:
			printf("%s", "(");
			printTree(car(tree));
			if (cdr(tree)->type != NULL_TYPE){
				printTree(cdr(tree));
			}
			
//			gives *car(value *): Assertions '!isNull(list)' failed.
//			printTree(cdr(tree));
            break;
       	default:
           	printf("\n");
           	break;
	}
//	printf("%s", ")");
}
	//	printf("%s", "(");
//	if (car(tree)->type == CONS_TYPE){
//		printTree(car(tree));
//		if (cdr(tree)->type != NULL_TYPE){
//			printTree(cdr(tree));
//		}
//	} else if(car(tree)->type == BOOL_TYPE){
//		if(car(tree)->b){
//			printf("#t");
//		}else{
//			printf("#f");
//		}
//	}else if (car(tree)->type == INT_TYPE){
//		printf("%d", car(tree)->i);
//	}else if (car(tree)->type == DOUBLE_TYPE){
//		printf("%f", car(tree)->d);
//	}else if (car(tree)->type == STR_TYPE){
//		displayStrP(car(tree));
//	}else{
////		printf("\n");
//	}
//	printf("%s", ")");
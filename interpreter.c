/*
* By Chae Kim, Tina Liu, James Yang
* A Scheme interpreter
*/
#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
#include "interpreter.h"
#include <string.h>

/*
* Prints a given error message and safely exits the program
*/
void evaluationError(char *errorMsg){
    printf("Evaluation error: %s\n", errorMsg);
    texit(0);
}

/*
* Creates a new frame with the specified parent
* and an empty list of bindings
*/
Frame *makeNullFrame(Frame *parent){
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = parent;
    newFrame->bindings = makeNull();
    return newFrame;
}

/*
* Looks up symbol in the given frame, and if not found,
* continues searching the parents until the binding is found.
*/
Value *lookUpSymbol(Value *symbol, Frame *frame){
    if(frame == NULL){
        evaluationError("Binding for symbol not found.");
    }
    Value *current = frame->bindings;
    while(current->type != NULL_TYPE){
        Value *binding = car(current);
        if(!strcmp(symbol->s, car(binding)->s)){
            return cdr(binding);
        }
        current = cdr(current);
    }
    return lookUpSymbol(symbol, frame->parent);
}

/*
* Validates new variable is not already used as a binding
*/
void validateNewBinding(Value *var, Value *localBind){
    Value *current = localBind;
    while (current->type != NULL_TYPE){
        if (!strcmp(var->s, car(car(current))->s)){
            evaluationError("Duplicate identifier cannot be bound.");
        }
        current = cdr(current);
    }
}

/*
* Validates bindings are syntactically correct.
* Checks that each binding has only two elements AND the first
* element is an identifier
*/
void validateBindings(Value *bindings){
    Value *current = bindings;

    while (current->type != NULL_TYPE){
        Value *binding = car(current);
        if (binding->type != CONS_TYPE ||
            car(binding)->type != SYMBOL_TYPE ||
            cdr(binding)->type != CONS_TYPE ||
            cdr(cdr(binding))->type != NULL_TYPE){
            evaluationError("Invalid bindings.");
        }
        current = cdr(current);
    }
}

/*
* Evaluates the special form QUOTE
*/
Value *evalQuote(Value *args, Frame *frame){
    if(args->type == NULL_TYPE || cdr(args)->type != NULL_TYPE){
        evaluationError("QUOTE requires exactly one argument.");
    }
	return args;
}

/*
* Evaluates the special form IF
*/
Value *evalIf(Value *args, Frame *frame){
    //test if there are only two or three arguments
    if (args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE ||
       (cdr(cdr(args))->type == CONS_TYPE &&
        cdr(cdr(cdr(args)))->type == CONS_TYPE)){
        evaluationError("Incorrect number of arguments for IF.");
    }
    Value *test = eval(car(args), frame);
    if (test->type == BOOL_TYPE && test->b == false){
        if (cdr(cdr(args))->type == NULL_TYPE){
            Value *voidValue = makeNull();
            voidValue->type = VOID_TYPE;
            return voidValue;
        }
        return eval(car(cdr(cdr(args))), frame);
    }
    return eval(car(cdr(args)), frame);
}

/*
* Evaluates the special form LET
*/
Value *evalLet(Value *args, Frame *frame){
    //create new frame g with frame as parent
    Frame *g = makeNullFrame(frame);

    //get list of bindings and body from args
    Value *bindings = car(args);
    validateBindings(bindings);
    Value *body = cdr(args);
    if(body->type == NULL_TYPE){
        evaluationError("LET requires at least two arguments.");
    }

    //evaluate e_i...e_k in frame
    Value *current = bindings;
    while(current->type != NULL_TYPE){
        Value *binding = car(current);
        Value *variable = car(binding);
        validateNewBinding(variable, g->bindings);
        Value *result = eval(car(cdr(binding)), frame);
        //create new binding in g
        Value *newBinding = cons(variable, result);
        g->bindings = cons(newBinding, g->bindings);
        current = cdr(current);
    }

    //evaluate until the last expression of body in g and return the result
    Value *bodyN = car(body);
    while (cdr(body)->type != NULL_TYPE){
        eval(bodyN, g);
        body = cdr(body);
        bodyN = car(body);
    }
    return eval(bodyN, g);
}

/*
* Evaluates CONS_TYPE values from the parse tree
*/
Value *evalConsType(Value *tree, Frame *frame){
    Value *result;
    Value *first = car(tree);
    Value *args = cdr(tree);

    // Sanity and error checking on first...
    if(first == NULL || (first->type != SYMBOL_TYPE
        && first->type != CONS_TYPE)){
        evaluationError("Invalid syntax.");
    }

    if (!strcmp(first->s, "if")) {
        result = evalIf(args, frame);
    } else if(!strcmp(first->s, "quote")) {
        result = evalQuote(args, frame);
    } else if(!strcmp(first->s, "let")) {
        result = evalLet(args, frame);
    }
    // ... other special forms here ...
    else {
        // not a recognized special form
        evaluationError("Unrecognized special form.");
    }
    return result;
}

/*
* Takes a parse tree of a single S-expression and an environment frame
* in which to evaluate the expression, and returns a pointer to a Value
* representing the value
*/
Value *eval(Value *tree, Frame *frame){
    assert(tree != NULL);
    Value *result;
    switch (tree->type) {
        case INT_TYPE:
        case DOUBLE_TYPE:
        case BOOL_TYPE:
        case STR_TYPE:
            return tree;
            break;
        case SYMBOL_TYPE:
            return lookUpSymbol(tree, frame);
            break;
        case CONS_TYPE: {
            result = evalConsType(tree, frame);
            break;
        }
        default:
            break;
    }
    return result;
}

/*
* Prints the result of CONS_TYPE
*/
void printResult(Value *result);
void printEvalConsType(Value *result){
	if(car(result)->type == CONS_TYPE){
		printf("(");
		printResult(car(result));
        printf(")");
	} else {
		if (car(result)->type == NULL_TYPE){
			printf("()");
		}
		else{
			printResult(car(result));
		}
	}

    if(cdr(result)->type != NULL_TYPE){
        printf(" ");
    }
    printResult(cdr(result));
}

void printStr(char *s);
/*
* Prints the result of evaluating an expression
*/
void printResult(Value *result){
    assert(result != NULL);
    switch(result->type){
        case BOOL_TYPE:
            if (result->b) {
                printf("#t");
            }
            else {
                printf("#f");
            }
            break;
        case INT_TYPE:
            printf("%i", result->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", result->d);
            break;
        case STR_TYPE:
            //using printStr() from parser
            printStr(result->s);
            break;
		case CONS_TYPE:
			printEvalConsType(result);
			break;
        case SYMBOL_TYPE:
    		printf("%s", result->s);
    		break;
        default:
            break;
    }
}

/*
* Handles a list of S-expressions (as in a Scheme program),
* calls eval on each S-expression in the top-level (global) environment,
* and prints each result in turn
*/
void interpret(Value *tree){
    Frame *topFrame = NULL;
    assert(tree != NULL);
    Value *current = tree;
    while(current->type != NULL_TYPE){
        Value *result = eval(car(current), topFrame);
        printResult(result);
        if(result->type != VOID_TYPE){
            printf("\n");
        }
        current = cdr(current);
    }
}

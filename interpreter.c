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

static Frame *topFrame;

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
* If overwrite is false, validates that var does not already exist
* as a binding in localBind.
* Otherwise if overwrite is true, returns the Value of the binding for
* var if it already exists, otherwise return NULL
*/
Value *checkDuplicateBinding(Value *var, Value *localBind, bool overwrite){
    Value *current = localBind;
    while (current->type != NULL_TYPE){
        if (!strcmp(var->s, car(car(current))->s)){
            if(!overwrite){
                evaluationError("Duplicate identifier cannot be bound.");
            } else{
                return car(current);
            }
        }
        current = cdr(current);
    }
    return NULL;
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
* Evaluates a body of an expression, returning the result
* of the last expression in the body
*/
Value *evalBody(Value *body, Frame *frame){
    Value *bodyN = car(body);
    while (cdr(body)->type != NULL_TYPE){
        eval(bodyN, frame);
        body = cdr(body);
        bodyN = car(body);
    }
    return eval(bodyN, frame);
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
        checkDuplicateBinding(variable, g->bindings, false);
        Value *result = eval(car(cdr(binding)), frame);
        //create new binding in g
        Value *newBinding = cons(variable, result);
        g->bindings = cons(newBinding, g->bindings);
        current = cdr(current);
    }

    return evalBody(body, g);
}

/*
* Evaluates the special form DEFINE
*/
Value *evalDefine(Value *args, Frame *frame){
    Value *voidType = makeNull();
    voidType->type = VOID_TYPE;

    if(frame != topFrame){
        evaluationError("Define only supported at the top level.");
    }

    if (args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE ||
        cdr(cdr(args))->type != NULL_TYPE){
        evaluationError("Incorrect number of arguments for DEFINE.");
    }

    Value *var = car(args);
    Value *expr = car(cdr(args));

    if(var->type != SYMBOL_TYPE){
        evaluationError("Variable for DEFINE must be an identifier.");
    }

    Value *newBinding = checkDuplicateBinding(var, frame->bindings, true);
    Value *result = eval(expr, frame);

    if(newBinding == NULL){
        newBinding = cons(var, result);
        frame->bindings = cons(newBinding, frame->bindings);
    } else {
        *newBinding = *(cons(var, result));
    }

    return voidType;
}

Value *evalLambda(Value *args, Frame *frame){
    //also check if arguments and body are both cons_type
    Value *closureType = makeNull();
    closureType->type = CLOSURE_TYPE;
    Frame *g = makeNullFrame(frame);
    closureType->cl.fr = g;
    closureType->cl.fp = car(car(args));
    closureType->cl.bod = car(cdr(args));
if(cdr(args)->type == NULL_TYPE){
        evaluationError("LAMBDA requires at least two arguments.");
    }
    
    return closureType;
}

//closureType->cl.fr->type


// Value *evalClosureType(Value *tree, Frame *frame){
//     Value *lProcedure = car(tree);
//     Value *actualParam = cdr(tree);
    
//     Value *cur = lProcedure->cl.fp;
//     while(cur->type != NULL_TYPE){
//         Value *variable = eval(car(actualParam), frame);
//         Value *newBinding = cons(cur,variable);
//         lProcedure->cl.fr->bindings = cons(newBinding, lProcedure->cl.fr->bindings);
//         cur= cdr(cur);
//     }
    
//     return evalBody(lProcedure->cl.bod, lProcedure->cl.fr);
    // must work on if statements for...
    // if formal param is x
    // else if formal param is (x)
    // else if formal param is (x . y)
//}

Value *apply(Value *function, Value *args){
    Frame *g = makeNullFrame(function->cl.fr);
    Value *currentFormal = function->cl.fp;
    Value *currentArgs = args;

    //assume lambda formal arguments are not passed in a parenthesis
    while (currentFormal->type != NULL_TYPE || currentArgs->type != NULL_TYPE) {
        if (currentFormal->type == NULL_TYPE || currentArgs->type == NULL_TYPE){
            evaluationError("Number of formal and actual parameters are not the same.");
        }
        Value *formal = car(currentFormal);
        Value *arg = car(currentArgs);
        if (formal->type == SYMBOL_TYPE){
            checkDuplicateBinding(formal, g->bindings, false);
            Value *newBinding = cons(formal, arg);
            g->bindings = cons(newBinding, g->bindings);
        }
        else{
            evaluationError("Non-identifiers cannot be bound");
        }
        currentFormal = cdr(currentFormal);
        currentArgs = cdr(currentArgs);
    }
    Value *result = eval(function->cl.bod, g);
    return result;
}

//evaluate all the actual parameters
Value *evalCombinations(Value *args, Frame *frame){
    Value *returnArgs = makeNull();
    while (args->type != NULL_TYPE){
        Value *newArgs = eval(car(args), frame);
        returnArgs = cons(newArgs, returnArgs);
        args = cdr(args);
    }
    returnArgs = cons(args, returnArgs);
    return returnArgs;
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
        && first->type != CONS_TYPE && first->type != CLOSURE_TYPE)){
        evaluationError("Invalid syntax.");
    }
    if (first->type == CONS_TYPE){
        Value *newFirst = evalConsType(first, frame);
        Value *newTree = cons(newFirst, args);
        result = evalConsType(newTree, frame);
    } else if (first->type == CLOSURE_TYPE){
        Value *newArgs = evalCombinations(args, frame);
        result = apply(first, newArgs);
    } else if (!strcmp(first->s, "if")) {
        result = evalIf(args, frame);
    } else if(!strcmp(first->s, "quote")) {
        result = evalQuote(args, frame);
    } else if(!strcmp(first->s, "let")) {
        result = evalLet(args, frame);
    } else if(!strcmp(first->s, "define")) {
        result = evalDefine(args, frame);
    } else if(!strcmp(first->s, "lambda")){
        result = evalLambda(args, frame);
    } 
    // ... other special forms here ...
    else {
        // not a recognized special form
        Value *newSymbol = lookUpSymbol(first, frame);
        Value *newTree = cons(newSymbol, args);
        result = evalConsType(newTree, frame);
        //evaluationError("Unrecognized special form.");
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
    topFrame = makeNullFrame(NULL);
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

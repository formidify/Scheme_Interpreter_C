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

void evaluationError(){
    printf("Evaluation error \n");
    texit(0);
}

Frame *makeNullFrame(Frame *parent){
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = parent;
    newFrame->bindings = makeNull();
    return newFrame;
}

Value *lookUpSymbol(Value *symbol, Frame *frame){
    if(frame == NULL){
        evaluationError();
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

void isInFrame(Value *var, Value *localBind){
    while (localBind->type != NULL_TYPE){
        if (!strcmp(var->s, car(car(localBind))->s)){
            evaluationError();
        }
        localBind = cdr(localBind);
    }
}

void isCorrectBindings(Value *bindings){
    Value *current = bindings;
    //if it's an empty binding
    if (current->type == NULL_TYPE){
        evaluationError();
    }
    while (current->type != NULL_TYPE){
        Value *binding = car(current);
        if (binding->type != CONS_TYPE || 
            car(binding)->type != SYMBOL_TYPE ||
            cdr(binding)->type == NULL_TYPE ||
            cdr(cdr(binding))->type != NULL_TYPE){
            evaluationError();
        }
        current = cdr(current);
    }
}
Value *evalQuote(Value *args, Frame *frame){
    return NULL;
}

Value *evalIf(Value *args, Frame *frame){
    //test if there are exactly three arguments
    if (args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE ||
        cdr(cdr(args))->type != CONS_TYPE || 
        cdr(cdr(cdr(args)))->type != NULL_TYPE){
        evaluationError();
    }
    Value *test = eval(car(args), frame);
    if (test->type == BOOL_TYPE && test->b == false){
        return eval(car(cdr(cdr(args))), frame);
    }
    return eval(car(cdr(args)), frame);
}

Value *evalLet(Value *args, Frame *frame){
    //create new frame g with frame as parent
    Frame *g = makeNullFrame(frame);

    //get list of bindings and body from args
    Value *bindings = car(args);
    /*
    check if the bindings are syntactically correct because Scheme
    checks that each binding has only two elements AND the first 
    element is an identifier for all the bindings before moving
    on to evaluate expressions individually
    */
    isCorrectBindings(bindings);
    Value *body = car(cdr(args));
    Value *current = bindings;

    while(current->type != NULL_TYPE){
        //evaluate ei...ek in frame
        Value *binding = car(current);
        Value *variable = car(binding);
        //if variable already in frame, then exit with an error
        isInFrame(variable, g->bindings);
        Value *result = eval(car(cdr(binding)), frame);
        //create new binding in g
        Value *newBinding = cons(variable, result);
        g->bindings = cons(newBinding, g->bindings);
        current = cdr(current);
    }

    //evaluate body in g and return result
    return eval(body, g);
}

/*
* Takes a parse tree of a single S-expression and an environment frame
* in which to evaluate the expression, and returns a pointer to a Value
* representing the value
*/
Value *eval(Value *tree, Frame *frame){
    Value *result;
    switch (tree->type) {
        case INT_TYPE:
            return tree;
            break;
        case DOUBLE_TYPE:
            return tree;
            break;
        case BOOL_TYPE:
            return tree;
            break;
        case STR_TYPE:
            return tree;
            break;

        case SYMBOL_TYPE:
            return lookUpSymbol(tree, frame);
            break;
        case CONS_TYPE: {
            Value *first = car(tree);
            Value *args = cdr(tree);

            // Sanity and error checking on first...

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
                evaluationError();
            }
            break;
        }
        // ... other cases ...
        default:
            break;
    }
    return result;
}

void printResult(Value *result){
    switch(result->type){
        case BOOL_TYPE:
            if (result->b) {
                printf("#t\n");
            }
            else {printf("#f\n");}
            break;
        case INT_TYPE:
            printf("%i\n", result->i);
            break;
        case DOUBLE_TYPE:
            printf("%f\n", result->d);
            break;
        case STR_TYPE:
            printf("\"%s\"\n", result->s);
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
    assert(tree->type == CONS_TYPE);
    Value *current = tree;
    while(current->type != NULL_TYPE){
        Value *result = eval(car(current), topFrame);
        printResult(result);
        current = cdr(current);
    }
}

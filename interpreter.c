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

Value *lookUpSymbol(Value *tree, Frame *frame);

Value *evalQuote(Value *args, Frame *frame);

Value *evalIf(Value *args, Frame *frame);

Value *evalLet(Value *args, Frame *frame);

void evaluationError();

/*
* Takes a parse tree of a single S-expression and an environment frame
* in which to evaluate the expression, and returns a pointer to a Value
* representing the value
*/
Value *eval(Value *tree, Frame *frame){
    Value *result;
    switch (tree->type) {
        case INT_TYPE:
            //...do something
            break;
        case DOUBLE_TYPE:
            //...do something
            break;
        case BOOL_TYPE:
            //...do something
            break;
        case STR_TYPE:
            //...do something
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
        display(result);
        current = cdr(current);
    }
}

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
static int level;

/*
* Prints a given error message and safely exits the program
*/
void evaluationError(char *errorMsg){
    printf("Evaluation error: %s\n", errorMsg);
    texit(0);
}

/*
* Creates a VOID_TYPE value and returns a pointer to it
*/
Value *makeVoidValue(){
    Value *voidValue = makeNull();
    voidValue->type = VOID_TYPE;
    return voidValue;
}

/*
* Creates a new frame with the specified parent
* and an empty list of bindings
*/
Frame *makeFrame(Frame *parent){
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = parent;
    newFrame->bindings = makeNull();
    return newFrame;
}

/*
* Looks up symbol in the given frame, and if not found,
* continues searching the parents until the binding is found.
*/
Value *lookUpSymbol(Value *symbol, Frame *frame, bool giveError){
    if(frame == NULL){
        if(giveError){
            evaluationError("Binding for symbol not found.");
        } else{
            return NULL;
        }
    }
    Value *current = frame->bindings;
    while(current->type != NULL_TYPE){
        Value *binding = car(current);
        if(!strcmp(symbol->s, car(binding)->s)){
            return cdr(binding);
        }
        current = cdr(current);
    }
    return lookUpSymbol(symbol, frame->parent, giveError);
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
* Returns true if the expression is a definition, returns false otherwise.
*/
bool isDefinition(Value *expr){
    if(expr->type == CONS_TYPE && !strcmp(car(expr)->s, "define")){
        return true;
    }
    return false;
}

/*
* Evaluates a body of an expression, returning the result
* of the last expression in the body
*/
Value *evalDefine(Value *tree, Frame *frame, bool inBody);
Value *evalBody(Value *body, Frame *frame){
    Value *bodyN = car(body);
    while (cdr(body)->type != NULL_TYPE){
        if(isDefinition(bodyN)){
            evalDefine(cdr(bodyN), frame, true);
        } else {
            eval(bodyN, frame);
        }
        body = cdr(body);
        bodyN = car(body);
    }
    if(isDefinition(bodyN)){
        evaluationError("Definitions cannot be last in body.");
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
	return car(args);
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
            return makeVoidValue();
        }
        return eval(car(cdr(cdr(args))), frame);
    }
    return eval(car(cdr(args)), frame);
}

/*
* Validates syntax of LET, LETREC, LET*
*/
void validateLetTypeSyntax(Value *bindings, Value *body){
    validateBindings(bindings);
    if(body->type == NULL_TYPE){
        evaluationError("LET/LETREC/LET* requires at least two arguments.");
    }
}

/*
* Helper method to evaluate LET and LETREC
*/
Value *evalLetHelper(Value *args, Frame *frame, bool let){
    Value *bindings = car(args);
    Value *body = cdr(args);
    validateLetTypeSyntax(bindings, body);
    Frame *g = makeFrame(frame);

    //Set evalFrame depending on let, letrec, or let*
    Frame *evalFrame;
    if(let){
        evalFrame = frame;
    } else {
        evalFrame = g;
    }
    //evaluate e_i...e_k in frame
    Value *newBindings = makeNull();
    Value *current = bindings;
    while(current->type != NULL_TYPE){
        Value *binding = car(current);
        Value *variable = car(binding);
        checkDuplicateBinding(variable, newBindings, false);
        Value *result = eval(car(cdr(binding)), evalFrame);
        Value *newBinding = cons(variable, result);
        newBindings = cons(newBinding, newBindings);
        current = cdr(current);
    }
    g->bindings = newBindings;
    return evalBody(body, g);
}

/*
* Evaluates the special form LET
*/
Value *evalLet(Value *args, Frame *frame){
    return evalLetHelper(args, frame, true);
}

/*
* Evaluates the special form LETREC
*/
Value *evalLetrec(Value *args, Frame *frame){
    return evalLetHelper(args, frame, false);
}

/*
* Evaluates the special form LET*
*/
Value *evalLetstar(Value *args, Frame *frame){
    Value *bindings = car(args);
    Value *body = cdr(args);
    validateLetTypeSyntax(bindings, body);

    Frame *currentFrame = frame;
    Value *current = bindings;
    while(current->type != NULL_TYPE){
        Frame *g = makeFrame(currentFrame);
        Value *binding = car(current);
        Value *variable = car(binding);
        Value *result = eval(car(cdr(binding)), currentFrame);
        Value *newBinding = cons(variable, result);
        g->bindings = cons(newBinding, g->bindings);
        current = cdr(current);
        currentFrame = g;
    }

    return evalBody(body, currentFrame);
}

/*
* Evaluates the special form 'and'
*/
Value *evalAnd(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        Value *trueValue = makeNull();
        trueValue->type = BOOL_TYPE;
        trueValue->b = true;
        return trueValue;
    }
    while (cdr(args)->type != NULL_TYPE){
        Value *result = eval(car(args), frame);
        if (result->type == BOOL_TYPE && !(result->b)){
            return result;
        }
        args = cdr(args);
    }
    Value *result = eval(car(args), frame);
    return result;
}

/*
* Evaluates the special form 'or'
*/
Value *evalOr(Value *args, Frame *frame){
    if (args->type == NULL_TYPE){
        Value *trueValue = makeNull();
        trueValue->type = BOOL_TYPE;
        trueValue->b = false;
        return trueValue;
    }
    while (cdr(args)->type != NULL_TYPE){
        Value *result = eval(car(args), frame);
        if (result->type != BOOL_TYPE || result->b){
            return result;
        }
        args = cdr(args);
    }
    Value *result = eval(car(args), frame);
    return result;
}
/*
* Validates formals are identifiers
*/
void validateFormals(Value *args){
    Value *formal = args;
    while(formal->type != NULL_TYPE){
        if(car(formal)->type != SYMBOL_TYPE){
            evaluationError("Formal must be an identifier.");
        }
        formal = cdr(formal);
    }
}

/*
* Evaluates the special form LAMBDA
*/
Value *evalLambda(Value *args, Frame *frame){
    if (args->type != CONS_TYPE || cdr(args)->type == NULL_TYPE){
        evaluationError("Incorrect number of arguments for LAMBDA.");
    }
    validateFormals(car(args));
    Value *closureType = makeNull();
    closureType->type = CLOSURE_TYPE;
    closureType->cl.fr = frame;
    closureType->cl.fp = car(args);
    closureType->cl.bod = cdr(args);
    return closureType;
}

/*
* Confirms DEFINE has correct number of args and has been called from
* either the top level or a body
*/
void validateDefine(bool inBody, Value *args){
    if(!inBody && level != 1){
        evaluationError("Definitions allowed at top level or in body only.");
    }

    if (args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE ||
        cdr(cdr(args))->type != NULL_TYPE){
        evaluationError("Incorrect number of arguments for DEFINE.");
    }
}

/*
* Evaluates the special form DEFINE
*/
Value *evalDefine(Value *args, Frame *frame, bool inBody){
    validateDefine(inBody, args);
    Value *result = NULL;
    Value *voidType = makeVoidValue();

    Value *var = car(args);
    if(var->type == CONS_TYPE){
        Value *sugar = var;
        var = car(sugar);
        Value *formals = makeNull();
        Value *current = cdr(sugar);
        while(current->type != NULL_TYPE){
            formals = cons(car(current), formals);
            current = cdr(current);
        }
        result = evalLambda(cons(reverse(formals), cdr(args)), frame);
    }
    if(var->type != SYMBOL_TYPE){
        evaluationError("Cannot bind to non-identifier.");
    }
    Value *newBinding = checkDuplicateBinding(var, frame->bindings, true);
    if(result == NULL){
        result = eval(car(cdr(args)), frame);
    }

    if(newBinding == NULL){
        newBinding = cons(var, result);
        frame->bindings = cons(newBinding, frame->bindings);
    } else {
        *newBinding = *(cons(var, result));
    }
    return voidType;
}

/*
* Primitive function that sums any number of numbers
*/
Value *primitiveAdd(Value *args) {
    double sum = 0;
    Value *numberValue = makeNull();
    numberValue->type = INT_TYPE;
    Value *current = args;
    Value *val;
    while (current->type != NULL_TYPE) {
        val = car(current);
        if (val->type != INT_TYPE && val->type != DOUBLE_TYPE){
            evaluationError("+ only takes numbers as arguments.");
        }
        if (val->type == DOUBLE_TYPE){
            numberValue->type = DOUBLE_TYPE;
            sum = sum + val->d;
        }
        else{
            sum = sum + val->i;
        }
        current = cdr(current);
    }
    if (numberValue->type == DOUBLE_TYPE){
        numberValue->d = sum;
    }
    else{
        numberValue->i = (int) sum;
    }
    return numberValue;
}

/*
* Primitive function that multiplies any number of numbers
*/
Value *primitiveMult(Value *args) {
    double product = 1;
    Value *numberValue = makeNull();
    numberValue->type = INT_TYPE;
    Value *current = args;
    while (current->type != NULL_TYPE) {
        Value *val = car(current);
        if (val->type != INT_TYPE && val->type != DOUBLE_TYPE){
            evaluationError("* only takes numbers as arguments.");
        }
        if (val->type == DOUBLE_TYPE){
            numberValue->type = DOUBLE_TYPE;
            product = product * val->d;
        }
        else{
            product = product * val->i;
        }
        current = cdr(current);
    }
    if (numberValue->type == DOUBLE_TYPE){
        numberValue->d = product;
    }
    else{
        numberValue->i = (int) product;
    }
    return numberValue;
}

/*
* Primitive function that subtracts at least one number
*/
Value *primitiveSubtract(Value *args) {
    if(args->type == NULL_TYPE){
        evaluationError("- requires at least one argument.");
    }
    bool firstVal = true;
    Value *numberValue = makeNull();
    numberValue->type = INT_TYPE;
    Value *current = args;
    double diff = 0;
    while (current->type != NULL_TYPE) {
        Value *val = car(current);
        if (val->type != INT_TYPE && val->type != DOUBLE_TYPE){
            evaluationError("- only takes numbers as arguments.");
        }
        if(firstVal && cdr(current)->type != NULL_TYPE){
            diff = val->type == DOUBLE_TYPE ? val->d : val->i;
            firstVal = false;
        } else if (val->type == DOUBLE_TYPE){
            numberValue->type = DOUBLE_TYPE;
            diff = diff - val->d;
        } else{
            diff = diff - val->i;
        }
        current = cdr(current);
    }
    if (numberValue->type == DOUBLE_TYPE){
        numberValue->d = diff;
    }
    else{
        numberValue->i = (int) diff;
    }
    return numberValue;
}

/*
* Primitive function that divides at least one number
*/
Value *primitiveDivide(Value *args) {
    if(args->type == NULL_TYPE){
        evaluationError("/ requires at least one argument.");
    }
    bool firstVal = true;
    Value *numberValue = makeNull();
    numberValue->type = INT_TYPE;
    Value *current = args;
    double quot = 1;
    while (current->type != NULL_TYPE) {
        Value *val = car(current);
        if (val->type != INT_TYPE && val->type != DOUBLE_TYPE){
            evaluationError("/ only takes numbers as arguments.");
        }
        if(firstVal && cdr(current)->type != NULL_TYPE){
            quot = val->type == DOUBLE_TYPE ? val->d : val->i;
            firstVal = false;
        } else {
            double divider = val->type == DOUBLE_TYPE ? val->d : val->i;
            if(divider == 0){
                evaluationError("Cannot divide by 0.");
            }
            if(val->type == DOUBLE_TYPE){
                numberValue->type = DOUBLE_TYPE;
            }
            quot = quot / divider;
        }
        current = cdr(current);
    }
    if(numberValue->type == INT_TYPE && quot - (int) quot == 0){
        numberValue->i = quot;
    } else{
        numberValue->type = DOUBLE_TYPE;
        numberValue->d = quot;
    }
    return numberValue;
}

/*
* Primitive function that returns true if each value on the left is
* less than or equal to the value on its right
*/
Value *primitiveLessOrEqual(Value *args){
    if(args->type != CONS_TYPE || cdr(args)->type == NULL_TYPE){
        evaluationError("<= requires at least two arguments.");
    }

    Value *lessThanOrEq = makeNull();
    lessThanOrEq->type = BOOL_TYPE;
    lessThanOrEq->b = true;
    bool firstVal = true;
    Value *current = args;
    double left = 0;
    while(current->type != NULL_TYPE){
        Value *val = car(current);
        if (val->type != INT_TYPE && val->type != DOUBLE_TYPE){
            evaluationError("<= only takes numbers as arguments.");
        }
        double right = val->type == INT_TYPE ? val->i : val->d;
        if(!firstVal){
            if(left > right){
                lessThanOrEq->b = false;
            }
        } else{
            firstVal = false;
        }
        left = right;
        current = cdr(current);
    }
    return lessThanOrEq;
}

/*
* Primitive function that returns true if the list is null,
* returns false otherwise.
*/
Value *primitiveIsNull(Value *args) {
	if (args->type != CONS_TYPE || cdr(args)->type != NULL_TYPE){
		evaluationError("Incorrect number of arguments for null?.");
	}

	Value *nullAnswer = makeNull();
	nullAnswer->type = BOOL_TYPE;
	if (car(args)->type != NULL_TYPE){
		nullAnswer->b = false;
	}else{
		nullAnswer->b = true;
	}
	return nullAnswer;
}

/*
* Primitive function that returns true if two values are equal,
* returns false otherwise.
*/
Value *primitiveIsEqual(Value *args){
    if(args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE
        || cdr(cdr(args))->type != NULL_TYPE){
        evaluationError("Incorrect number of arguments for eq?.");
    }
    Value *isEqual = makeNull();
    isEqual->type = BOOL_TYPE;
    Value *val1 = car(args);
    Value *val2 = car(cdr(args));
    if(val1->type != val2->type){
        isEqual->b = false;
        return isEqual;
    }
    switch(val1->type){
        case BOOL_TYPE:
            isEqual->b = val1->b == val2->b;
            break;
        case INT_TYPE:
            isEqual->b = val1->i == val2->i;
            break;
        case DOUBLE_TYPE:
            isEqual->b = val1->d == val2->d;
            break;
        case STR_TYPE:
            isEqual->b = !strcmp(val1->s, val2->s);
            break;
        case CONS_TYPE:
            isEqual->b = &val1->c == &val2->c;
            break;
        case CLOSURE_TYPE:
            isEqual->b = &val1->cl == &val2->cl;
            break;
        case NULL_TYPE:
            isEqual->b = true;
            break;
        case SYMBOL_TYPE:
            isEqual->b = !strcmp(val1->s, val2->s);
            break;
        default:
            evaluationError("Unrecognized value type.");
    }
    return isEqual;
}

/*
* Primitive function that returns true value is a pair,
* returns false otherwise.
*/
Value *primitiveIsPair(Value *args){
    if(args->type != CONS_TYPE || cdr(args)->type != NULL_TYPE){
        evaluationError("pair? expects exactly one argument.");
    }

    Value *isPair = makeNull();
    isPair->type = BOOL_TYPE;
    isPair->b = car(args)->type == CONS_TYPE;
    return isPair;
}

/*
* Primitive function that returns the car of a cons type
*/
Value *primitiveCar(Value *args) {
	if (args->type != CONS_TYPE || cdr(args)->type != NULL_TYPE){
		evaluationError("Incorrect number of arguments for car.");
	} else if (car(args)->type != CONS_TYPE){
		evaluationError("Argument is not a cons type.");
	}
	return car(car(args));
}

/*
* Primitive function that returns the cdr of a cons type
*/
Value *primitiveCdr(Value *args) {
    if (args->type != CONS_TYPE || cdr(args)->type != NULL_TYPE){
		evaluationError("Incorrect number of arguments for cdr.");
	} else if (car(args)->type != CONS_TYPE){
		evaluationError("Argument is not a cons type.");
	}
	return cdr(car(args));
}

/*
* Primitive function that takes two values and returns a cons pair
*/
Value *primitiveCons(Value *args) {
    if(args->type != CONS_TYPE || cdr(args)->type != CONS_TYPE
        || cdr(cdr(args))->type != NULL_TYPE){
        evaluationError("Primitive cons requires two arguments.");
    }
    return cons(car(args), car(cdr(args)));
}

/*
* Applies the value of the first expression in a combination to the
* remaining values.
*/
Value *apply(Value *function, Value *args){
    if(function->type == PRIMITIVE_TYPE){
        return (function->pf)(args);
    }
    if(function->type != CLOSURE_TYPE){
        evaluationError("Cannot apply to non-procedure.");
    }
    Frame *g = makeFrame(function->cl.fr);
    Value *formals = function->cl.fp;
    Value *actuals = args;

    while (formals->type != NULL_TYPE && actuals->type != NULL_TYPE) {
        Value *formal = car(formals);
        Value *actual = car(actuals);
        if (formal->type == SYMBOL_TYPE){
            checkDuplicateBinding(formal, g->bindings, false);
            Value *newBinding = cons(formal, actual);
            g->bindings = cons(newBinding, g->bindings);
        }
        else{
            evaluationError("Non-identifiers cannot be bound");
        }
        formals = cdr(formals);
        actuals = cdr(actuals);
    }
    if (formals->type != NULL_TYPE || actuals->type != NULL_TYPE){
        evaluationError("Number of formal and actual params don't match.");
    }

    return evalBody(function->cl.bod, g);
}

/*
* Evaluates expressions in a combination
*/
Value *evalCombination(Value *first, Value *args, Frame *frame){
    Value *function = eval(first, frame);
    Value *actuals = makeNull();
    Value *current = args;
    while(current->type != NULL_TYPE){
        Value *actual = eval(car(current), frame);
        actuals = cons(actual, actuals);
        current = cdr(current);
    }
    return apply(function, reverse(actuals));
}

/*
* Evaluates CONS_TYPE values from the parse tree
*/
Value *evalConsType(Value *tree, Frame *frame){
    Value *result;
    Value *first = car(tree);
    Value *args = cdr(tree);

    // Sanity and error checking on first
    if(first == NULL || (first->type != SYMBOL_TYPE
        && first->type != CONS_TYPE && first->type != CLOSURE_TYPE)){
        evaluationError("Invalid syntax.");
    }

    if(first->type == SYMBOL_TYPE
        && lookUpSymbol(first, frame, false) != NULL){
        result = evalCombination(first, args, frame);
    }else if (!strcmp(first->s, "if")) {
        result = evalIf(args, frame);
    } else if(!strcmp(first->s, "quote")) {
        result = evalQuote(args, frame);
    } else if(!strcmp(first->s, "let")) {
        result = evalLet(args, frame);
    } else if(!strcmp(first->s, "define")) {
        result = evalDefine(args, frame, false);
    } else if(!strcmp(first->s, "lambda")){
        result = evalLambda(args, frame);
    } else if(!strcmp(first->s, "letrec")){
        result = evalLetrec(args, frame);
    } else if(!strcmp(first->s, "let*")){
        result = evalLetstar(args, frame);
    } else if(!strcmp(first->s, "and")){
        result = evalAnd(args, frame);
    } else if(!strcmp(first->s, "or")){
        result = evalOr(args, frame);
    } else {
        result = evalCombination(first, args, frame);
    }
    return result;
}

/*
* Takes a parse tree of a single S-expression and an environment frame
* in which to evaluate the expression, and returns a pointer to a Value
* representing the value
*/
Value *eval(Value *tree, Frame *frame){
    level++;
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
            return lookUpSymbol(tree, frame, true);
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
    printResult(car(result));
    if(cdr(result)->type != NULL_TYPE){
        printf(" ");
        if(cdr(result)->type != CONS_TYPE){
            printf(". ");
            printResult(cdr(result));
        } else {
            printEvalConsType(cdr(result));
        }
    }
}

/*
* Prints the result of BOOL_TYPE
*/
void printResult(Value *result);
void printBoolType(Value *result){
    if (result->b) {
        printf("#t");
    }
    else{
        printf("#f");
    }
}

/*
* Prints the result of evaluating an expression
*/
void printStr(char *s);
void printResult(Value *result){
    assert(result != NULL);
    switch(result->type){
        case BOOL_TYPE:
            printBoolType(result);
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
            printf("(");
			printEvalConsType(result);
            printf(")");
			break;
        case SYMBOL_TYPE:
    		printf("%s", result->s);
    		break;
        case CLOSURE_TYPE:
            printf("#<procedure>");
            break;
        case NULL_TYPE:
            printf("()");
            break;
        default:
            break;
    }
}

/*
* Binds a given primitive function to a specific variable name in
* the specified frame.
*/
void bind(char *name, Value *(*function)(Value *), Frame *frame) {
   Value *value = makeNull();
   value->type = PRIMITIVE_TYPE;
   value->pf = function;
   Value *var = makeNull();
   var->type = SYMBOL_TYPE;
   var->s = name;
   Value *binding = cons(var, value);
   frame->bindings = cons(binding, frame->bindings);
}

/*
* Handles a list of S-expressions (as in a Scheme program),
* calls eval on each S-expression in the top-level (global) environment,
* and prints each result in turn
*/
void interpret(Value *tree){
    assert(tree != NULL);
    topFrame = makeFrame(NULL);
    bind("+", primitiveAdd, topFrame);
    bind("-", primitiveSubtract, topFrame);
    bind("*", primitiveMult, topFrame);
    bind("/", primitiveDivide, topFrame);
    bind("<=", primitiveLessOrEqual, topFrame);
    bind("null?", primitiveIsNull, topFrame);
    bind("eq?", primitiveIsEqual, topFrame);
    bind("pair?", primitiveIsPair, topFrame);
    bind("car", primitiveCar, topFrame);
    bind("cdr", primitiveCdr, topFrame);
    bind("cons", primitiveCons, topFrame);
    Value *current = tree;
    while(current->type != NULL_TYPE){
        level = 0;
        Value *result = eval(car(current), topFrame);
        printResult(result);
        if(result->type != VOID_TYPE){
            printf("\n");
        }
        current = cdr(current);
    }
}

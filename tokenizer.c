#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include <assert.h>
#include "tokenizer.h"
#include "linkedlist.h"
#include <string.h>

// function should read stdin in its entireity
// and return a linked list consisting of all
// tokens found.
// https://www.cs.bu.edu/teaching/c/file-io/intro/
// boolean, integer, float, string, symbol, open, close

bool isDigit(char c){
    return (c == 0 || c == 1 || c == 2 ||
            c == 3 || c == 4 || c == 5 ||
            c == 6 || c == 7 || c == 8 ||
            c == 9);
}

bool isLetter(char c){
    return (c == 'a' || c == 'i' || c == 'q' ||
            c == 'b' || c == 'j' || c == 'r' ||
            c == 'c' || c == 'k' || c == 's' ||
            c == 'd' || c == 'l' || c == 't' ||
            c == 'e' || c == 'm' || c == 'u' ||
            c == 'f' || c == 'n' || c == 'v' ||
            c == 'g' || c == 'o' || c == 'w' ||
            c == 'h' || c == 'p' || c == 'x' ||
            c == 'y' || c == 'z' || c == 'A' ||
            c == 'B' || c == 'J' || c == 'R' ||
            c == 'C' || c == 'K' || c == 'S' ||
            c == 'D' || c == 'L' || c == 'T' ||
            c == 'E' || c == 'M' || c == 'U' ||
            c == 'F' || c == 'N' || c == 'V' ||
            c == 'G' || c == 'O' || c == 'W' ||
            c == 'H' || c == 'P' || c == 'X' ||
            c == 'I' || c == 'Q' || c == 'Y' ||
            c == 'Z');
}

bool isInitial(char c){
    return (isLetter(c) || c == '!' || c == '$' ||
            c == '%' || c == '&' || c == '*' ||
            c == '/' || c == ':' || c == '<' ||
            c == '=' || c == '>' || c == '?' ||
            c == '~' || c == '_' || c == '^');
}

bool isSubsequent(char c){
    return (isInitial(c) || isDigit(c) || c == '.' ||
            c == '+' || c == '-');
}
// open close bool have been implemented
// confused about what he said about list in
//  syntax detail
// confused about string escape characters
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
    charRead = fgetc(stdin);
    while (charRead != EOF){
        if (charRead == '('){
            Value *openType = makeNull();
            openType->type = OPEN_TYPE;
            list = cons(openType, list);
        } else if (charRead == ')'){
            Value *closeType = makeNull();
            closeType->type = CLOSE_TYPE;
            list = cons(closeType, list);
        } else if (charRead == '#'){
            charRead = fgetc(stdin);
            char temp = charRead;
            if (charRead == 't' || charRead == 'f') {
                charRead = fgetc(stdin);
                if (charRead == '(' || charRead == ')' ||
                    charRead == ' ') {
                    charRead = ungetc(charRead, stdin);
                    Value *boolType = makeNull();
                    boolType->type = BOOL_TYPE;
                    if (temp == 'f'){
                        boolType->b = false;
                    } else {
                        boolType->b = true;
                    }
                    list = cons(boolType, list);
                } else {
                   printf("Tokenization error: # is not a valid symbol.");
                    texit(0);
                }
            } else {
                printf("Tokenization error: # is not a valid symbol.");
                texit(0);
            }
        } else if(charRead == '+'){
            charRead = fgetc(stdin);
            if (charRead == '(' || charRead == ')' ||
                charRead == ' ') {
                charRead = ungetc(charRead, stdin);
                Value *symbolType = makeNull();
                symbolType->type = SYMBOL_TYPE;
                symbolType->s = "+";
                list = cons(symbolType, list);
            }
        } else if(charRead == '-'){
            charRead = fgetc(stdin);
            if (charRead == '(' || charRead == ')' ||
                charRead == ' ') {
                charRead = ungetc(charRead, stdin);
                Value *symbolType = makeNull();
                symbolType->type = SYMBOL_TYPE;
                symbolType->s = "-";
                list = cons(symbolType, list);
            }
        } else if(isInitial(charRead)){
            char *symbol = talloc(sizeof(char) * 2);
            symbol[0] = charRead;
            symbol[1] = '\0';
            charRead = fgetc(stdin);
            while(charRead != EOF && charRead != '(' &&
                charRead != ')' && charRead != ' '){
                    if(isSubsequent(charRead)){
                        size_t length = strlen(symbol);
                        char *tempSymbol = talloc(length + 2);
                        strcpy(tempSymbol, symbol);
                        tempSymbol[length] = charRead;
                        tempSymbol[length + 1] = '\0';
                        symbol = tempSymbol;
                        charRead = fgetc(stdin);
                    } else {
                        printf("Tokenization error.");
                        texit(0);
                    }
            }
            Value *symbolType = makeNull();
            symbolType->type = SYMBOL_TYPE;
            symbolType->s = symbol;
            list = cons(symbolType, list);
        } else{

        }
        charRead = fgetc(stdin);
    }
    return reverse(list); //see below
}


// The displayTokens function takes a linked
// list of tokens as input, and displays those
// tokens, one per line, with each token's type.
void displayTokens(Value *list){
    assert(list != NULL);
    switch(list->type) {
        case OPEN_TYPE:
            printf("%s:open", "(");
            break;
        case CLOSE_TYPE:
            printf("%s:close", ")");
            break;
        case BOOL_TYPE:
            if(list->b){
                printf("#t:boolean");
            } else{
                printf("#f:boolean");
            }
            break;
        case SYMBOL_TYPE:
            printf("%s:symbol", list->s);
            break;
        case CONS_TYPE:
            displayTokens(list->c.car);
            if(cdr(list)->type != NULL_TYPE){
                printf("\n");
            }
            displayTokens(list->c.cdr);
            break;
        default:
            printf("\n");
            break;
    }
}

#include <stdlib.h>
#include <stdin.h>
#include <stdio.h>
#include "value.h"
#include <assert.h>
#include "tokenizer.h"

// function should read stdin in its entireity
// and return a linked list consisting of all
// tokens found.
// https://www.cs.bu.edu/teaching/c/file-io/intro/
// boolean, integer, float, string, symbol, open, close

bool Digit?(char){
    return (char == 0 || char == 1 || char == 2 ||
            char == 3 || char == 4 || char == 5 ||
            char == 6 || char == 7 || char == 8 ||
            char == 9)
}

bool Letter?(char){
    return (char == "a" || char == "i" || char == "q" ||
            char == "b" || char == "j" || char == "r" ||
            char == "c" || char == "k" || char == "s" ||
            char == "d" || char == "l" || char == "t" ||
            char == "e" || char == "m" || char == "u" ||
            char == "f" || char == "n" || char == "v" ||
            char == "g" || char == "o" || char == "w" ||
            char == "h" || char == "p" || char == "x" ||
            char == "y" || char == "z" || char == "A" ||
            char == "B" || char == "J" || char == "R" ||
            char == "C" || char == "K" || char == "S" ||
            char == "D" || char == "L" || char == "T" ||
            char == "E" || char == "M" || char == "U" ||
            char == "F" || char == "N" || char == "V" ||
            char == "G" || char == "O" || char == "W" ||
            char == "H" || char == "P" || char == "X" ||
            char == "I" || char == "Q" || char == "Y" ||
            char == "Z")
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
        if (charRead == "("){
            Value *openType = makeNull();
            openType->type = OPEN_TYPE;
            list = cons(openType, list);
        }else if (charRead == ")"){
            Value *closeType = makeNull();
            closeType-> = CLOSE_TYPE;
            list = cons(openType, list);
        }else if (charRead == "#"){
            charRead = fgetc(stdin);
            temp = charRead;
            if (charRead == "t" || charRead == "f") {
                charRead = fgetc(stdin);
                if (charRead == "(" || charRead == ")" ||
                    charRead == " ") {
                    charRead = ungetc(stdin);
                    Value *boolType = makeNull();
                    boolType->type = BOOL_TYPE;
                    if (temp == "f"){
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
        }else if (charRead ){
            
        }else if ()
        }else{
            
        }
        charRead = fgetc(stdin);
    }
    return reverse(list); //see below
}


// The displayTokens function takes a linked
// list of tokens as input, and displays those 
// tokens, one per line, with each token's type.
void displayTokens(Value *list);
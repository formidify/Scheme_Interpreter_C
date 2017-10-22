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

Value *isNumber(Value *list, bool didSign, bool didDot){
    int resultNumber;
    char numberString[];
    int i;
    if (didSign) {
        numberString[i] = charRead;
        charRead = fgetc(stdin);
        i ++;
    }
    //since dot must be followed by an integer
    if (didDot) {
        lookAhead = fgetc(stdin);
        if (!isDigit){
            printf("Tokenization error: dot must be followed by a digit.");
            texit(0);
        }
        ungetc(lookAhead, stdin);
    }
    charRead = fgetc(stdin);
    while (charRead != EOF && charRead != '(' &&
            charRead != '(' && charRead != ' ') {
            if ((charRead == '+' || charRead == '-')
            || (didDot && charRead == '.')
            || (isInitial(charRead))) {
                printf("Tokenization error: numbers cannot be parsed with symbols.");
                texit(0);
            }
            else if (charRead == '.'){
                numberString[i] = charRead;
                i ++;
                isDot = true;
                lookAhead = fgetc(stdin);
                if (!isDigit){
                    printf("Tokenization error: dot must be followed by a digit.");
                    texit(0);
                }
                ungetc(lookAhead, stdin);
            }
            else if(isDigit(charRead)){
                numberString[i] = charRead;
                i ++;
            }
            charRead = fgetc(stlin);

        }
        ungetc(charRead, stdin); //not sure if can ungetc an EOF
        Value *numberType = makeNull();
        if (didDot){
            resultNumber = sscanf(numberString, "%f")
            numberType->type = DOUBLE_TYPE;
            numberType->d = resultNumber;
        }
        else{
            resultNumber = sscanf(numberString, "%d")
            numberType->type = INT_TYPE;
            numberType->i = resultNumber;
        }
        list = cons(numberType, list);
        return list;
}

// open close bool have been implemented
// confused about what he said about list in
//  syntax detail
// confused about string escape characters
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
	bool init = false;
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
            char sign = charRead;
            charRead = fgetc(stdin);
            if (charRead == '(' || charRead == ')' ||
                charRead == ' ') {
                charRead = ungetc(charRead, stdin);
                Value *symbolType = makeNull();
                symbolType->type = SYMBOL_TYPE;
                symbolType->s = "+";
                list = cons(symbolType, list);
            }
            else {
                ungetc(charRead, stdin);
                ungetc(sign, stdin); //pushes it back so it's readable next time
                isNumber(list, true, false);
            }

        }else if(isDigit(charRead)){
            int integer = charRead; //not sure if can be declared this way
            charRead = fgetc(stdin);
            if (charRead == '(' || charRead == ')' ||
                charRead == ' ') {
                charRead = ungetc(charRead, stdin);
                Value *intType = makeNull();
                intType->type = INT_TYPE;
                intType->i = integer;
                list = cons(intType, list);
            }
            else{
                ungetc(charRead, stdin); //do we need to assign ungetc() to something?
                ungetc(integer, stdin); //place the number back into the stack
                isNumber(list, false, false)
            }
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
            else {
                ungetc(charRead, stdin);
                ungetc(sign, stdin);
                isNumber(list, true, false);
            }

        } else if(charRead == "."){
            charRead = fgetc(stdin);
            if (isDigit(charRead)) {
                charRead = ungetc(charRead, stdin);
                isNumber(list, false, true);
            }
            else{
                printf("Tokenization error: '.' cannot be followed by non-digits.")
                texit(0);
            }
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
	//changes made by Chae. No other changes except bool init = false;
	// in beginning of function
        } else if(charRead == '"' && init == false){
			init = true;
			//char *string;
			int acc = 1;
			long int cur = ftell(stdin);
			charRead = fgetc(stdin);
			while (charRead != EOF || (charRead != '"' && init == true)){
				acc += 1;
				charRead = fgetc(stdin);
			}
			if (charRead == EOF){
				printf("Err: Quotations do not pair up");
				texit(0);
			}else if(charRead == '"' && init == true){
				fseek(stdin, cur, 0);
				char *string[acc + 2];
				for (int i = 0; i < acc + 1; i++){
					string[i] = fgetc(stdin);
				}
				string[i+1] = '\0';
				init = false;
				Value *strType = makeNull();
				strType->type = STR_TYPE;
				strType->s = string;
				list = cons(strType, list);
			}
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

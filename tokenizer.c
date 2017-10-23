#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "vector.h"
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
    return (c == '0' || c == '1' || c == '2' ||
    c == '3' || c == '4' || c == '5' ||
    c == '6' || c == '7' || c == '8' ||
    c == '9');
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

Value *tokenizeNumber(char charRead, bool hasSign, bool hasDot){
    Vector *number = talloc(sizeof(Vector));
    init(number, 1);
    add(number, 0, '\0');

    if(hasSign){
        add(number, number->size - 1, charRead);
        charRead = fgetc(stdin);
    }

    if(hasDot){
        char lookAhead = fgetc(stdin);
        if(!isDigit(lookAhead)){
            printf("Tokenization error: dot must be followed by a digit.\n");
            texit(0);
        }
        ungetc(lookAhead, stdin);
        add(number, number->size - 1, charRead);
        charRead = fgetc(stdin);
    }

    while(charRead != EOF && charRead != '(' &&
        charRead != ')' && charRead != ' ' && charRead !='\n'){
        if(isDigit(charRead)){
            add(number, number->size - 1, charRead);
            charRead = fgetc(stdin);
        } else if(charRead == '.' && !hasDot){
            add(number, number->size - 1, charRead);
            hasDot = true;
            charRead = fgetc(stdin);
        }else {
            printf("Tokenization error, unrecognized symbol in number.\n");
            texit(0);
        }
    }

    ungetc(charRead, stdin);
    Value *numberType = makeNull();

    if(hasDot){
        numberType->type = DOUBLE_TYPE;
        numberType->d = atof(number->data);
    } else{
        numberType->type = INT_TYPE;
        numberType->i = atoi(number->data);
    }
    return numberType;
}

Value *tokenizeSymbol(char charRead){
    Vector *symbol = talloc(sizeof(Vector));
    init(symbol, 2);
    add(symbol, 0, charRead);
    add(symbol, 1, '\0');
    charRead = fgetc(stdin);
    while(charRead != EOF && charRead != '(' &&
        charRead != ')' && charRead != ' ' && charRead != '\n'){
        if(isSubsequent(charRead)){
            add(symbol, symbol->size - 1, charRead);
            charRead = fgetc(stdin);
        } else {
            printf("Tokenization error, invalid symbol.\n");
            texit(0);
        }
    }
    ungetc(charRead, stdin);
    Value *symbolType = makeNull();
    symbolType->type = SYMBOL_TYPE;
    symbolType->s = symbol->data;
    return symbolType;
}

Value *tokenizeBoolean(){
    Value *boolType;
    char charRead = fgetc(stdin);
    if (charRead == 't' || charRead == 'f') {
        char lookAhead = fgetc(stdin);
        if (lookAhead == '(' || lookAhead == ')' ||
            lookAhead == ' ' || lookAhead == EOF || lookAhead == '\n') {
            ungetc(lookAhead, stdin);
            boolType = makeNull();
            boolType->type = BOOL_TYPE;
            if (charRead == 'f'){
                boolType->b = false;
            } else {
                boolType->b = true;
            }
        } else {
            printf("Tokenization error: # is not a valid symbol.\n");
            texit(0);
        }
    } else {
        printf("Tokenization error: # is not a valid symbol.\n");
        texit(0);
    }
    return boolType;
}

Value *tokenizeSign(char charRead){
    Value *token;
    char lookAhead = fgetc(stdin);
    if (lookAhead == EOF || lookAhead == '('
        || lookAhead == ')' || lookAhead == ' ' || lookAhead == '\n') {
        ungetc(lookAhead, stdin);
        token = makeNull();
        token->type = SYMBOL_TYPE;
        if(charRead == '-'){
            token->s = "-";
        } else {
            token->s = "+";
        }
        return token;
    } else if(isDigit(lookAhead) || lookAhead == '.'){
        bool isDot = (lookAhead == '.');
        ungetc(lookAhead, stdin);
        token = tokenizeNumber(charRead, true, isDot);
        return token;
    } else {
        printf("Tokenization error, - or + not symbol or number.\n");
        texit(0);
        return token;
    }
}

Value *tokenizeString(){
    Vector *string = talloc(sizeof(Vector));
    init(string, 1);
    add(string, 0, '\0');
    char charRead = fgetc(stdin);
    while(charRead != '"'){
        if(charRead == '\\'){
            charRead = fgetc(stdin);
            if(charRead == 'n'){
                add(string, string->size - 1, '\n');
            } else if(charRead == 't'){
                add(string, string->size - 1, '\t');
            } else if(charRead == '\\'){
                add(string, string->size - 1, '\\');
            } else if(charRead == '\''){
                add(string, string->size - 1, '\'');
            } else if(charRead == '"'){
                add(string, string->size - 1, '"');
            } else{
                printf("Tokenization error, unallowed escape.\n");
                texit(0);
            }
        } else if(charRead == EOF){
            printf("Tokenization error, missing second \".\n");
            texit(0);
        } else{
            add(string, string->size - 1, charRead);
        }
        charRead = fgetc(stdin);
    }
    Value *strType = makeNull();
    strType->type = STR_TYPE;
    strType->s = string->data;
    return strType;
}

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
            Value *boolType = tokenizeBoolean();
            list = cons(boolType, list);
        } else if(charRead == '-' || charRead == '+'){
            Value *withSign = tokenizeSign(charRead);
            list = cons(withSign, list);
        } else if(charRead == '.'){
            Value *numberType = tokenizeNumber(charRead, false, true);
            list = cons(numberType, list);
        } else if(isDigit(charRead)){
            Value *numberType = tokenizeNumber(charRead, false, false);
            list = cons(numberType, list);
        } else if(isInitial(charRead)){
            Value *symbolType = tokenizeSymbol(charRead);
            list = cons(symbolType, list);
        } else if(charRead == '"'){
            Value *strType = tokenizeString();
            list = cons(strType, list);
        } else if(charRead == ';'){
            charRead = fgetc(stdin);
            while(charRead != '\n' && charRead != EOF){
                charRead = fgetc(stdin);
            }
        } else if(charRead != ' ' && charRead != EOF && charRead != '\n'){
            printf("Tokenization error, unknown symbol:%c\n", charRead);
            texit(0);
        }
        charRead = fgetc(stdin);
    }
    return reverse(list); //see below
}


void displayStr(Value *list){
	int i = 0;
	printf("%c", '"');
	while (list->s[i] != '\0'){
		if (list->s[i] == '\n'){
			printf("\\n");
		}else if (list->s[i] == '\t'){
			printf("\\t");
		}else if (list->s[i] == '\''){
			printf("\\'");
		}else if (list->s[i] == '\"'){
			printf("\\'");
		}else if (list->s[i] == '\\'){
			printf("\\\\");
		}else{
			printf("%c", list->s[i]);	
		}
		i++;
	}
	printf("%c", '"');
	printf(":string");
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
        case INT_TYPE:
            printf("%d:integer", list->i);
            break;
        case DOUBLE_TYPE:
            printf("%f:float", list->d);
            break;
        case CONS_TYPE:
            displayTokens(list->c.car);
            if(cdr(list)->type != NULL_TYPE){
                printf("\n");
            }
            displayTokens(list->c.cdr);
            break;
		case STR_TYPE:
			displayStr(list);
			break;
        default:
            printf("\n");
            break;
    }
}

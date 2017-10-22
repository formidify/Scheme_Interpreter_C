#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "tokenizer.h"
#include "linkedlist.h"

int main(void) {
   Value *list = tokenize();
   displayTokens(list);
   tfree();
   return 0;
}

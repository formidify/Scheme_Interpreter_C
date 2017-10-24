// delete unnecessary head files?
// literally just copy and pasted.

#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "tokenizer.h"
#include "linkedlist.h"
#include "parser.h"
// what is string.h for?
#include <string.h>
#include "vector.h"

int main(void) {
   Value *list = tokenize();
   Value *tree = parse(list);
   printTree(tree);
   printf("\n");
   tfree();
   return 0;
}
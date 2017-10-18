#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"

int main() {
   Value *val1 = talloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 12;

   Value *val2 = talloc(sizeof(Value));
   val2->type = DOUBLE_TYPE;
   val2->d = 4.3;

   Value *head = makeNull();
   head = cons(val1, head);
   head = cons(val2, head);

   Value *val3 = talloc(sizeof(Value));
   val3->type = PTR_TYPE;
   val3->p = val2;

   display(val3);

   tfree();
   printf("I can see this.\n");
   Value *val5 = talloc(sizeof(Value));
   texit(1);
   printf("I should never see this.\n");
}

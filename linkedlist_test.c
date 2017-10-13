#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"

int main() {
   Value *val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 12;

   Value *val2 = malloc(sizeof(Value));
   val2->type = STR_TYPE;
   val2->s = "stringgg";
    
    Value *val3 = malloc(sizeof(Value));
   val3->type = STR_TYPE;
   val3->s = "stringgg3";
    
    Value *val4 = malloc(sizeof(Value));
   val4->type = STR_TYPE;
   val4->s = "stringgg4";
    
//Value *empty = makeNull();
    Value *val5 = malloc(sizeof(Value));
   val5 = cons(val3,val4);
   val5->type = CONS_TYPE;

   Value *head = makeNull();
   head = cons(val1, val2);
   head = cons(val2, head);
   head = cons(val3, head);
    head = cons(val5, head);

//Value *reversehead = reverse(head);
display(head);
//printf("%i \n", length(head));
   //display(reversehead);
  //cleanup(head);
  
}

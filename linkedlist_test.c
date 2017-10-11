#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"

int main() {
   Value *val1 = malloc(sizeof(Value));
   /*val1->type = NULL_TYPE;
   printf("%d\n", isNull(val1));
   Value *val2 = malloc(sizeof(Value));
   val2->type = INT_TYPE;
   printf("%d\n", isNull(val2));*/
   val1->type = INT_TYPE;
   val1->i = 12;
   //printf("%d\n", length(val1));

   Value *val2 = malloc(sizeof(Value));
   val2->type = DOUBLE_TYPE;
   val2->d = 4.3;

   Value *head = makeNull();
   printf("%i \n", length(head));
   head = cons(val1, head);
   head = cons(val2, head);
   printf("%i \n", length(head));
   length(head);

   display(head);
   Value *reversed = reverse(head);
   display(reversed);

   printf("Length = %i\n", length(head));
   printf("Empty? %i\n", isNull(head));
   cleanup(reversed);
   cleanup(head);

   /*
   James
   */

   /*
   Tina
   */

   /*
   Chae
   */
}

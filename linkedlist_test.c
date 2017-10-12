#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"

int main() {
   Value *head = makeNull();
   printf("Expected length 0, Actual length: %i \n", length(head));
   /*val1->type = NULL_TYPE;
   printf("%d\n", isNull(val1));
   Value *val2 = malloc(sizeof(Value));
   val2->type = INT_TYPE;
   printf("%d\n", isNull(val2));*/
   //printf("%d\n", length(val1));

   /*
   James
   */

   /*
   Tina
   */
   // Value *list = NULL;
   // isNull(list);
   // reverse(list);
   // cleanup(list);

   Value *val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 12;
   Value *val2 = malloc(sizeof(Value));
   val2->type = DOUBLE_TYPE;
   val2->d = 4.3;
   Value *val3 = malloc(sizeof(Value));
   val3->type = STR_TYPE;
   val3->s = "hello";

   head = cons(val1, head);
   Value *reversed = reverse(head);
   display(reversed);
   head = cons(val2, head);
   printf("Expected length 2, Actual length %i \n", length(head));
   display(head);
   head = cons(val3, head);
   printf("Expected length 3, Actual length %i \n", length(head));
   display(head);;
   reversed = reverse(head);
   display(reversed);
   printf("Expected length 3, Actual length %i\n", length(head));
   printf("Empty? Expect false (0), Actual: %i\n", isNull(head));
   head->c.car->s = "bye";
   head->c.cdr->c.car->d = 3.14;
   display(head);
   display(reversed);
   cleanup(reversed);
   cleanup(head);
   /*
   Chae
   */
}

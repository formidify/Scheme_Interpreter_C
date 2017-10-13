#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"

int main() {
   //Test empty list
   Value *head = makeNull();
   printf("Expected length 0, Actual length: %i \n", length(head));
   printf("Empty? Expect true (1), Actual: %i\n", isNull(head));
   printf("Empty list: ");
   display(head);
   Value *reversed = reverse(head);
   printf("Reversed empty list: ");
   display(reversed);
   cleanup(reversed);
   printf("=============Cleaned up reversed.============\n");

   //Check NULL value doesn't throw segmentation error - uses assert
   //Uncomment one at a time to test:
   // head = NULL;
   // car(head);
   // cdr(head);
   // isNull(head);
   // length(head);
   // reverse(head);
   // cleanup(head);

   //Create values
   Value *val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 12;
   Value *val2 = malloc(sizeof(Value));
   val2->type = DOUBLE_TYPE;
   val2->d = 4.3;
   Value *val3 = malloc(sizeof(Value));
   val3->type = STR_TYPE;
   val3->s = "hello!";

   //Create a proper list
   head = cons(val1, head);
   head = cons(val2, head);
   head = cons(val3, head);

   printf("Expected length 3, Actual length %i \n", length(head));
   printf("Head: ");
   display(head);
   reversed = reverse(head);
   printf("Reversed: ");
   display(reversed);
   printf("Expected length 3, Actual length %i\n", length(reversed));
   printf("Empty? Expect false (0), Actual: %i\n", isNull(head));

   //Modify head - reverse should not be modified
   head->c.car->s = "bye";
   head->c.cdr->c.car->d = 3.14;
   printf("Modified head: ");
   display(head);
   printf("Original reversed: ");
   display(reversed);
   // printf("head car: ");
   // display(car(head));
   // printf("\n");
   printf("head cdr: ");
   display(cdr(head));
   cleanup(reversed);
   cleanup(head);
   printf("========Cleaned up reversed and head=========\n");

   /*
   //Test Display - uncommented because cleanup cannot free nested lists
   head = makeNull();
   Value *headTwo = makeNull();
   val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 12;
   val2 = malloc(sizeof(Value));
   val2->type = DOUBLE_TYPE;
   val2->d = 4.3;

   head = cons(val1, head);
   head = cons(val2, head);
   headTwo = cons(head, headTwo);
   display(headTwo);

   printf("============Cleaned up headTwo=========\n");
   */

   // val1 = malloc(sizeof(Value));
   // val1->type = INT_TYPE;
   // val1->i = 12;
   // val2 = malloc(sizeof(Value));
   // val2->type = DOUBLE_TYPE;
   // val2->d = 4.3;
   // head = makeNull();
   // printf("Length = %i \n", length(head));
   // head = cons(val1, head);
   // printf("Length = %i \n", length(head));
   // head = cons(val2, head);
   // printf("Length = %i \n", length(head));
	//
	// Value *sample1 = makeNull();
	// Value *sample2 = makeNull();
	// Value *sample3 = makeNull();
	// Value *sample4 = makeNull();
	// Value *sample5 = makeNull();
	// Value *sample6 = makeNull();
	// Value *sample7 = makeNull();
	// Value *empty = makeNull();
	//
	// // nested here right?
	// // cons at least compiles nested
	// sample1 = cons(head, empty);
	// display(sample1);
	// printf("Length of sample1 = %i\n", length(sample1));
	// printf("************************\n");
	// // length still works for nested
	// // display does not work for nested but instead shows empty lines
	//
	// sample2 = cons(val1, val2);
	// /*
	// display(sample2);
	// printf("Length of sample2 = %i\n", length(sample2));
	// // length and display do not work for improper list
	// // doesn't throw error, just ignores
	// // error here just exits the whole program??
	// */
	//
	// sample3 = cons(empty, head);
	// display(sample3);
	// printf("Length of sample3 = %i\n", length(sample3));
	// printf("************************\n");
	// // add \n to end of "ERR: ILL FORMED.." in display
	// // gives length despite ill-formed list
	//
	// sample4 = cons(car(sample2), car(sample1));
	// display(sample4);
	// printf("Length of sample4 = %i\n", length(sample4));
	// printf("************************\n");
	//
	// sample5 = cons(cdr(sample2), car(sample1));
	// display(sample5);
	// printf("Length of sample5 = %i\n", length(sample5));
	// printf("************************\n");
	//
	// sample6 = cons(cdr(sample2), cdr(head));
	// display(sample6);
	// printf("Length of sample6 = %i\n", length(sample6));
	// printf("************************\n");
	//
	// /*
	// sample7 = cons(car(empty), cdr(head));
	// display(sample7);
	// printf("Length of sample7 = %i\n", length(sample7));
	// printf("************************\n");
	// // gives proper error aborts
	// */
	//
	// //everything works fine except length when dealing with improper.
	//
	// cleanup(sample1);
   // free(val1);
   // free(val2);
   // free(sample2);
	// cleanup(sample3);
	// cleanup(sample4);
	// cleanup(sample5);
	// cleanup(sample6);
	// cleanup(sample7);
	// cleanup(empty);
}

#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"
#include <string.h>
#include <assert.h>

/*
* Create an empty list (a new Value object of type NULL_TYPE).
*/
Value *makeNull(){
   Value *list = malloc(sizeof(Value));
   if(!list){
      printf("Out of memory");
   }
   list->type= NULL_TYPE;
   return list;
}

/*
* Create a nonempty list (a new Value object of type CONS_TYPE).
*/
Value *cons(Value *car, Value *cdr) {
   Value *list = makeNull();
   list->type = CONS_TYPE;
   list->c.car = car;
   list->c.cdr = cdr;
   return list;
}

/*
* Print a representation of the contents of a linked list.
*/
void display(Value *list){
   if (list->type == NULL_TYPE) {
      printf("\n");
   }
   else {
      switch (list->c.car->type) {
         case INT_TYPE:
            printf("%i ", list->c.car->i);
            break;
         case DOUBLE_TYPE:
            printf("%f ", list->c.car->d);
            break;
         case STR_TYPE:
            printf("%s ", list->c.car->s);
            break;
		 case CONS_TYPE:
            display(list->c.cdr);
            break;
         default:
            printf("ERR: ILL FORMED LIST");
			return;
			
      }
      display(list->c.cdr);
   }
}

/*
* Get the car value of a given list.
* (Uses assertions to ensure that this is a legitimate operation.)
*/
Value *car(Value *list){
	assert(list->type == CONS_TYPE);
	return (list->c.car);
}

/*
* Get the cdr value of a given list.
* (Uses assertions to ensure that this is a legitimate operation.)
*/
Value *cdr(Value *list){
	assert(list->type == CONS_TYPE);
	return (list->c.cdr);
}

/*
* Test if the given value is a NULL_TYPE value.
* (Uses assertions to ensure that this is a legitimate operation.)
*/
bool isNull(Value *value){
	return (value->type == NULL_TYPE);
}

/*
* Compute the length of the given list.
* (Uses assertions to ensure that this is a legitimate operation.)
*/
int length(Value *value){
	Value *temp = value;
	int count = 0;
	while (!isNull(temp)){
		assert(temp->type == CONS_TYPE);
		count++;
		temp = temp->c.cdr;
	}
	return count;
}

/*
* Helper method for reverse - takes care of actual recursing
*/
Value *reverseHelper(Value *list, Value *reversedList){
	if(isNull(list)){
		return reversedList;
	} else{
		assert(list->type == CONS_TYPE);
		return reverseHelper(list->c.cdr, cons(list->c.car, reversedList));
	}
}

/*
* Create a new linked list whose entries correspond to the given list's
* entries, but in reverse order.  The resulting list is a deep copy of the
* original: that is, there should be no shared memory between the original
* list and the new one.
*
* (Uses assertions to ensure that this is a legitimate operation.)
*
* FAQ: What if there are nested lists inside that list?
* ANS: There won't be for this assignment. There will be later, but that will
*      be after we've got an easier way of managing memory.
*/
Value *reverse(Value *list){
	Value *newList = makeNull();
	return reverseHelper(list, newList);
}

/*
* Frees up all memory directly or indirectly referred to by list.
*
* (Uses assertions to ensure that this is a legitimate operation.)
*
* FAQ: What if there are nested lists inside that list?
* ANS: There won't be for this assignment. There will be later, but that will
*      be after we've got an easier way of managing memory.
*/
void cleanup(Value *list){
	if(isNull(list)){
		free(list);
	} else{
		assert(list->type == CONS_TYPE);
		free(list->c.car);
		cleanup(list->c.cdr);
		free(list);
	}
}

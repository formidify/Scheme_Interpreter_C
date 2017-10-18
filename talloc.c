/*
* By Chae Kim, Tina Liu, James Yang
* Program implementing talloc, a garbage collector that stores pointers to
* memory allocated in a linked list.
*/
#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include <stdbool.h>

#ifndef TALLOC_H
#define TALLOC_H

static Value *head;
static bool initialized = false;

/*
* Create and return an empty list (a new Value object of type NULL_TYPE).
*/
Value *consMakeNull(){
   Value *list = malloc(sizeof(Value));
   if(!list){
      printf("Out of memory\n");
   }
   list->type= NULL_TYPE;
   return list;
}

/*
* Create and return a nonempty list (a new Value object of type CONS_TYPE).
*/
Value *tallocCons(Value *car, Value *cdr) {
   Value *list = consMakeNull();
   list->type = CONS_TYPE;
   list->c.car = car;
   list->c.cdr = cdr;
   return list;
}


/*
 * A malloc-like function that allocates memory, tracking all allocated
 * pointers in the "active list."  (You can choose your implementation of the
 * active list, but whatever it is, your talloc code should NOT call functions
 * in linkedlist.h; instead, implement any list-like behavior directly here.
 * Otherwise you'll end up with circular dependencies, since you're going to
 * modify the linked list to use talloc instead of malloc.)
 */
void *talloc(size_t size){
    if(!initialized){
        head = consMakeNull();
        initialized = true;
    }
    Value *newValue = malloc(size);
    Value *newPointer = malloc(sizeof(Value));
    newPointer->type = PTR_TYPE;
    newPointer->p = newValue;
    Value *temp = tallocCons(newPointer, head);
    head = temp;
    return newValue;
}

/*
 * A helper function for tfree() - uses recursion to free the active list
 */
void tfreeHelper(Value *list){
   if(list->type == CONS_TYPE){
      free(list->c.car->p);
      free(list->c.car);
      tfreeHelper(list->c.cdr);
      free(list);
   } else{
      free(list);
   }
}

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */

void tfree(){
    initialized = false;
    tfreeHelper(head);
}

/*
 * A simple two-line function to stand in the C function "exit", which calls
 * tfree() and then exit().  (You'll use this later to allow a clean exit from
 * your interpreter when you encounter an error: so memory can be automatically
 * cleaned up when exiting.)
 */
void texit(int status){
    tfree();
    exit(status);
}

#endif

#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include <stdbool.h>

#ifndef TALLOC_H
#define TALLOC_H

static Value *head;
static bool initialized = false;

/*
 * A malloc-like function that allocates memory, tracking all allocated
 * pointers in the "active list."  (You can choose your implementation of the
 * active list, but whatever it is, your talloc code should NOT call functions
 * in linkedlist.h; instead, implement any list-like behavior directly here.
 * Otherwise you'll end up with circular dependencies, since you're going to
 * modify the linked list to use talloc instead of malloc.)
 */
Value *talloc(size_t size){
    if(!initialized){
        head = malloc(sizeof(Value));
        head->type = NULL_TYPE;
        initialized = true;
        printf("initialized!");
    }
    Value *newValue = malloc(size);
    
    
    Value *newPointer = malloc(size);
    newPointer->type = PTR_TYPE;
    newPointer->p = newValue;
    head->c.cdr = head;
    head->c.car = newPointer;
    head->type = CONS_TYPE;
    return newValue;
}
    
/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */

void tfree(){
    while (head->type == CONS_TYPE) {
        free(head->c.car);
        head = head->c.cdr;
    }
    free(head);
}


/*
 * A simple two-line function to stand in the C function "exit", which calls
 * tfree() and then exit().  (You'll use this later to allow a clean exit from
 * your interpreter when you encounter an error: so memory can be automatically
 * cleaned up when exiting.)
 */
void texit(int status){
    tfree();
    exit(0);
}

#endif

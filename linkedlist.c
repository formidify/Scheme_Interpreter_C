/*
* By Chae Kim, Tina Liu, James Yang
* Program implementing methods for a Linked List in C
*/
#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include <string.h>
#include <assert.h>

/*
* Create and return an empty list (a new Value object of type NULL_TYPE).
*/
Value *makeNull(){
    Value *list = talloc(sizeof(Value));
    if(!list){
        printf("Out of memory\n");
    }
    list->type= NULL_TYPE;
    return list;
}

/*
* Create and return a nonempty list (a new Value object of type CONS_TYPE).
*/
Value *cons(Value *car, Value *cdr) {
    Value *list = makeNull();
    list->type = CONS_TYPE;
    list->c.car = car;
    list->c.cdr = cdr;
    return list;
}

/*
* Helper method for display(), works recursively to print a linked list.
*/
void displayHelper(Value *list){
    assert(list != NULL);
    switch(list->type) {
        case OPEN_TYPE:
            printf("%s", "(");
            break;
        case CLOSE_TYPE:
            printf("%s", ")");
            break;
        case BOOL_TYPE:
            printf("%d", list->b);
            break;
        case SYMBOL_TYPE:
            printf("%s", list->s);
            break;
        case INT_TYPE:
            printf("%i", list->i);
            break;
        case DOUBLE_TYPE:
            printf("%f", list->d);
            break;
        case STR_TYPE:
            printf("\"%s\"", list->s);
            break;
        case PTR_TYPE:
            printf("%p", list->p);
            break;
        case CONS_TYPE:
            displayHelper(list->c.car);
            if(cdr(list)->type != NULL_TYPE){
                printf(" ");
            }
            displayHelper(list->c.cdr);
            break;
        default:
            break;
    }
}

/*
* Print a representation of the contents of a linked list.
*/
void display(Value *list){
    assert(list != NULL);
    printf("(");
    displayHelper(list);
    printf(")\n");
}

/*
* Return the car value of a given list.
*/
Value *car(Value *list){
    assert(!isNull(list));
    assert(list->type == CONS_TYPE);
    assert(list->c.car != NULL);
    return (list->c.car);
}

/*
* Return the cdr value of a given list.
*/
Value *cdr(Value *list){
    assert(!isNull(list));
    assert(list->type == CONS_TYPE);
    assert(list->c.cdr != NULL);
    return (list->c.cdr);
}

/*
* Test if the given value is a NULL_TYPE value.
* Return 1 if true (is NULL_TYPE), otherwise return 0
*/
bool isNull(Value *value){
    assert(value != NULL);
    return (value->type == NULL_TYPE);
}

/*
* Return the length of the given list.
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
* entries, but in reverse order.  The resulting list is a shallow copy of the
* original: that is, stored data within the linked list should NOT be
* duplicated; rather, the new list's (new) CONS_TYPE nodes should point to
* precisely the items in the original list.
*
* (Uses assertions to ensure that this is a legitimate operation.)
*/
Value *reverse(Value *list){
    Value *newList = makeNull();
    return reverseHelper(list, newList);
}

/*
* Frees up all memory directly or indirectly referred to by list.
*
* Does not handle nested lists.
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

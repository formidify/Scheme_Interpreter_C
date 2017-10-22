/*
* By Tina Liu
* Program implementing methods for a Vector struct in C,
* which functions as a list.
*/
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "talloc.h"

/*
* Given a pointer to an existent (but uninitialized) Vector, allocates the
* internal values of the Vector to initialize it to be empty.  The initial
* size of the internally stored array is the given size.
*
* Returns 1 for success and 0 for failure.
*/
int init(Vector *list, int initialSize) {
   char *array = talloc(sizeof(char) * initialSize);
   if(!array){
      printf("Out of memory\n");
      return 0;
   }
   list->data = array;
   list->allocated = initialSize;
   list->size = 0;
   return 1;
}

/*
 * Prints the contents of the Vector.
 */
void print(Vector *list){
   for(int i = 0; i < list->size; i++){
      printf("%c ", list->data[i]);
   }
   printf("\n");
}

/*
 * Helper method for add() - creates a new array twice the size of the
 * existing array, copying over the values, adding the specified value at its
 * specified index and shifting everything after to the right
 *
 * Returns 1 for success and 0 for failure.
 */
int resizeAdd(Vector *list, int index, char value){
   char *newArray;
   if(list->allocated == 0){
      newArray = talloc(sizeof(char));
      list->allocated++;
   } else{
      newArray = talloc(sizeof(char) * (list->allocated * 2));
      list->allocated*=2;
   }
   if(!newArray){
      printf("Out of memory\n");
      return 0;
   }

   //Copies over values before value to be added
   for(int i = 0; i < index; i++){
      newArray[i] = list->data[i];
   }

   //Sets the value to be added
   newArray[index] = value;

   //Copies over values after value to be added
   for(int j = index + 1; j <= list->size; j++){
      newArray[j] = list->data[j - 1];
   }

   list->data = newArray;
   list->size++;
   return 1;
}

/*
 * Inserts the given value at the given index inside the given Vector.
 *
 * If the list has items in positions 0 through n-1, a new value is permitted
 * to be added at locations 0 through n.  (This is identical to Java's
 * ArrayList behaviour.)  This shifts the current value at the index (if any)
 * and all subsequent values (if any) to the right to make room for the new
 * value.  Inserting at the very end (position equal to size) is fine.  All
 * inserts should increase size by 1.
 *
 * If the internal array is too small to fit a new value, a new array of twice
 * the size is created with malloc.  The values are copied, the Vector is
 * appropriately updated, and the old array is freed.
 *
 * Returns 1 for success and 0 for failure (index too small or too large,
 * insufficient memory).
 */
int add(Vector *list, int index, char value){
   //Check if index is too small or large
   if(index > list->size || index < 0){
      printf("Index out of bounds\n");
      return 0;
   }

   if(list->size == list->allocated){
      //Create new array if internal array is too small
      return resizeAdd(list, index, value);
   } else{
      //Add value at specified index and shift everything after to the right
      char cur;
      for(int i = index; i <= list->size; i++){
         cur = list->data[i];
         list->data[i] = value;
         value = cur;
      }
      list->size++;
      return 1;
   }
}

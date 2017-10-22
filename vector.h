/*
 * Originally by Dave Musicant, and modified by David Liben-Nowell, Laura
 * Effinger-Dean, and Jed Yang.  Last modified by JY (Fall 2017).
 *
 * Header for vector.c.  The Vector struct contains the data necessary to
 * maintain the list; but because C is not object-oriented, the traditional
 * "methods" that you're accustomed to are instead functions that take a
 * pointer to that struct as a parameter.
 */

struct Vector {
   char* data;      /* A pointer to a traditional C array to hold data */
   int allocated;  /* The size of the actual C array that you have allocated */
   int size;       /* The number of data items actually stored in the Vector */
};

/* Introduces "Vector" as an abbreviation for "struct Vector", for ease. */
typedef struct Vector Vector;

/* Function prototypes.  These should be implemented in vector.c.  */

/*
 * Given a pointer to an existent (but uninitialized) Vector, allocates the
 * internal values of the Vector to initialize it to be empty.  The initial
 * size of the internally stored array is the given size.
 *
 * Returns 1 for success and 0 for failure.
 */
int init(Vector *list, int initialSize);

/*
 * Prints the contents of the Vector.
 */
void print(Vector *list);


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
int add(Vector *list, int index, char value);

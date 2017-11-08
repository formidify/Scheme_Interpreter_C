#ifndef VALUE_H
#define VALUE_H
#include <stdbool.h>

typedef enum {
    PTR_TYPE,
    INT_TYPE,
    DOUBLE_TYPE,
    STR_TYPE,
    CONS_TYPE,
    NULL_TYPE,
    OPEN_TYPE,
    CLOSE_TYPE,
    DOT_TYPE,
    BOOL_TYPE,
    SYMBOL_TYPE,
    VOID_TYPE,
    CLOSURE_TYPE,
    PRIMITIVE_TYPE
} valueType;

struct Value {
   valueType type;
   union {
      void *p;
      int i;
      double d;
      char *s;
      bool b;
	  struct Closure{
		  struct Value *fp;
		  struct Value *bod;
		  struct Frame *fr;
	  } cl;
      struct ConsCell {
         struct Value *car;
         struct Value *cdr;
      } c;
      struct Value *(*pf)(struct Value *);
   };
};

typedef struct Value Value;

#endif

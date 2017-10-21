#ifndef VALUE_H
#define VALUE_H

typedef enum {
   PTR_TYPE,
   INT_TYPE,
   DOUBLE_TYPE,
   STR_TYPE,
   CONS_TYPE,
   NULL_TYPE,
   OPEN_TYPE,
   CLOSE_TYPE,
   BOOL_TYPE,
   SYMBOL_TYPE
} valueType;

struct Value {
   valueType type;
   union {
      void *p;
      int i;
      double d;
      char *s;
      bool b;
      struct ConsCell {
         struct Value *car;
         struct Value *cdr;
      } c;
   };
};

typedef struct Value Value;

#endif

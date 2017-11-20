# Intepreter project
Tina Liu, Chae Kim, James Yang

Our interpreter project takes in a file of Scheme codes and evaluate them according to the syntax of R5RS, a dialect of Scheme. 

Our project supports methods that include:
1) Special forms: let, letrec, let*, quote, define, lambda, set!, and, or, cond, begin
2) Primitive functions: +, -, *, /, null?, car, cdr, cons, <=, pair?, eq?
3) Library procedures (the following functions can be used by calling (load "XX.scm") in your file):
  i) library.scm: not, equal?
  ii) math.scm: =, zero?, >=, <, >, positive?, negative?, abs, truncate, ceiling, floor, round, modulo, even?, odd?, max, min, gcd, lcm
  iii) lists.scm: list, append, caar, cadr, list?, length, list-ref, list-tail, member

Here is the list of extensions we implemented for this project:
1) lambda/let/letrec/let* support multiple body expressions, in which case they evaluate all and return the last one
2) if supports missing alternative statement
3) 'expr works as an alternative to (quote expr)
4) Our tokenizer/parser/intepreter give out informative error messages
5) Supports the syntactic sugar for binding variables to procedure
6) Supports internal definitions in the body of lambda/let/letrec/let* expressions (unless define is the last in the body (according to DrRacket))
7) Supports correct return types for + (ex. it returns an integer if all the input numbers are integers)
8) Supports dotted pair notation for input
9) Implements the function "load" such that (load "filename") will read the file and execute the Scheme codes within it
10) Supports correct behaviors for the primitive functions - and / such that it allows only one argument as well
11) Cond supports clauses with zero or more arguments
12) We support additional built-in functions for lists.scm: caar, cadr, list?, length, list-ref, list-tail, member
13) We support additional built-in functions for math.scm: =, >=, <, >, zero?, positive?, negative, even?, odd?, max, min, abs, gcd, lcm, modulo, floor, ceiling, truncate, round

Thanks for viewing our project! 
            

(load "library.scm")

(define list
  (lambda x x))


(define append
  (lambda x
    (letrec ((helper (lambda (x y) (if (null? x) y (cons (car x) (helper (cdr x) y))))))
      (cond ((null? x)
             (quote ()))
            ((null? (cdr x))
             (car x))
            ((not (null? (cdr (cdr x))))
             (evalError "Too many arguments are given."))
            ((and (pair? (car x)) (pair? (car (cdr x))))
             (helper (car x) (car (cdr x))))
            ((and (pair? (car x)) (not (pair? (car (cdr x)))))
             (helper (car x) (car (cdr x))))
            ((null? (car x))
             (car (cdr x)))
            ((not (pair? (car x)))
             (evalError "Cannot append to non-list."))))))
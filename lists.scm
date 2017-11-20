(load "math.scm")

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
             (append (car x) (car (cdr x))))
            ((and (pair? (car x)) (pair? (car (cdr x))))
             (helper (car x) (car (cdr x))))
            ((and (pair? (car x)) (not (pair? (car (cdr x)))))
             (helper (car x) (car (cdr x))))
            ((null? (car x))
             (car (cdr x)))
            ((not (pair? (car x)))
             (evalError "Cannot append to non-list."))))))


(define caar
  (lambda (x)
    (cond ((not (pair? x))
           (evalError "Expecting pair for arg."))
          ((pair? (car x))
           (car (car x)))
          (else
           (evalError "Expecting pair for (car arg).")))))

(define cadr
  (lambda (x)
    (cond ((not (pair? x))
           (evalError "Expecting pair for arg."))
          ((pair? (cdr x))
           (car (cdr x)))
          (else
           (evalError "Expecting pair for (cdr arg).")))))


(define list?
  (lambda (x)
    (if (pair? x)
        (list? (cdr x))
        (null? x))))


(define length
  (lambda (x)
    (cond ((and (list? x) (not (null? x)))
           (+ 1 (length (cdr x))))
          ((and (list? x) (null? x))
           0)
          (else
           (evalError "Argument must be a valid list.")))))

(define list-ref
  (lambda (x y)
    (cond ((and (pair? x) (<= 0 y))
           (if (= y 0)
               (car x)
               (list-ref (cdr x) (- y 1))))
          (else #f))))

(define list-tail
  (lambda (x y)
    (cond ((and (pair? x) (<= 0 y))
           (if (= y 0)
               x
               (list-tail (cdr x) (- y 1))))
          ((and (not (pair? x)) (= y 0))
           x)
          (else #f))))

(define member
  (lambda (x y)
    (cond ((and (list? y) (not (null? y)))
           (if (not (equal? x (car y)))
               (member x (cdr y))
               y))
          ((and (list? y) (null? y))
           #f)
          ((not (list? y))
           #f))))
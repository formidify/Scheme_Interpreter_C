(define helper
  (lambda (x y)
    (if (and (number? x) (number? y))
        (if (eq? x y)
            #t
            #f)
        (display "Error: input to '=' must be number.\n"))))

(define =
  (lambda x
    (if (null? (cdr x))
        (display "Error: need more than 2 arguments.\n")
        (helper (car x) (car (cdr x))))))


(define zero?
  (lambda (x)
    (if (= 0 x)
        #t
        #f)))


(define modulo
  (lambda (x y)
    (if (= 0 y)
        (display "Error: Cannot divide by zero.\n")
        (if (< x y)
            x
            (if (> (- x y) y)
            (modulo (- x y) y)
            (- x y))))))
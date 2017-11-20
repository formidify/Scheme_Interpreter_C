(load "library.scm")

(define =
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (<= x y) (<= y x))
        (evalError "Input to = must be number."))))

(define zero?
  (lambda (x)
    (if (number? x)
        (= 0 x)
        (evalError "Input to zero? must be a number"))))

(define <
    (lambda (x y)
    (if (and (number? x) (number? y))
        (and (<= (- x y) 0) (not (zero? (- x y))))
        (evalError "Input to < must be number."))))

(define modulo
  (lambda (x y)
    (letrec ((helper (lambda (x y)
                        (cond ((zero? y) (evalError "Cannot divide by zero."))
                              ((< x y) x)
                              (else (modulo (- x y) y))))))
        (helper x y))))

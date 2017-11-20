(load "library.scm")

(define =
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (<= x y) (<= y x))
        (evalError "Input to = must be number."))))

(define >=
    (lambda (x y)
        (if (and (number? x) (number? y))
            (<= (- y x) 0)
            (evalError "Input to >= must be number."))))

(define zero?
  (lambda (x)
    (= 0 x)))

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

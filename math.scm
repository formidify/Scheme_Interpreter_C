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

(define >
    (lambda (x y)
        (if (and (number? x) (number? y))
            (and (>= (- x y) 0) (not (zero? (- x y))))
            (evalError "Input to >= must be number."))))

(define positive?
    (lambda (x)
        (if (number? x)
            (> x 0)
            (evalError "Input to positive? must be number."))))

(define negative?
    (lambda (x)
        (if (number? x)
            (< x 0)
            (evalError "Input to negative? must be number."))))

(define abs
    (lambda (x)
        (if (number? x)
            (if (< x 0) (* x -1) x)
            (evalError "Input to abs must be number."))))

(define modulo
  (lambda (x y)
    (letrec ((helper (lambda (x y)
                        (cond ((zero? y) (evalError "Cannot divide by zero."))
                              ((< (abs x) (abs y)) x)
                              (else (modulo (- x y) y))))))
        (helper x y))))

(define even?
    (lambda (x)
        (if (integer? x)
            (zero? (modulo x 2))
            (evalError "Input to even? must be integer."))))

(define odd?
    (lambda (x)
        (not (even? x))))

(define max
    (lambda (x y)
        (if (and (number? x) (number? y))
            (if (>= x y) x y)
            (evalError "Input to max must be number."))))

(define min
    (lambda (x y)
        (if (and (number? x) (number? y))
            (if (<= x y) x y)
            (evalError "Input to min must be number."))))

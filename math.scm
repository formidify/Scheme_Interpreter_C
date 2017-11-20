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

(define >=
    (lambda (x y)
    (if (and (number? x) (number? y))
        (<= (- y x) 0)
        (evalError "Input to >= must be number."))))

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

(define truncate
    (lambda (x)
        (if (number? x)
            (letrec ((helper (lambda(x)
                        (if (positive? x)
                            (if (and (>= x 0) (< x 1))
                                0
                                (+ 1 (truncate (- x 1))))
                            (if (and (<= x 0) (> x -1))
                                0
                                (- (truncate (+ x 1)) 1))))))
                (helper x))
            (evalError "Input to truncate must be number."))))

(define ceiling
    (lambda (x)
        (if (number? x)
            (letrec ((helper (lambda (x)
                    (cond ((positive? x)
                                (if (and (> x 0) (<= x 1))
                                    1
                                    (+ 1 (ceiling (- x 1)))))
                            ((negative? x)
                                (if (and (< x 0) (> x -1))
                                    0
                                    (- (ceiling (+ x 1)) 1)))
                            (else 0)))))
                (helper x)))))

(define floor
    (lambda (x)
        (if (number? x)
            (letrec ((helper (lambda (x)
                    (cond ((positive? x)
                                (if (and (>= x 0) (< x 1))
                                    0
                                    (+ 1 (floor (- x 1)))))
                            ((negative? x)
                                (if (and (< x 0) (>= x -1))
                                    -1
                                    (- (floor (+ x 1)) 1)))
                            (else 0)))))
                (helper x)))))

(define modulo
    (lambda (x y)
        (if (number? x)
            (- x (* (floor (/ x y)) y))
            (evalError "Input to modulo must be number."))))

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

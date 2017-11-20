(define not
    (lambda (x)
        (if x #f #t)))

(define list
  (lambda x x))

(define append-helper
  (lambda (x y)
    (if (null? x)
        y
        (cons (car x) (append-helper (cdr x) y)))))

(define append
  (lambda x
    (if (null? x)
        (quote ())
        (if (null? (cdr x))
            (car x)
            (if (not (null? (cdr (cdr x))))
                #f
                (cond ((and (pair? (car x)) (pair? (car (cdr x))))
                       (append-helper (car x) (car (cdr x))))
                      ((and (pair? (car x)) (not (pair? (car (cdr x)))))
                       (append-helper (car x) (car (cdr x))))
                      ((null? (car x))
                       (car (cdr x)))
                      ((not (pair? (car x)))
                       #f)))))))
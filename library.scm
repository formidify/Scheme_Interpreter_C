;;A library containing general built-in Scheme functions.
(define not
    (lambda (x)
        (if x #f #t)))

(define equal?
    (lambda (x y)
        (if (and (pair? x) (pair? y))
            (letrec ((helper (lambda (x y)
                                (cond ((not (equal? (car x) (car y))) #f)
                                    (else (equal? (cdr x) (cdr y)))))))
                (helper x y))
            (eq? x y))))

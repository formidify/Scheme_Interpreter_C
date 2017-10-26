(+ 2.34 +.6)
(define x (quote a))
(define insert
  (lambda (v bst)
    (cond ((not (bst? bst)) #f)
          ((null-bst? bst) (make-bst v (null-bst) (null-bst)))
          ((< v (entry bst)) (make-bst (entry bst) (insert v (left bst)) (right bst)))
          ((> v (entry bst)) (make-bst (entry bst) (left bst) (insert v (right bst))))
          (else bst))))

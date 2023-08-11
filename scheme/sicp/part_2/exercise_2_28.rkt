#lang sicp

(define (fringe items)
  (cond ((null? items) items)
        ((pair? items) (append (fringe (car items)) (fringe (cdr items))))
        (else
         (list items))))

(define x (list (list 1 2 4) (list 3 9)))
(define y (list
           (list (list 1 2 4) (list 8 4 6))
           (list (list (list 3 9) (list 11 23))
                  (list 83 72) (list 3 9 0) (list 17 19 25 48 12))))

(fringe x)
(fringe y)

(define a 3)
a
(define b (append (list a) (list 4)))
b
(car b)
(cdr b)
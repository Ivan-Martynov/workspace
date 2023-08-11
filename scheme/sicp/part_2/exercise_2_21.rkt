#lang sicp

(define (square x) (* x x))

(define (square-list-1 items)
  (if (null? items)
      nil
      (cons (square (car items)) (square-list-1 (cdr items)))))

(define (square-list-2 items) (map square items))

(square-list-1 (list 1 2 3 4))
(square-list-2 (list 1 2 3 4))

(define (square-list-3 items)
  (define (iter things answer)
    (if (null? things)
        answer
        (iter (cdr things)
              (cons answer (square (car things))
                    ))))
  (iter items nil))

(square-list-3 (list 1 2 3 4))
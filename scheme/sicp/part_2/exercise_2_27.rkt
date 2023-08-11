#lang sicp

(define x (list (list 1 2 4) (list 3 9)))
(define y (list
           (list (list 1 2 4) (list 8 4 6))
           (list (list (list 3 9) (list 11 23))
                  (list 83 72) (list 3 9 0) (list 17 19 25 48 12))))

(define (reverse a)
  (if (null? a)
      a
      (append (reverse (cdr a)) (list (car a)))))


(define (deep-reverse a)
  (cond ((null? a) nil)
        ((pair? (car a))
         (append (deep-reverse (cdr a)) (list (deep-reverse (car a)))))
        (else
         (append (deep-reverse (cdr a)) (list (car a))))))

x
(reverse x)
(deep-reverse x)
;(deep-reverse 1)

y
;(reverse y)
(deep-reverse y)

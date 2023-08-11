#lang sicp

(define (count-leaves x)
  (cond ((null? x) 0)
        ((not (pair? x)) 1)
        ;((null? (cdr x)) 1)
        (else (+ (count-leaves (car x))
                 (count-leaves (cdr x))))))

(define x (cons (list 1 2) (list 3 4)))

(length x)

(count-leaves x)

(list x x)

(length (list x x))

(count-leaves (list x x))

(list 1 (list 2 (list 3 4)))


(define a1 (list 1 3 (list 5 7) 9))
(define a2 (list (list 7)))
(define a3 (list 1 (list 2 (list 3 (list 4 (list 5 (list 6 7)))))))
(display a1)
(newline)
(car (cdr (car (cdr (cdr a1)))))
(display a2)
(newline)
(car (car a2))
(display a3)
(newline)
(car (cdr (car (cdr (car (cdr (car (cdr (car (cdr (car (cdr a3))))))))))))

(define b (list 1 2 3))
(define c (list 4 5 6))

(append b c)
(cons b c)
(list b c)

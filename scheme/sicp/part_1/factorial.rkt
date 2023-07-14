#lang sicp

(define (factorial_01 n)
  (if (= n 1)
      n
      (* n (factorial_01 (- n 1)))))

(define (factorial_02 n)
  (define (iter product count)
    (if (> count n)
        product
        (iter (* count product) (+ count 1))))
  (iter 1 1))

(factorial_01 6)
(factorial_02 6)

(define (A x y)
  (cond ((= y 0) 0)
        ((= x 0) (* 2 y))
        ((= y 1) 2)
        (else (A (- x 1) (A x (- y 1))))))

(A 1 10)
(A 2 4)
(A 3 3)
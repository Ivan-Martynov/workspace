#lang sicp

(define (is_even n)
  (= (remainder n 2) 0))
         

(define (fib_iter a b p q count)
  (cond ((= count 0) b)
        ((is_even count)
         (fib_iter a
                   b
                   (+ (* p p) (* q q))
                   (+ (* 2 q p) (* q q))
                   (/ count 2)))
        (else (fib_iter (+ (* b q) (* a q) (* a p))
                        (+ (* b p) (* a q))
                        p
                        q
                        (dec count)))))

(define (fib n)
  (fib_iter 1 0 0 1 n))

(fib 0)
(fib 1)
(fib 2)
(fib 3)
(fib 4)
(fib 5)
(fib 6)
(fib 7)
(fib 8)
(fib 9)
(fib 10)

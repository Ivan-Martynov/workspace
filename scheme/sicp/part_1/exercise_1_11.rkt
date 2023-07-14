#lang sicp

(define (f n)
  (if (< n 3)
      n
      (+ (f (- n 1)) (* 2 (f (- n 2))) (* 3 (f (- n 3))))))

(define (f2 n)
  (f2_iter 2 1 0 n))

(define (f2_iter a b c n)
  (if (= n 0)
      c
      (f2_iter (+ a (* 2 b) (* 3 c)) a b (dec n))))

(f 1)
(f 2)
(f 3)
(f 4)
(f 5)
(f 6)
(f 7)
(f 8)

(f2 1)
(f2 2)
(f2 3)
(f2 4)
(f2 5)
(f2 6)
(f2 7)
(f2 8)
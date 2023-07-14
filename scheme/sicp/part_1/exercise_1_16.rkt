#lang sicp

(define (is_even n)
  (= (remainder n 2) 0))

(define (fast_expt_iter a b n)
  (cond ((= n 0) a)
        ((is_even n) (fast_expt_iter a (* b b) (/ n 2)))
        (else (fast_expt_iter (* a b) b (dec n)))))

(define (fast_expt a n)
  (fast_expt_iter 1.0 a n))

(define (square n) (* n n))

(define (fast-expt-rec b n)
  (cond ((= n 0) 1)
        ((is_even n) (square (fast-expt-rec b (/ n 2))))
        (else (* b (fast-expt-rec b (- n 1))))))

(fast-expt-rec -3.0 0)
(fast-expt-rec -3.0 1)
(fast-expt-rec -3.0 2)
(fast-expt-rec -3.0 5)
(fast-expt-rec -3.0 6)

(fast_expt -3.0 0)
(fast_expt -3.0 1)
(fast_expt -3.0 2)
(fast_expt -3.0 5)
(fast_expt -3.0 6)

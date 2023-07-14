#lang sicp

(define (* a b)
  (cond ((< b 0) (* (- a) (- b)))
        ((= b 0) 0)
        (else (+ a (* a (dec b))))))

(define (is_even n)
  (= (remainder n 2) 0))

(define (double a) (+ a a))

(define (halve a) (/ a 2))

(define (fast_mult_rec a b)
  (cond ((< b 0) (fast_mult_rec (- a) (- b)))
        ((= b 0) 0)
        ((is_even b) (double (fast_mult_rec a (halve b))))
        (else (+ a (fast_mult_rec a (dec b))))))

(define (fast_mult a b)
  (fast_mult_iter 0.0 a b))

(define (fast_mult_iter x a b)
  (cond ((< b 0) (fast_mult_iter x (- a) (- b)))
        ((= b 0) x)
        ((is_even b) (fast_mult_iter x (double a) (halve b)))
        (else (fast_mult_iter (+ x a) a (dec b)))))

(* 0 0)
(* 3 0)
(* 0 4)
(* 1 4)
(* 5 1)
(* 2.3 5)
(* -10 16)
(* -16 10)
(* -10 -10)

(fast_mult_rec 0 0)
(fast_mult_rec 3 0)
(fast_mult_rec 0 4)
(fast_mult_rec 1 4)
(fast_mult_rec 5 1)
(fast_mult_rec 2.3 5)
(fast_mult_rec -10 16)
(fast_mult_rec -16 10)
(fast_mult_rec -10 -10)

(fast_mult 0 0)
(fast_mult 3 0)
(fast_mult 0 4)
(fast_mult 1 4)
(fast_mult 5 1)
(fast_mult 2.3 5)
(fast_mult -10 16)
(fast_mult -16 10)
(fast_mult -10 -10)



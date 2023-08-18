#lang sicp

(define (int-pow x n)
  (if (= n 0)
      1
      (* x (int-pow x (dec n)))))

(define (cons-prod x y)
  (* (expt 2 x) (expt 3 y)))

(define (count-power num n)
  (if (not (= (remainder num n) 0))
      0
      (inc (count-power (/ num n) n))))

(define (car-prod z)
  (count-power z 2))

(define (cdr-prod z)
  (count-power z 3))


(define z (cons-prod 9 5))
(car-prod z)
(cdr-prod z)
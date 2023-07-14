#lang sicp

(define (average x y)
  (* (+ x y) 0.5))

(define (improve guess x)
  (average guess (/ x guess)))

(define (square x)
  (* x x))

(define (good_enough guess x)
  (< (abs (- (square guess) x)) 0.0001))

(define (good_enough2 guess prev_guess)
  (< (abs (- guess prev_guess)) 0.0001))

(define (sqrt_iter guess x)
  (if (good_enough guess x)
      guess
      (sqrt_iter (improve guess x) x)))

(define (sqrt_iter2 guess prev_guess x)
  (if (good_enough2 guess prev_guess)
      guess
      (sqrt_iter2 (improve guess x) guess x)))

(define (sqrt x)
  (sqrt_iter 1.0 x))

(define (sqrt2 x)
  (sqrt_iter2 1.0 x x))

(sqrt 9)
(sqrt2 9)

(sqrt (+ 100 37))
(sqrt2 (+ 100 37))

(sqrt (+ (sqrt 2) (sqrt 3)))
(sqrt2 (+ (sqrt 2) (sqrt 3)))

(square (sqrt 1000))
(square (sqrt2 1000))

(define x 0.000009)
(sqrt x)
(sqrt2 x)

(define y 1234567890123)
(sqrt y)
(sqrt2 y)

(define (improve_cubic guess x)
  (/ (+ (/ x (square guess)) guess guess) 3))

(define (cubic_root_iter guess prev_guess x)
  (if (good_enough2 guess prev_guess)
      guess
      (cubic_root_iter (improve_cubic guess x) guess x)))

(define (cubic_root x)
  (cubic_root_iter 1.0 x x))

(cubic_root 8.0)
(cubic_root -8.0)

(cubic_root (+ 90 10 25))
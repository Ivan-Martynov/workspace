#lang sicp

(define (average x y) (/ (+ x y) 2.0))

(define tolerance 0.0001)

(define (fixed_point f first_guess)
  (define (are_close_enough v1 v2)
    (< (abs (- v1 v2)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      (display "current guess = ")
      (display guess)
      (newline)
      (if (are_close_enough guess next)
          next
          (try next))))
  (try first_guess))

;(define (sqrt x)
  ;forever loop => (fixed_point (lambda (y) (/ x y)) 1.0))
;  (fixed_point (lambda (y) (average y (/ x y))) 1.0))

"Golden ratio"
(fixed_point (lambda (x) (inc (/ 1.0 x))) 1.0)
(/ (inc (sqrt 5)) 2)

;(fixed_point cos 1.0)
;(sqrt 3)

"Find x for x^x = 1000"
(fixed_point (lambda (x) (/ (log 1000) (log x))) 2.0)
"Find x for x^x = 1000 with average damping"
(fixed_point (lambda (x) (average x (/ (log 1000) (log x)))) 2.0)



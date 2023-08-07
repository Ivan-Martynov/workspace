#lang sicp

(define (average x y) (/ (+ x y) 2))

(define tolerance 0.0001)

(define (fixed_point f first_guess)
  (define (are_close_enough v1 v2)
    (< (abs (- v1 v2)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
      ;(display "current guess = ")
      ;(display guess)
      ;(newline)
      (if (are_close_enough guess next)
          next
          (try next))))
  (try first_guess))


(define (square x) (* x x))

(define (iterative_improve good_enough? improve_guess)
  (define (try guess)
    (if (good_enough? guess)
        guess
        (try (improve_guess guess))))
  (lambda (guess) (try guess)))

(define (sqrt_imp x)
  ((iterative_improve
    (lambda (guess) (< (abs (- (square guess) x)) tolerance))
    (lambda (guess) (average guess (/ x guess))))
   1.0))

"Square root with iterative_improve"
(sqrt_imp 25)
(newline)

(define (fixed_point_imp f first_guess)
  (define (close_enough? next)
    (< (abs (- next first_guess)) tolerance))
  (define (improve_guess guess) (f guess))
  ((iterative_improve close_enough? f) first_guess))
    

(define dx 0.0001)

(define (deriv g)
  (lambda (x) (/ (- (g (+ x dx)) (g x)) dx)))

(define (cube x) (* x x x))

(define (newton_transform g)
  (lambda (x) (- x (/ (g x) ((deriv g) x)))))

(define (newton_method g guess)
  (fixed_point (newton_transform g) guess))


(define (sqrt x)
  (newton_method
   (lambda (y) (- (square y) x)) 1.0))

((deriv cube) 5)
(sqrt 5)

(define (cubic a b c)
  (lambda (x)
  (+ (cube x) (* a (square x)) (* b x) c)))

(newton_method (cubic 1 3 4) 1)

(define (double g)
  (lambda (x) (g (g x))))

((double inc) 3)
(((double (double double)) inc) 5)

(define (compose f g)
  (lambda (x) (f (g x))))

((compose square inc) 6)

(define (repeated f n)
  (if (= n 1)
      f
      (compose f (repeated f (dec n)))))

((repeated square 3) 2)
((repeated dec 5) 13)

(define (smooth f)
  (lambda (x) (/ (+ (f (- x dx)) (f x) (f (+ x dx))) 3.0)))

(define (smooth_n_fold f n)
  ((repeated smooth n) f))

(define (exp_n_iter x n)
  (define (iter i result)
    (if (= i 0)
        result
        (* x (iter (dec i) result))))
  (iter n 1))

(define (exp_n x n)
  (if (= n 0)
      1
      (* x (exp_n x (dec n)))))

(exp_n_iter 5.4 3)

(define (average_damp f)
  (lambda (x) (average x (f x))))

(define (nth_root x n)
  (let ((damp (floor (/ (log n) (log 2)))))
  (fixed_point ((repeated average_damp damp)
                (lambda (y)
                  (/ x (exp_n y (dec n))))) 1.0)))


(nth_root 9 5)
(define y (nth_root 2 258))
(display y)
(newline)
(exp_n y 258)
(nth_root 5 32)


(define (general_root x nth)
  (fixed_point (general-damping
                (lambda (y) (/ x (exp_n y (dec nth))))
                (/ (dec nth) nth))
               1.0))

(define (general-damping f alpha)
  (lambda (x) (+
               (* (- 1 alpha) (f x))
               (* alpha x))))

(general_root 2 258)




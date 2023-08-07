#lang sicp

(define (cont_frac_iter n d k)
  (define (iter i result)
    ;(display result)
    ;(newline)
    (if (= i 0)
        result
        (iter (dec i) (/ (n i) (+ (d i) result)))))
  (iter k 0))

(define (cont_frac_rec n d k)
  (define (step i)
    (if (= i k)
        (/ (n i) (d i))
        (/ (n i) (+ (d i) (step (inc i))))))
  (step 1))
  

(define (cont_frac n d k)
  (cont_frac_iter n d k))

(define phi (/ (inc (sqrt 5)) 2))

"1/phi"
(display (/ 1 phi))
(newline)
(define k 1)
(cont_frac (lambda (i) 1.0) (lambda (i) 1.0) k)

(define tolerance 0.00001)

(define (find_k k)
  (let ((next (cont_frac (lambda (i) 1.0) (lambda (i) 1.0) k)))
    (display k)
    (display " => ")
    (display next)
    (newline)
    (if (> (abs (- next (/ 1 phi))) tolerance)
        (find_k (inc k)))))

(find_k 4)

(define (exp_approx n)
  (define (d_fun k)
    (if (= (remainder (inc k) 3) 0)
        (* (/ (inc k) 3) 2)
        1))
  (+ 2 (cont_frac (lambda (i) 1.0) d_fun n)))

(define (tan_cf x k)
  (define (n_fun k)
    (if (= k 1)
        x
        (- (* x x))))
  (define (d_fun k)
    (inc (* (dec k) 2)))
  (cont_frac n_fun d_fun k))

(exp_approx 7)

(define pi 3.14)
(tan_cf (/ pi 8) 4)
(tan (/ pi 8))







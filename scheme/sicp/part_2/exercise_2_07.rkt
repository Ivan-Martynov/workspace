#lang sicp

(define (make-interval a b) (cons a b))

(define (make-center-width c w)
  (make-interval (- c w) (+ c w)))

(define (make-center-percent c p)
  (let ((dx (/ (* p c) 100.0)))
    (make-center-width c dx)))

(define (lower-bound z) (car z))
(define (upper-bound z) (cdr z))

(define (add-interval x y)
  (make-interval (+ (lower-bound x) (lower-bound y))
                 (+ (upper-bound x) (upper-bound y))))

(define (sub-interval x y)
  (make-interval (- (lower-bound x) (lower-bound y))
                 (- (upper-bound x) (upper-bound y))))

(define (is-negative z)
  (and (< (lower-bound z) 0) (< (upper-bound z) 0)))

(define (is-non-negative z)
  (and (>= (lower-bound z) 0) (>= (upper-bound z) 0)))

(define (does-span-zero z)
  (<= (* (lower-bound z) (upper-bound z)) 0))

(define (mul-interval x y)
  (let ((x-lower (lower-bound x))
        (x-upper (upper-bound x))
        (y-lower (lower-bound y))
        (y-upper (upper-bound y)))
    (cond ((is-non-negative x)
           (cond ((is-non-negative y)
                  (make-interval (* x-lower y-lower)
                                 (* x-upper y-upper)))
                 ((is-negative y)
                  (make-interval (* x-upper y-lower)
                                 (* x-lower y-upper)))
                 (else
                  (make-interval (* x-upper y-lower)
                                 (* x-upper y-upper)))))
          ((is-negative x)
           (cond ((is-non-negative y)
                  (make-interval (* x-lower y-upper)
                                 (* x-upper y-lower)))
                 ((is-negative y)
                  (make-interval (* x-upper y-upper)
                                 (* x-lower y-lower)))
                 (else
                  (make-interval (* x-lower y-upper)
                                 (* x-lower y-lower)))))
          (else
           (cond ((is-non-negative y)
                  (make-interval (* x-lower y-upper)
                                 (* x-upper y-upper)))
                 ((is-negative y)
                  (make-interval (* x-upper y-lower)
                                 (* x-lower y-lower)))
                 (else
                  (make-interval (min
                                  (* x-lower y-lower)
                                  (* x-lower y-upper)
                                  (* x-upper y-lower)
                                  (* x-upper y-upper))
                                 (max
                                  (* x-lower y-lower)
                                  (* x-lower y-upper)
                                  (* x-upper y-lower)
                                  (* x-upper y-upper)))))))))

(define (div-interval x y)
  (if (does-span-zero y)
      (error "Cannot divide by interval with zero span.")
      (mul-interval
       x
       (make-interval (/ 1.0 (upper-bound y))
                      (/ 1.0 (lower-bound y))))))

(define (width z)
  (/ (- (upper-bound z) (lower-bound z)) 2))

(define (center z)
  (/ (+ (lower-bound z) (upper-bound z)) 2))

(define (percent z)
  (/ (* (width z) 100) (center z)))

(define (print-interval z)
  (display "(")
  (display (lower-bound z))
  (display ", ")
  (display (upper-bound z))
  (display ")")
  (newline))

(define (debug-interval z)
  (display "(")
  (display (lower-bound z))
  (display ", ")
  (display (upper-bound z))
  (display "), width = ")
  (display (width z))
  (display ", center = ")
  (display (center z))
  (display ", percent = ")
  (display (percent z))
  (display ", bounds = [")
  (display (lower-bound z))
  (display ", ")
  (display (upper-bound z))
  (display "]")
  (newline))

(define interval_1 (make-center-percent 6.8 10))
(define interval_2 (make-center-percent 4.7 10))
(define interval_3 (make-interval -1.1 2.3))
(define interval_4 (make-center-width 2.0 3.0))

(debug-interval interval_1)
(debug-interval interval_2)
(debug-interval interval_3)
(debug-interval interval_4)
;(define test (div-interval interval_4 interval_1))
;(debug-interval test)

(define one (make-interval 1.0 1.0))
(debug-interval one)

(define (par-1 r1 r2)
  (div-interval (mul-interval r1 r2)
                (add-interval r1 r2)))

(define (par-2 r1 r2)
  (div-interval one (add-interval (div-interval one r1)
                                  (div-interval one r2))))

"Par-1"
(debug-interval (par-1 interval_1 interval_2))
"Par-2"
(debug-interval (par-2 interval_1 interval_2))
(debug-interval (div-interval interval_1 interval_1))
(debug-interval (div-interval interval_1 interval_2))






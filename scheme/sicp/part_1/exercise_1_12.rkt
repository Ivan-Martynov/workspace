#lang sicp

(define (f i j)
  (cond ((or (= i 0) (<= j 0) (>= j i)) 1)
        (else (+ (f (dec i) (dec j)) (f (dec i) j)))))

(f 0 0)
(f 1 3)
(f 2 1)
(f 3 2)
(f 4 1)
(f 4 2)
(f 5 2)
(f 5 8)
(f 10 -1)
(f 10 4)
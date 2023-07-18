#lang sicp

(define (square x) (* x x))

(define (is_divisible a b)
  (= (remainder b a) 0))

(define (find_divisor n value)
  (cond ((> (square value) n) n)
        ((is_divisible value n) value)
        (else (find_divisor n (inc value)))))

(define (smallest_divisor n)
  (find_divisor n 2))

(define (is_prime n)
  (= n (smallest_divisor n)))

(define (is_even n)
  (= (remainder n 2) 0))

(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))

(define (expmod base exp m)
  (cond ((= exp 0) 1)
        ((is_even exp)
         (remainder (square (expmod base (/ exp 2) m))
                    m))
        (else (remainder (* base (expmod base (- exp 1) m))
                         m))))

(define (fermat_test n)
  (define (try_it a)
    (= (expmod a n n) a))
  (try_it (inc (random (dec n)))))

(define (is_prime_fast n count)
  (cond ((= count 0) true)
        ((fermat_test n) (is_prime_fast n (dec count)))
        (else false)))

(is_prime 2)
(is_prime 237)
(is_prime 23)


(is_prime_fast 2 2)
(is_prime_fast 237 13)
(is_prime_fast 23 3)

(is_prime 561)
(is_prime_fast 561 560)

(is_prime 1105)
(is_prime_fast 1105 1104)

(is_prime 1729)
(is_prime_fast 1729 1728)

(is_prime 6601)
(is_prime_fast 6601 6600)

(smallest_divisor 199)
(smallest_divisor 1999)
(smallest_divisor 19999)



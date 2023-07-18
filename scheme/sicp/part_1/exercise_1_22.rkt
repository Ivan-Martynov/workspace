#lang sicp

(define (is_even n)
  (= (remainder n 2) 0))

(define (square x) (* x x))

(define (is_divisible a b)
  (= (remainder b a) 0))

(define (next x)
  (if (= x 2)
      3
      (+ x 2)))

(define (find_divisor n value)
  (cond ((> (square value) n) n)
        ((is_divisible value n) value)
        (else (find_divisor n (next value)))))

(define (smallest_divisor n)
  (find_divisor n 2))

(define (is_prime n)
  (= n (smallest_divisor n)))

(define (report_prime n elapsed)
  (display n)
  (display " => *** ")
  (display elapsed)
  (newline))

(define (start_prime_test n start_time)
  (if (is_prime n)
      (report_prime n (- (runtime) start_time))
      false))

(define (timed_prime_test n)
  (newline)
  (start_prime_test n (runtime)))

(define (search_for_primes_odd_count n count)
  (if (< count 3)
      (if (start_prime_test n (runtime))
          (search_for_primes_odd_count (+ n 2) (inc count))
          (search_for_primes_odd_count (+ n 2) count))))

(define (search_for_primes n)
  (if (is_even n)
      (search_for_primes_odd_count (inc n) 0)
      (search_for_primes_odd_count n 0)))

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

(define (check_rem n m)
  (if (and (not (or (= n 1)
                    (= n (dec m))))
           (= (remainder (square n) m) 1))
      0
      (remainder (square n) m)))

(define (expmod_mr base exp m)
  (cond ((= exp 0) 1)
        ((is_even exp) (check_rem (expmod_mr base (/ exp 2) m) m))
        (else (check_rem (* base (expmod_mr base (- exp 1) m)) m))))

(define (miller_rabin_test n)
  (define (try_it a)
    ;(= (expmod_mr a (dec n) n) a))
    (not (= (expmod_mr a (dec n) n) 0)))
  (try_it (inc (random (dec n)))))

(define (start_prime_test_fermat n start_time)
  (if (is_prime_fast n n)
      (report_prime n (- (runtime) start_time))
      false))

(define (timed_prime_fermat n)
  (newline)
  (start_prime_test_fermat n (runtime)))

(define (is_prime_fast_mr n count)
  (cond ((= count 0) true)
        ((miller_rabin_test n)
         (is_prime_fast_mr n (dec count)))
        (else false)))


(sqrt 10)
(search_for_primes 10000)
(sqrt 10000)
(search_for_primes 100000)
(sqrt 100000)
(search_for_primes 1000000)
(sqrt 1000000)

(is_prime 561)
(fermat_test 561)
(is_prime_fast_mr 561 560)

;(newline)
;(timed_prime_fermat 10007)
;(timed_prime_fermat 10009)
;(timed_prime_fermat 10037)
;(newline)
;(timed_prime_fermat 100003)
;(timed_prime_fermat 100019)
;(timed_prime_fermat 100043)
;(newline)
;(timed_prime_fermat 1000003)
;(timed_prime_fermat 1000033)
;(timed_prime_fermat 1000037)

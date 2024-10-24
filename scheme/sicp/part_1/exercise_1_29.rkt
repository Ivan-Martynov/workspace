#lang sicp

(define (is_even n)
  (= (remainder n 2) 0))

(define (identity x) x)

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
(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))

(define (sum_rec term a next b)
  (if (> a b)
      0
      (+ (term a) (sum_rec term (next a) next b))))

(define (sum_iter term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (+ result (term a)))))
  (iter a 0))

(define (product_rec term a next b)
  (if (> a b)
      1
      (* (term a) (product_rec term (next a) next b))))

(define (product_iter term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (* result (term a)))))
  (iter a 1))

(define (filtered_accumulate_recursive
         filter
         combiner
         null_value
         term
         a
         next
         b)
  (define (filtered_term x)
    (if (filter x)
        (term x)
        null_value))
  (if (> a b)
      null_value
      (combiner (filtered_term a)
                (filtered_accumulate_recursive
                 filter
                 combiner
                 null_value
                 term
                 (next a)
                 next b))))

(define (filtered_accumulate_iterative
         filter
         combiner
         null_value
         term
         a
         next
         b)
  (define (filtered_term x)
    (if (filter x)
        (term x)
        null_value))
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (combiner result (filtered_term a)))))
  (iter a null_value))

(define (filtered_accumulate
         filter
         combiner
         null_value
         term
         a
         next
         b)
  (filtered_accumulate_iterative
         filter
         combiner
         null_value
         term
         a
         next
         b))

(define (accumulate_recursive combiner null_value term a next b)
  (if (> a b)
      null_value
      (combiner (term a)
                (accumulate_recursive
                 combiner
                 null_value
                 term
                 (next a)
                 next b))))

(define (accumulate_iterative combiner null_value term a next b)
  (define (iter a result)
    (if (> a b)
        result
        (iter (next a) (combiner result (term a)))))
  (iter a null_value))

;(define (accumulate combiner null_value term a next b)
;  (accumulate_iterative combiner null_value term a next b))

(define (accumulate combiner null_value term a next b)
  (filtered_accumulate
   (lambda (x) true)
   combiner
   null_value
   term
   a
   next
   b))

(define (sum_prime_squares a b)
  (filtered_accumulate
   is_prime
   +
   0
   square
   a
   inc
   b))

(define (product_relative_primes n)
  (filtered_accumulate
   (lambda (k) (= (gcd n k) 1))
   *
   1
   identity
   2
   inc
   (dec n)))
   

(sum_prime_squares 1 5)
(sum_prime_squares 3 6)
(newline)
(product_relative_primes 1)
(product_relative_primes 2)
(product_relative_primes 3)
(product_relative_primes 5)
(product_relative_primes 10)
(newline)
  
;(define (sum term a next b) (sum_iter term a next b))
(define (sum term a next b)
  (accumulate + 0 term a next b))

;(define (product term a next b) (product_iter term a next b))
(define (product term a next b)
  (accumulate * 1 term a next b))

(define (fact_gen n)
  (product identity 1 inc n))

(define (pi_prod n)
  (define (term k)
    (/ (* k (+ k 2.0)) (* (+ k 1) (+ k 1))))
;  (define (next k)
;    (+ k 2))
  (product term 2 (lambda (k) (+ k 2)) n))

(* 4 (pi_prod 100))
(* 4 (pi_prod 101))


(fact_gen 3)
(fact_gen 5)
(fact_gen 6)
(fact_gen 7)
(newline)

(define (cube x) (* x x x))

(define (sum_ints a b)
  (if (> a b)
      0
      (+ a (sum_ints (inc a) b))))

(define (sum_ints_gen a b)
  (sum identity a inc b))

(define (sum_cubes a b)
  (if (> a b)
      0
      (+ (cube a) (sum_cubes (inc a) b))))

(define (sum_cubes_gen a b)
  (sum cube a inc b))

(define (sum_pi a b)
  (if (> a b)
      0
      (+ (/ 1.0 (* a (+ a 2))) (sum_pi (+ a 4) b))))

(define (sum_pi_gen a b)
  (define (term a)
    (/ 1.0 (* a (+ a 2))))
  (define (next a)
    (+ a 4))
  (sum term a next b))

(define (integral_gen f a b dx)
  (define (add_dx x)
    (+ x dx))
  (* dx (sum f (+ a (/ dx 2.0)) add_dx b)))

(define (simpson_sum f a b n)
  (define h (/ (- b a) n))
  (define (next x) (+ x h h))
  (* (+ (f a)
        (* 2 (sum f a next b))
        (* 4 (sum f (+ a h) next b))
        (f b))
     (/ h 3)))


(sum_ints_gen 1 10)
(sum_cubes_gen 1 10)
(* 8 (sum_pi_gen 1 10000))
(integral_gen cube 0 1.0 0.001)
(simpson_sum cube 0 1.0 1000)




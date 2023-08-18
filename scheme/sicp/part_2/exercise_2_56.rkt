#lang sicp

(define (variable? x) (symbol? x))

(define (same-variable? x y)
  (and (variable? x) (variable? y) (eq? x y)))

(define (=number? expr num)
  (and (number? expr) (= expr num)))

(define (make-sum a1 a2)
  (cond ((=number? a1 0) a2)
        ((=number? a2 0) a1)
        ((and (number? a1) (number? a2)) (+ a1 a2))
        (else
         (list '+ a1 a2))))

(define (sum? expr) (and (pair? expr) (eq? (car expr) '+)))

(define (addend s) (cadr s))
;(define (augend s) (caddr s))
(define (augend s)
  (if (null? (cdddr s)) (caddr s)
      (cons '+ (cddr s))))
  
(define (make-product m1 m2)
  (cond ((or (=number? m1 0) (=number? m2 0)) 0)
        ((=number? m1 1) m2)
        ((=number? m2 1) m1)
        ((and (number? m1) (number? m2)) (* m1 m2))
        (else (list '* m1 m2))))

(define (product? expr) (and (pair? expr) (eq? (car expr) '*)))

(define (multiplier p) (cadr p))
;(define (multiplicand p) (caddr p))
(define (multiplicand p)
  (if (null? (cdddr p)) (caddr p)
      (cons '* (cddr p))))

(define (make-exponentiation base exponent)
  (cond ((=number? exponent 0) 1)
        ((=number? exponent 1) base)
        ((and (number? base) (number? exponent)) (expt base exponent))
        (else (list '** base exponent))))

(define (exponentiation? expr) (and (pair? expr) (eq? (car expr) '**)))

(define (base expr) (cadr expr))
(define (exponent expr) (caddr expr))

(define (deriv expr var)
  (cond ((number? expr) 0)
        ((variable? expr)
         (if (same-variable? expr var)
             1
             0))
         ((sum? expr) (make-sum (deriv (addend expr) var)
                                (deriv (augend expr) var)))
         ((product? expr)
          (let ((m1 (multiplier expr))
                (m2 (multiplicand expr)))
            (make-sum
             (make-product m1 (deriv m2 var))
             (make-product (deriv m1 var) m2))))
         ((exponentiation? expr)
          (let ((b (base expr))
                (n (exponent expr)))
            (make-product
             (make-product n (make-exponentiation b (make-sum n -1)))
             (deriv b var))))
         (else (error "Unknown expression type: DERIV" expr))))

;(deriv '(+ x 3) 'x)
;(deriv '(* x y) 'x)
;(deriv '(* (* x y) (+ x 3)) 'x)
;(deriv '(not an expression) 'x)
;(deriv (make-exponentiation 'x 7) 'x)
;(deriv (make-exponentiation 'x '2y) 'x)
(define expr1 '(+ x y 3 x))
(define expr2 '(* x y (+ x 3)))
;(sum? expr1)
;(sum? expr2)
;(null? (cdddr expr1))
;(null? (cdddr expr2))

(deriv expr1 'x)
(deriv expr2 'x)


;(define s1 (make-sum 'x '(y 3)))
;s1
;(cddr s1)

;(deriv s1 'x)










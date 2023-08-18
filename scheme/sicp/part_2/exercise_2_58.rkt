#lang sicp

(define (memq item x)
  (cond ((null? x) false)
        ((eq? item (car x)) x)
        (else (memq item (cdr x)))))

(define (memr item x)
  (if (pair? x)
      (reverse (memq item (reverse x)))
      x))

(define (atomic? x) (null? (cdr x)))

(define (variable? x) (symbol? x))

(define (same-variable? x y)
  (and (variable? x) (variable? y) (eq? x y)))

(define (=number? expr num)
  (and (number? expr) (= expr num)))

(define (list-head expr pos)
  (if (= pos 0)
      '()
      (cons (car expr)
            (list-head (cdr expr) (- pos 1)))))

(define (lowest-priority expr)
  (cond ((memq '+ expr) '+)
        ((memq '* expr) '*)
        ((memq '** expr) '**)
        (else (error "Expression contains no operand: LOWEST-PRIORITY" expr))))

(define (make-sum a1 a2)
  (cond ((=number? a1 0) a2)
        ((=number? a2 0) a1)
        ((and (number? a1) (number? a2)) (+ a1 a2))
        (else (list a1 '+ a2))))

(define (make-product m1 m2)
  (cond ((or (=number? m1 0) (=number? m2 0)) 0)
        ((=number? m1 1) m2)
        ((=number? m2 1) m1)
        ((and (number? m1) (number? m2)) (* m1 m2))
        (else (list m1 '* m2))))

(define (make-exponentiation base exponent)
  (cond ((=number? exponent 0) 1)
        ((=number? exponent 1) base)
        ((and (number? base) (number? exponent)) (expt base exponent))
        (else (list base '** exponent))))

(define (has-op? expr op) (eq? (lowest-priority expr) op))

(define (sum? expr) (has-op? expr '+))
(define (product? expr) (has-op? expr '*))
(define (exponentiation? expr) (has-op? expr '**))

(define (get-first-operand-from-list expr op)
  (list-head expr (- (length expr)
                     (length (memq op expr)))))

(define (choose-first-arg expr op)
  (let ((a
         (list-head expr (- (length expr)
                            (length (memq op expr))))))
;         (get-first-operand expr op)))
    (if (atomic? a) (car a) a)))

;(define (addend expr) (choose-first-arg expr '+))
;(define (multiplier expr) (choose-first-arg expr '*))
;(define (base expr) (choose-first-arg expr '**))

(define (get-first-operand expr op)
  (let ((result (reverse (cdr (reverse (memr op expr))))))
    (if (= (length result) 1)
        (car result)
        result)))

(define (get-second-operand expr op)
  (let ((result (cdr (memq op expr))))
    (if (= (length result) 1)
        (car result)
        result)))

(define (addend expr) (get-first-operand expr '+))
(define (augend expr) (get-second-operand expr '+))

(define (multiplier expr) (get-first-operand expr '*))
(define (multiplicand expr) (get-second-operand expr '*))

(define (base expr) (get-first-operand expr '**))
(define (exponent expr) (get-second-operand expr '**))

(define (choose-second-arg expr op)
    (if (atomic? (cdr (memq op expr))) (car expr) expr))

;(define (augend expr) (choose-second-arg expr '+))
;(define (multiplicand expr) (choose-second-arg expr '*))
;(define (exponent expr) (choose-second-arg expr '**))

(define (deriv expr var)
  (cond ((number? expr) 0)
        ((variable? expr)
         (if (same-variable? expr var) 1 0))
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


(deriv '(x + 3) 'x)
(deriv '(x * y) 'x)
(deriv '(x + (3 * (x + (y + 2)))) 'x)
(deriv '(x + x * (x + y + 2)) 'x)
(deriv '(x + x * (x + y + 2)) 'y)













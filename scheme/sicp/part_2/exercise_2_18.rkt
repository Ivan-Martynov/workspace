#lang sicp

(define (reverse_iter a)
  (define (iter i)
    (if (< i 0)
        nil
        (cons (list-ref a i) (iter (dec i)))))
  (iter (dec (length a))))

(define (reverse_rec a)
  (if (null? a)
      nil
      (cons (reverse_rec (cdr a)) (car a))))

(define (reverse a)
  (if (null? a)
      a
      (append (reverse (cdr a)) (list (car a)))))

(reverse_rec (list 1 4 9 5))
(reverse_iter (list 1 4 9 5))
(reverse (list 1 4 9 5))
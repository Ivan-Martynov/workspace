#lang sicp

(define (make-mobile left right)
  ;(list left right))
  (cons left right))

(define (make-branch length structure)
  ;(list length structure))
  (cons length structure))

(define (left-branch mobile)
  (car mobile))
(define (right-branch mobile)
  ;(car (cdr mobile)))
  (cdr mobile))

(define (branch-length branch)
  (car branch))
(define (branch-structure branch)
  ;(car (cdr branch)))
  (cdr branch))

(define (branch-weight branch)
   (if (pair? (branch-structure branch))
       (total-weight (branch-structure branch))
       (branch-structure branch)))

(define (total-weight mobile)
  (+ (branch-weight (left-branch mobile))
     (branch-weight (right-branch mobile))))

(define (torque branch)
  (* (branch-length branch) (branch-weight branch)))

(define (branch-balanced? branch)
  (if (pair? (branch-structure branch))
      (balanced? (branch-structure branch))
      true))

(define (balanced? mobile)
  (and (= (torque (left-branch mobile)) (torque (right-branch mobile)))
  (branch-balanced? (left-branch mobile))
  (branch-balanced? (right-branch mobile))))

(define (debug-mobile mobile)
  (display mobile)
  (newline)
  (display "Total weight = ")
  (display (total-weight mobile))
  (if (balanced? mobile)
      (display ", is balanced")
      (display ", is not balanced"))
  (newline))

(define a (make-mobile (make-branch 2 3) (make-branch 2 3)))
(define b (make-mobile (make-branch 7 6) (make-branch 8 5)))
(define c (make-mobile (make-branch 5 a) (make-branch 3 b)))
(define d (make-mobile (make-branch 10 a) (make-branch 12 5)))

(debug-mobile a)
(debug-mobile b)
(debug-mobile c)
(debug-mobile d)










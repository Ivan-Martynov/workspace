#lang sicp

(define (square x) (* x x))

(define (square-tree_iter tree)
  (cond ((null? tree) tree)
        ((pair? tree)
         (cons (square-tree_iter (car tree))
               (square-tree_iter (cdr tree))))
        (else
         (square tree))))

(define (square-tree-map tree)
  (map (lambda (sub-tree)
         (if (pair? sub-tree)
             (square-tree-map sub-tree)
             (square sub-tree)))
       tree))

(define (tree-map proc tree)
  (map (lambda (sub-tree)
         (if (pair? sub-tree)
             (tree-map proc sub-tree)
             (proc sub-tree)))
       tree))

(define (square-tree tree)
  (tree-map square tree))  

(define t (list 1 (list 2 (list 3 4) 5) (list 6 7) 10))
(square-tree t)
(tree-map (lambda (x) (sqrt x)) t)

(square-tree (list 1 (list 2 (list 3 4) 5) (list 6 7)))
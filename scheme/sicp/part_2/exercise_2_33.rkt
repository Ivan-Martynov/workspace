#lang sicp

(define (accumulate op initial sequence)
  (if (null? sequence)
      initial
      (op (car sequence)
          (accumulate op initial (cdr sequence)))))

;(define (map p sequence)
;  (accumulate (lambda (x y) (cons (p x) y)) nil sequence))

(define (append seq1 seq2)
  (accumulate cons seq2 seq1))

(define (length sequence)
  (accumulate (lambda (x y) (+ 1 y)) 0 sequence))

(define (horner-eval x coefficient-sequence)
  (accumulate (lambda (this-coeff higher-terms) (+ this-coeff (* x higher-terms)))
              0
              coefficient-sequence))

(define (enumerate-tree tree)
  (cond ((null? tree) nil)
        ((not (pair? tree)) (list tree))
        (else (append (enumerate-tree (car tree))
                      (enumerate-tree (cdr tree))))))

(define (count-leaves t)
  (accumulate + 0 (map (lambda (x) 1) (enumerate-tree t))))

(define (accumulate-n op init seqs)
  (if (null? (car seqs))
      nil
      (cons (accumulate op init (map car seqs))
            (accumulate-n op init (map cdr seqs)))))

             
(map sqrt (list 4 9.0 25))
(append (list 1 8) (list 2 9 7))
(length (list 1 8 0 2 8 4))
(horner-eval 2 (list 1 3 0 5 0 1))

(enumerate-tree (list 1 (list 2 (list 3 4)) 5))
(count-leaves (list 1 (list 2 (list 3 4)) 5))

(define z (list (list 1 2 3) (list 4 5 6) (list 7 8 9) (list 10 11 12)))
(accumulate-n + 0 z)

(define (dot-product v w)
  (accumulate + 0 (map * v w)))

(define (matrix-*-vector m v)
  (map (lambda (row) (dot-product row v)) m))

(define (transpose mat)
  (accumulate-n cons nil mat))

(define (matrix-*-matrix m n)
  (let ((cols (transpose n)))
    (map (lambda (row) (matrix-*-vector cols row)) m)))

(define v1 (list 1 2 3 4))
(define v2 (list 5 6 7 8))
(define m (list (list 1 2 3 4) (list 4 5 6 6) (list 6 7 8 9)))
(map * v1 v2)
(dot-product v1 v2)
(matrix-*-vector m v1)
(matrix-*-vector m v2)
(transpose m)
(matrix-*-matrix m (transpose m))
(matrix-*-matrix m (transpose (list (list 1 1 1 1) (list -1 -1 -1 -1))))

(define (fold-right op init seq)
  (accumulate op init seq))

(define (fold-left op initial sequence)
  (define (iter result rest)
    (if (null? rest)
        result
        (iter (op result (car rest))
              (cdr rest))))
  (iter initial sequence))

(define (reverse-right sequence)
  (fold-right (lambda (x y) (append y (list x))) nil sequence))

(define (reverse-left sequence)
  (fold-left (lambda (x y) (append (list y) x)) nil sequence))


(reverse-left (list 1 2 3 4 5 6 7 8 9 0))
(reverse-right (list 1 2 3 4 5 6 7 8 9 0))













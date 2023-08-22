#lang sicp

(define (make-leaf symbol weight) (list 'leaf symbol weight))

(define (leaf? object) (eq? (car object) 'leaf))

(define (symbol-leaf x) (cadr x))

(define (weight-leaf x) (caddr x))

(define (left-branch tree) (car tree))
(define (right-branch tree) (cadr tree))

(define (symbols tree)
  (if (leaf? tree)
      (list (symbol-leaf tree))
      (caddr tree)))

(define (weight tree)
  (if (leaf? tree)
      (weight-leaf tree)
      (cadddr tree)))

(define (make-code-tree left right)
  (list
   left
   right
   (append (symbols left) (symbols right))
   (+ (weight left) (weight right))))

(define (choose-branch bit branch)
  (cond ((= bit 0) (left-branch branch))
        ((= bit 1) (right-branch branch))
        (else (error "bad bit: CHOOSE-BRANCH" bit))))

(define (decode bits tree)
  (define (decode-1 bits current-branch)
    (if (null? bits)
        '()
        (let ((next-branch
               (choose-branch (car bits) current-branch)))
          (if (leaf? next-branch)
              (cons (symbol-leaf next-branch)
                    (decode-1 (cdr bits) tree))
              (decode-1 (cdr bits) next-branch)))))
  (decode-1 bits tree))

(define (adjoin-set x set)
  (cond ((null? set) (list x))
        ((< (weight x) (weight (car set))) (cons x set))
        (else (cons (car set) (adjoin-set x (cdr set))))))

(define (make-leaf-set pairs)
  (if (null? pairs)
      '()
      (let ((pair (car pairs)))
        (adjoin-set (make-leaf (car pair)   ; symbol
                               (cadr pair)) ; frequency
                    (make-leaf-set (cdr pairs))))))

(define (element-of-set? x set)
  (cond ((null? set) false)
        ((eq? x (car set)) true)
        (else (element-of-set? x (cdr set)))))

(define (encode-symbol symbol tree)
  (cond ((null? tree) (error "Empty tree: ENCODE-SYMBOL" symbol))
         ((leaf? tree)
         (if (not (eq? symbol (symbol-leaf tree)))
             (error "No such symbol in the tree: ENCODE-SYMBOL" symbol)
             '()))
         ((element-of-set? symbol (symbols (left-branch tree)))
          (append (list 0) (encode-symbol symbol (left-branch tree))))
         ((element-of-set? symbol (symbols (right-branch tree)))
          (append (list 1) (encode-symbol symbol (right-branch tree))))
         (else
          (error "No such symbol in the tree: ENCODE-SYMBOL" symbol))))

(define (encode message tree)
  (if (null? message)
      '()
      (append (encode-symbol (car message) tree)
              (encode (cdr message) tree))))

(define (successive-merge sorted-pairs)
  ;(define (merge-iter result)
  ;  (adjoin-set x set))
  (let ((left (car sorted-pairs))
        (right (cadr sorted-pairs)))
    (make-code-tree left right)))

(define sample-tree
  (make-code-tree (make-leaf 'A 4)
                  (make-code-tree
                   (make-leaf 'B 2)
                   (make-code-tree
                    (make-leaf 'D 1)
                    (make-leaf 'C 1)))))

(define sample-message '(0 1 1 0 0 1 0 1 0 1 1 1 0))

sample-tree
sample-message

(decode sample-message sample-tree)
(symbols sample-tree)
;(weight sample-tree)
;(element-of-set? 'K (symbols sample-tree))
;(encode-symbol 'D sample-tree)

(encode '(A D A B B C A) sample-tree)
(encode '(A) (make-leaf 'A 1))

(define pairs (make-leaf-set (list '(A 4) '(B 2) '(D 1) '(C 1))))
(define pairs1 (make-leaf-set (list '(A 4) '(B 3) '(C 2) '(D 2))))
(define pairs2 (make-leaf-set (list '(A 4) '(B 3))))
pairs2
(null? (car pairs2))
(null? (cdr pairs2))
(successive-merge pairs2)

















#lang sicp

(#%require (planet "sicp.ss" ("soegaard" "sicp.plt" 2 1)))

(define (square-of-four tl tr bl br)
  (lambda (p)
    (let ((top (beside (tl p) (tr p)))
          (bottom (beside (bl p) (br p))))
      (below bottom top))))

(define (flipped-pairs painter)
  ((square-of-four identity flip-vert identity flip-vert) painter))

(define (split op1 op2)
  (lambda (painter n)
    (if (= n 0)
        painter
        (let ((smaller ((split op1 op2) painter (- n 1))))
        (op1 painter (op2 smaller smaller))))))

(define right-split (split beside below))
(define up-split (split below beside))

(define (corner-split painter n)
  (if (= n 0)
      painter
      (let ((up (up-split painter (- n 1)))
            (right (right-split painter (- n 1))))
        (let ((top-left (beside up up))
        ;(let ((top-left up)
              (bottom-right (below right right))
              ;(bottom-right right)
              (corner (corner-split painter (- n 1))))
          (beside (below painter top-left)
                  (below bottom-right corner))))))

(define (square-limit painter n)
  ((square-of-four flip-vert identity rotate180 flip-horiz)
   (corner-split painter n)))

(define pic einstein)
;(paint (square-limit pic 4))
;(paint (up-split pic 2))
;(paint (right-split pic 2))
;(paint (corner-split pic 2))
(paint (square-limit pic 2))












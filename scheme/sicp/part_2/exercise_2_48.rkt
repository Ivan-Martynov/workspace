#lang sicp

(define (make-vect x y) (list x y))
(define (xcor-vect v) (car v))
(define (ycor-vect v) (cadr v))

(define (elementwise-vec-op op)
  (lambda (v1 v2)
    (make-vect (op (xcor-vect v1) (xcor-vect v2))
            (op (ycor-vect v1) (ycor-vect v2)))))

(define add-vect (elementwise-vec-op +))
(define sub-vect (elementwise-vec-op -))

(define (scale-vect s v) (map (lambda (a) (* a s)) v))

(define (make-segment start-vector end-vector) (list start-vector end-vector))
(define (start-segment seg) (car seg))
(define (end-segment seg) (cadr seg))

(define (draw-line first-point second-point) true)

(define (make-frame origin edge1 edge2) (list origin edge1 edge2))

(define (origin-frame frame) (car frame))
(define (edge1-frame frame) (cadr frame))
(define (edge2-frame frame) (caddr frame))

(define (frame-coord-map frame)
  (lambda (v)
    (add-vect (origin-frame frame)
              (add-vect (scale-vect (xcor-vect v) (edge1-frame frame))
                        (scale-vect (ycor-vect v) (edge2-frame frame))))))

(define (segments->painter segment-list)
  (lambda (frame)
    (for-each
     (lambda (segment)
       (draw-line
        ((frame-coord-map frame)
         (start-segment segment))
        ((frame-coord-map frame)
         (end-segment segment))))
     segment-list)))


(define vec-0-0 (make-vect 0.0 0.0))
(define vec-0-1 (make-vect 0.0 1.0))
(define vec-1-0 (make-vect 1.0 0.0))
(define vec-1-1 (make-vect 1.0 1.0))
(define outline (segments->painter (list
                    (make-segment vec-0-0 vec-0-1)
                    (make-segment vec-0-1 vec-1-1)
                    (make-segment vec-1-1 vec-1-0)
                    (make-segment vec-1-0 vec-0-0))))

(define cross-X (segments->painter (list
                    (make-segment vec-0-0 vec-1-1)
                    (make-segment vec-0-1 vec-1-0))))

(define diamond-shape (segments->painter (list
                    (make-segment (make-vect 0.0 0.5) (make-vect 0.5 1.0))
                    (make-segment (make-vect 0.5 1.0) (make-vect 1.0 0.5))
                    (make-segment (make-vect 1.0 0.5) (make-vect 0.5 0.0))
                    (make-segment (make-vect 0.5 0.0) (make-vect 0.0 0.5)))))







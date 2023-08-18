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

(define (make-frame-list origin edge1 edge2)
  (list origin edge1 edge2))

(define (make-frame-pair origin edge1 edge2)
  (cons origin (cons edge1 edge2)))

(define make-frame make-frame-list)

(define (origin-frame frame)
  (car frame))

(define (edge1-frame frame) (cadr frame))

(define (edge2-frame-list frame) (caddr frame))
(define (edge2-frame-pair frame) (cddr frame))

(define edge2-frame edge2-frame-list)

(define vec1 (make-vect 1.0 2.5))
(define vec2 (make-vect 3.5 4.0))
vec1
vec2
(add-vect vec1 vec2)
(sub-vect vec1 vec2)
(scale-vect 2 vec1)

(define (frame-coord-map frame)
  (lambda (v)
    (add-vect (origin-frame frame)
              (add-vect (scale-vect (xcor-vect v) (edge1-frame frame))
                        (scale-vect (ycor-vect v) (edge2-frame frame))))))

(define frame-list1 (make-frame-list (make-vect 5 7) vec1 vec2))
(define frame-pair1 (make-frame-pair (make-vect 5 7) vec1 vec2))
"Frames"
frame-list1
frame-pair1
"Origins"
(origin-frame frame-list1)
(origin-frame frame-pair1)
"First edges"
(edge1-frame frame-list1)
(edge1-frame frame-pair1)
"Second edges"
(edge2-frame-list frame-list1)
(edge2-frame-pair frame-pair1)
"Map a frame"
((frame-coord-map frame-list1) (make-vect 0 0))
















#lang sicp

(define (square x) (* x x))

(define (is_divisible a b)
  (= (remainder b a) 0))

(define (find_divisor n value)
  (cond ((> (square value) n) n)
        ((is_divisible value n) value)
        (else (find_divisor n (inc value)))))

(define (smallest_divisor n)
  (find_divisor n 2))

(define (prime? n)
  (= n (smallest_divisor n)))

(define (accumulate op initial sequence)
  (if (null? sequence)
      initial
      (op (car sequence)
          (accumulate op initial (cdr sequence)))))

(define (flatmap proc seq)
  (accumulate append nil (map proc seq)))

(define (prime-sum? pair)
  (prime? (+ (car pair) (cadr pair))))

(define (enumerate-interval low high)
  (if (> low high)
      nil
      (cons low (enumerate-interval (+ low 1) high))))

(define (make-pair-sum pair)
  (let ((a (car pair))
        (b (cadr pair)))
    (list a b (+ a b))))
    
(define (filter pred items)
  (if (null? items)
      nil
      (if (pred (car items))
          (cons (car items) (filter pred (cdr items)))
          (filter pred (cdr items)))))

(define (unique-pairs n)
  (flatmap
   (lambda (i)
     (map (lambda (j) (list i j))
          (enumerate-interval 1 (- i 1))))
   (enumerate-interval 1 n)))

(define (unique-triples n)
  (flatmap
   (lambda (i)
     (flatmap (lambda (j)
                (map (lambda (k) (list i j k))
                     (enumerate-interval 1 (- j 1))))
              (enumerate-interval 1 (- i 1))))
   (enumerate-interval 1 n)))


(define (sum-to-num? triple num)
  (let ((a (car triple))
        (b (cadr triple))
        (c (caddr triple)))
    (= (+ a b c) num)))

(define (prime-sum-pairs n)
  (map make-pair-sum (filter prime-sum? (unique-pairs n))))

(define (sum-triples n num)
  (filter (lambda (x) (sum-to-num? x num)) (unique-triples n)))

(define (permutations items)
  (if (null? items)
      (list nil)
      (flatmap (lambda (x)
                 (map (lambda (p) (cons x p))
                      (permutations (remove x items))))
               items)))

(define (remove x items)
  (filter (lambda (y) (not (= y x))) items))

(define (adjoin-position row col positions) (cons (list row col) positions))
;(define (adjoin-position row col positions) (append positions (list (cons row col))))

(define (position-row pos) (car pos))
;(define (position-col pos) (cdr pos))
(define (position-col pos) (cadr pos))

(define empty-board nil)

(define (attacks? q1 q2)
  (or
  (= (position-row q1) (position-row q2))
 ; (= (position-col q1) (position-col q2)) ; not necessary as the column is already different.
  (= (abs (- (position-row q1) (position-row q2)))
     (abs (- (position-col q1) (position-col q2))))))

(define (safe? k positions)
  (define (iter q board)
    (or (null? board)
        (and (not (attacks? q (position-row board)))
             (iter q (cdr board)))))
  (let ((kth_queen
         (position-row (filter (lambda (q) (= (position-col q) k))
                               positions)))
        (other-queens (filter (lambda (q) (not (= (position-col q) k))) positions)))
    (if (null? other-queens)
        true
        (accumulate (lambda (a b) (and a b)) true
                    (map (lambda (q)
                           (not (attacks? q kth_queen)))
                         other-queens)))))
    ;(iter kth_queen other-queens)))
    

(define (queens board-size)
  (define (queen-cols k)
    (if (= k 0)
        (list empty-board)
        (filter
         (lambda (positions) (safe? k positions))
         (flatmap
          (lambda (rest-of-queens)
            (map (lambda (new-row)
                   (adjoin-position
                    new-row k rest-of-queens))
                 (enumerate-interval 1 board-size)))
          (queen-cols (- k 1))))))
  (queen-cols board-size))

;(prime-sum-pairs 6)
;(permutations (list 1 2 3))
;"Unique pairs"
;(unique-pairs 4)
;(sum-to-num? (list 1 2 3) 5)
;(sum-triples 6 8)
(queens 4)










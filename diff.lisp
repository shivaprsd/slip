;;; Boolean NOT
(defun not (a)
  (cond (a '()) ('t 't)))

;;; Check if lst is an empty list or not
(defun null (lst)
  ;; Our implementation of eq will warn if the
  ;; argument is non-atomic, so check that
  (cond ((atom lst) (eq lst '()))
        ('t '())))

;;; Check if two lists m & n have the same
;;; car-cdr structure
(defun equal (m n)
  (cond ((atom m)
         (cond ((atom n) (eq m n)) ('t '())))
        ((not (atom m))
         (cond ((not (atom n))
                (cond ((equal (car m) (car n))
                       (equal (cdr m) (cdr n)))
                      ('t '())))
               ('t '())))
        ('t '())))

;;; Apply f to a list l and its sub-lists in turn
(defun maplist (l f)		; note the order of params
  (cond ((null l) '())		; reached end of list
        ('t (cons (f l) (maplist (cdr l) f)))))

;;; Differentiate y w.r.t. x
(defun diff (y x)
  (cond ((atom y)
         (cond ((eq y x) 'ONE)
               ('t 'ZERO)))
        ((eq (car y) 'PLUS)
         (cons 'PLUS
               (maplist (cdr y)
                        '(lambda (z) (diff (car z) x)))))
        ((eq (car y) 'TIMES)
         (cons 'PLUS
               (maplist
                 (cdr y)
                 '(lambda (z)
                    (cons 'TIMES
                          (maplist
                            (cdr y)
                            '(lambda (w)
                               (cond ((not (equal z w)) (car w))
                                     ('t (diff (car w) x))))))))))))

;;; Calculate the derivative of an expression
(diff '(TIMES X (PLUS X A) Y) 'X)

;;; Expected output:
;;;(PLUS (TIMES ONE (PLUS X A) Y) (TIMES X (PLUS ONE ZERO) Y) (TIMES X (PLUS X A) ZERO))

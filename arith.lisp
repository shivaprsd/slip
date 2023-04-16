; Binary arithmetic using T and NIL
(bind O nil)
(bind I t)

; Boolean operators
(defmac and (x y)
        (list 'cond
              (list x (list 'cond (list y t) (list t nil)))
              (list t nil)))

(defmac or (x y)
        (list 'cond
              (list x t)
              (list t (list 'cond (list y t) (list t nil)))))

(defmac not (x) (list 'cond (list x nil) (list t t)))

(defun xor (x y)
  (or (and x (not y)) (and (not x) y)))

; (defun hadd (a b)
;   (list (and a b) (xor a b)))

(defun fadd (a b c)
  (list (or (and a b) (or (and b c) (and c a)))
        (xor a (xor b c))))

(defun null (x)
  (and (atom x) (eq x nil)))

(defun append (l m)
  (cond ((null l) m)
        (t (cons (car l) (append (cdr l) m)))))

(defun plus (numA numB)
  (cond ((null numA) (list O))
        (t ((lambda (fsum)
              (append (fadd (car numA) (car numB) (car fsum)) (cdr fsum)))
            (plus (cdr numA) (cdr numB))))))

(defun compli (num)
  (cond ((null num) nil)
        (t (cons (not (car num))
                 (compli (cdr num))))))

(defun int (num)
  ((label pad (lambda (x y z)
                (cond ((null x) (append y z))
                      (t (pad (cdr x) (cdr y) z)))))
   num (list  O O O O  O O O O  O O O O  O O O O) num))

(defun digit (n)
  (cond
    ((eq n 'zero)	(int nil))
    ((eq n 'one)	(int (list I)))
    ((eq n 'two)	(incf (digit 'one)))
    ((eq n 'three)	(incf (digit 'two)))
    ((eq n 'four)	(incf (digit 'three)))
    ((eq n 'five)	(incf (digit 'four)))
    ((eq n 'six)	(incf (digit 'five)))
    ((eq n 'seven)	(incf (digit 'six)))
    ((eq n 'eight)	(incf (digit 'seven)))
    ((eq n 'nine)	(incf (digit 'eight)))))

(defun incf (num)
  (add num (digit 'one)))

(defun decf (num)
  (add num (minus (digit 'one))))

(defun minus (num)
  (incf (compli num)))

(defun add (numA numB)
  (cdr (plus numA numB)))

(defun subt (numA numB)
  (add numA (minus numB)))

(defun zerop (num)
  (cond ((null num) t)
        (t (and (not (car num)) (zerop (cdr num))))))

(defun double (num)
  (append (cdr num) (list O)))

(defun reverse (lst)
  ((label rev (lambda (old new)
                (cond ((null old) new)
                      (t (rev (cdr old) (cons (car old) new))))))
   lst nil))

(defun half (num)
  (cons O (reverse (cdr (reverse num)))))

(defun last (lst)
  (cond ((null (cdr lst)) (car lst))
        (t (last (cdr lst)))))

(defun mult (numA numB)
  ((label multaccum
          (lambda (numA numB prod)
            (cond ((zerop numB) prod)
                  (t (multaccum (double numA)
                                (half numB)
                                (cond ((last numB) (add numA prod))
                                      (t prod)))))))
   numA numB (digit 'zero)))

(defun fact (n)
  (cond ((zerop n) (digit 'one))
        (t (mult n (fact (decf n))))))

(defun multen (n)
  (double (mult n (digit 'five))))

(defun numb (lst)
  ((label numacc
          (lambda (n acc)
            (cond ((null n) acc)
                  (t (numacc (cdr n)
                             (add (multen acc) (digit (car n))))))))
   lst (digit 'zero)))

(add (numb '(four seven)) (numb '(three one)))
; (fact (digit 'four))
; Expected output (100! = 11000):
;(t t nil nil nil)

;;; The core init file, that will create a shell around our C kernel ;;;

;;; Make truth values self-evaluating
(bind t 't)
(bind nil 'nil)

;;; Bind *defmac* to a macro that binds a symbol to a macro
;;; (defmac msym slots exp) => (bind msym '(macro slots exp))
(bind defmac
      '(macro (msym slots exp)
              (list 'bind msym (list 'quote (list 'macro slots exp)))))

;;; *defun*, our second macro, binds a symbol to a function
;;; (defun fsym args body) => (bind fsym '(label fsym (lambda args body)))
(defmac defun (fsym args body)
        (list 'bind fsym
              (list 'quote (list 'label fsym (list 'lambda args body)))))

;;; Now start defining functions and start Lisping!


;; command: ../scheme -
;; command timeout 1.0

;; description: null?
;; command expect success
;; feed
(write (null? '()))
(write (null? 'a))
(write (null? 123))
(define var '())
(write (null? var))
;; check
#t
#f
#f
#t
;; pause check
;; run command

;; description: null? too few args
;; command expect failure
;; feed
(null?)
;; pause feed
;; run command

;; description: null? too much args
;; command expect failure
;; feed
(null? 1 2)
;; pause feed
;; run command



;; description: list?
;; command expect success
;; feed
(write (list? '()))
(write (list? '(1 2 . 3)))
(write (list? 123))
(define var '())
(write (list? var))
;; check
#t
#t
#f
#t
;; pause check
;; run command

;; description: list? too few args
;; command expect failure
;; feed
(list?)
;; pause feed
;; run command

;; description: list? too much args
;; command expect failure
;; feed
(list? '() '(1))
;; pause feed
;; run command



;; description: pair?
;; command expect success
;; feed
(write (pair? '(1 . 2)))
(write (pair? '(1 . (1 2))))
(write (pair? 123))
(define var '(4 . 2))
(write (pair? var))
;; check
#t
#t
#f
#t
;; pause check
;; run command

;; description: pair? too few args
;; command expect failure
;; feed
(pair?)
;; pause feed
;; run command

;; description: pair? too much args
;; command expect failure
;; feed
(pair? '(1 . 2) '("a" . (1)))
;; pause feed
;; run command


;; description: list->string 
;; command expect success
;; feed
(write (list->string '(#\a)))
(write (list->string '(#\a #\b #\c)))
;; check
"a"
"abc"
;; pause check
;; run command

;; description: list->string () case
;; command expect success
;; feed
(write (list->string '()))
;; check
""
;; pause check
;; run command

;; description: list->string no args
;; command expect failure
;; feed
(list->string)
;; pause feed
;; run command

;; description: list->string too much args
;; command expect failure
;; feed
(list->string '(#\a) '())
;; pause feed
;; run command

;; description: list->string improper list
;; command expect failure
;; feed
(list->string '(#\a . #\b))
;; pause feed
;; run command

;; description: list->string not a list
;; command expect failure
;; feed
(list->string 'symbol)
;; pause feed
;; run command



;; description: cons
;; command expect success
;; feed
(write (cons 1 2))
(write
 (cons
  (cons 2 '())
  '()))
(define va #\y)
(define vb #\z)
(write (cons va vb))
;; check
(1 . 2)
((2))
(#\y . #\z)
;; pause check
;; run command

;; description: cons not enough args
;; command expect failure
;; feed
(cons)
;; pause feed
;; run command

;; description: cons not enough args 2
;; feed
(cons 1)
;; pause feed
;; run command

;; description: cons too much args
;; feed
(cons 1 2 3)
;; pause feed
;; run command


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
;; run command

;; description: null? too much args
;; command expect failure
;; feed
(null? 1 2)
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
;; run command

;; description: list? too much args
;; command expect failure
;; feed
(list? '() '(1))
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
;; run command

;; description: pair? too much args
;; command expect failure
;; feed
(pair? '(1 . 2) '("a" . (1)))
;; run command

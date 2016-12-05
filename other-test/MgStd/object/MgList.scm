
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

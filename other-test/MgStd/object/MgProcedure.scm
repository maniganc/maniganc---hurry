
;; command: ../scheme -
;; command timeout 1.0

;; description: procedure?
;; command expect success
;; feed
(write (procedure? #t))
(write (procedure? 1))
;; check
#f
#f
;; pause check
;; run command

;; description: procedure?
;; command expect success
;; feed
(write (procedure? define))
(write (procedure? let))
;; check
#t
#t
;; pause check
;; run command

;; description: procedure?
;; command expect success
;; feed
(write (procedure? (lambda (x) ())))
(define (f x y) (list x y))
(write (procedure? f))
;; check
#t
#t
;; pause check
;; run command

;; description: procedure? too few args
;; command expect failure
;; feed
(procedure?)
;; pause feed
;; run command

;; description: procedure? too much args
;; command expect failure
;; feed
(procedure? 1 2)
;; pause feed
;; run command

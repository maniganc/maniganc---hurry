
;; command: ../scheme -
;; command timeout 1.0

;; description: integer?
;; command expect success
;; feed
(write (integer? #t))
(write (integer? #f))
(write (integer? 'a))
(write (integer? 123))
;; check
#f
#f
#f
#t
;; pause check
;; run command

;; description: integer? too few args
;; command expect failure
;; feed
(integer?)
;; pause feed
;; run command

;; description: integer? too much args
;; command expect failure
;; feed
(integer? 1 2)
;; pause feed
;; run command



;; description: integer->char
;; command expect success
;; feed
(write (integer->char 48))	;; 0
(write (integer->char 97))	;; a
(write (integer->char 124))	;; |
(write (integer->char 32))	;; space
(define var 98)		        ;; b
(write (integer->char var))
;; check
#\0
#\a
#\|
#\space
#\b
;; pause check
;; run command

;; description: integer->char < 0
;; command expect failure
;; feed
(integer->char -1)
;; pause feed
;; run command

;; description: integer->char > 255
;; command expect failure
;; feed
(integer->char 256)
;; pause feed
;; run command

;; description: integer->char too few args
;; command expect failure
;; feed
(integer->char)
;; pause feed
;; run command

;; description: integer->char too much args
;; command expect failure
;; feed
(integer->char 1 2)
;; pause feed
;; run command

;; description: integer->char not an integer
;; command expect failure
;; feed
(integer->char "no")
;; pause feed
;; run command

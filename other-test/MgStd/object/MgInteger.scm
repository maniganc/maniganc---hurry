
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
;; run command

;; description: integer? too much args
;; command expect failure
;; feed
(integer? 1 2)
;; run command

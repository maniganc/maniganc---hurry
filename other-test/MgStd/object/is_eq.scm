
;; command: ../scheme -
;; command timeout 1.0

;; description: eq?
;; command expect success
;; feed
(write (eq? #t #t))
(write (eq? #f #f))
(write (eq? #f #t))
(write (eq? write write))

(define var eq?)
(write (eq? var eq?))
;; check
#t
#t
#f
#t
#t
;; pause check
;; run command


;; description: eq? no args
;; command expect success
;; feed
(write (eq?))
;; check
#t
;; pause check
;; run command


;; description: eq? one arg
;; command expect success
;; feed
(write (eq? 1))
(write (eq? #f))
;; check
#t
#t
;; pause check
;; run command

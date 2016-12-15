
;; command: ../scheme -
;; command timeout 1.0

;; description: let
;; command expect success
;; feed
(define val "avalue")
(write (let ((val 12) (other 42))
  (write other)
  val))
(write val)
;; check
42
12
"avalue"
;; pause check
;; run command


;; description: let, invalid
;; command expect failure
;; feed
(let)
;; pause feed
;; run command

;; command expect failure
;; feed
(let (()) ())
;; pause feed
;; run command

;; command expect failure
;; feed
(let ((a)) ())
;; pause feed
;; run command

;; command expect failure
;; feed
(let ((a 1) 1) ())
;; pause feed
;; run command


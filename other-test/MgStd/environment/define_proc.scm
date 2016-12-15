
;; command: ../scheme -
;; command timeout 1.0

;; description: procedure with params
;; command expect success
;; feed
;; a procedure with one param
(define (say-hello-to name)
  (write "hello") (write name))
(say-hello-to "world")
;; check
"hello"
"world"
;; pause check
;; run command

;; description: procedure without params
;; command expect success
;; feed
;; a procedure with one param
(define (say-hello)
  (write "hello"))
(say-hello)
;; check
"hello"
;; pause check
;; run command


;; description: procedure return
;; command expect success
;; feed
;; a procedure with one param
(define (greeting-string)
  123
  "hello!")
(write (greeting-string))
;; check
"hello!"
;; pause check
;; run command

;; description: procedure nesting
;; command expect success
;; feed
;; a procedure with one param
(define (greeting-func)
  (define (greeting-eng) "hello")
  (define (greeting-fr) "bonjour")
  greeting-eng)
(write ((greeting-func)))
;; check
"hello"
;; pause check
;; run command

;; description: define procedure, not an identifier
;; command expect failure
;; feed
(define (42 x y) 2)
;; pause feed
;; run command

;; description: define procedure, not 
;; command expect failure
;; feed
(define (42 x y) 2)
;; pause feed
;; run command

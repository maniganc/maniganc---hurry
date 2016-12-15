
;; command: ../scheme -
;; command timeout 1.0

;; description: lambda
;; command expect success
;; feed
(lambda () ())
((lambda () (write "hello world")))
(write ((lambda () 1)))
(define f (lambda (x y z)
	    x
	    (write y)
	    z))
(write (f 1 2 3))
;; check
"hello world"
1
2
3
;; pause check
;; run command



;; description: lambda, indirect args
;; command expect success
;; feed
(define f (lambda (x) (write x)))
(define var "12")
(f var) (f '(wow))
;; check
"12"
(wow)
;; pause check
;; run command



;; description: lambda, invalid number of args
;; command expect failure
;; feed
(lambda)
(lambda ())
(lambda (x y))
;; pause feed
;; run command

;; description: lambda, invalid procedure argument
;; command expect failure
;; feed
(lambda (a 1) (write a))
;; pause feed
;; run command

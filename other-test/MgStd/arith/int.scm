
;; command: ../scheme -
;; command timeout 1.0

;; description: + op
;; command expect success
;; feed
(write (+))
(write (+ 1))
(write (+ -1 3))
(write (+ 1 2 3))
(define var 5)
(write (+ var var 1))
;; check
0
1
2
6
11
;; pause check
;; run command


;; description: - op
;; command expect success
;; feed
(write (- 42))
(write (- 1 2))
(define var 5)
(write (- var))
(write (- var 1))
;; check
-42
-1
-5
4
;; pause check
;; run command


;; description: incorrect - op
;; command expect failure
;; feed
(-)
;; pause feed
;; run command


;; description: * op
;; command expect success
;; feed
(write (*))
(write (* 2))
(write (* 2 -3))
(define var 5)
(write (* var))
(write (* var 2))
;; check
1
2
-6
5
10
;; pause check
;; run command


;; description: / op
;; command expect success
;; feed
(write (/ 42))
(write (/ 10 2))
(define var 7)
(write (/ var))
(write (/ var 2))
;; check
42
5
7
3
;; pause check
;; run command


;; description: incorrect / op
;; command expect failure
;; feed
(/)
;; pause feed
;; run command


;; description: = op
;; command expect success
;; feed
(write (=))
(write (= 42))
(write (= 10 2))
(define var 7)
(write (= var 1))
(write (= var 7))
;; check
#t
#t
#f
#f
#t
;; pause check
;; run command


;; description: < op
;; command expect success
;; feed
(write (<))
(write (< 42))
(write (< 10 2))
(define var 7)
(write (< var 1))
(write (< var 8))
;; check
#t
#t
#f
#f
#t
;; pause check
;; run command


;; description: > op
;; command expect success
;; feed
(write (>))
(write (> 42))
(write (> 10 20))
(define var 7)
(write (> var 1))
(write (> var 8))
;; check
#t
#t
#f
#t
#f
;; pause check
;; run command

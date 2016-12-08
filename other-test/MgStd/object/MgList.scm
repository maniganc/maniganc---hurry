
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
;; pause feed
;; run command

;; description: null? too much args
;; command expect failure
;; feed
(null? 1 2)
;; pause feed
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
;; pause feed
;; run command

;; description: list? too much args
;; command expect failure
;; feed
(list? '() '(1))
;; pause feed
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
;; pause feed
;; run command

;; description: pair? too much args
;; command expect failure
;; feed
(pair? '(1 . 2) '("a" . (1)))
;; pause feed
;; run command


;; description: list->string
;; command expect success
;; feed
(write (list->string '(#\a)))
(write (list->string '(#\a #\b #\c)))
;; check
"a"
"abc"
;; pause check
;; run command

;; description: list->string () case
;; command expect success
;; feed
(write (list->string '()))
;; check
""
;; pause check
;; run command

;; description: list->string no args
;; command expect failure
;; feed
(list->string)
;; pause feed
;; run command

;; description: list->string too much args
;; command expect failure
;; feed
(list->string '(#\a) '())
;; pause feed
;; run command

;; description: list->string improper list
;; command expect failure
;; feed
(list->string '(#\a . #\b))
;; pause feed
;; run command

;; description: list->string not a list
;; command expect failure
;; feed
(list->string 'symbol)
;; pause feed
;; run command



;; description: cons
;; command expect success
;; feed
(write (cons 1 2))
(write
 (cons
  (cons 2 '())
  '()))
(define va #\y)
(define vb #\z)
(write (cons va vb))
;; check
(1 . 2)
((2))
(#\y . #\z)
;; pause check
;; run command

;; description: cons not enough args
;; command expect failure
;; feed
(cons)
;; pause feed
;; run command

;; description: cons not enough args 2
;; feed
(cons 1)
;; pause feed
;; run command

;; description: cons too much args
;; feed
(cons 1 2 3)
;; pause feed
;; run command



;; description: car
;; command expect success
;; feed
(write (car '(1 2)))
(define lst '("a" 1 2))
(write (car lst))
;; check
1
"a"
;; pause check
;; run command

;; description: car not enough args
;; command expect failure
;; feed
(car)
;; pause feed
;; run command

;; description: car too much args
;; feed
(car '(1) '(4))
;; pause feed
;; run command

;; description: car empty list
;; feed
(car '())
;; pause feed
;; run command



;; description: cdr
;; command expect success
;; feed
(write (cdr '(1 2)))
(define lst '("a" 1 2))
(write (cdr lst))
;; check
(2)
(1 2)
;; pause check
;; run command

;; description: cdr not enough args
;; command expect failure
;; feed
(cdr)
;; pause feed
;; run command

;; description: cdr too much args
;; feed
(cdr '(1) '(4))
;; pause feed
;; run command

;; description: cdr empty list
;; feed
(cdr '())
;; pause feed
;; run command



;; description: set-car!
;; command expect success
;; feed
(define lst '(1 2 3))
(set-car! lst 4)
(write lst)

(define var 12)
(set-car! lst var)
(write lst)

(set! lst '(1))
(set-car! lst 3)
(write lst)

(write (set-car! lst 3))
;; check
(4 2 3)
(12 2 3)
(3)
<unspecified>
;; pause check
;; run command

;; description: set-car! not enough args
;; command expect failure
;; feed
(set-car!)
;; pause feed
;; run command

;; description: set-car! not enough args 2
;; command expect failure
;; feed
(set-car! '(1))
;; pause feed
;; run command

;; description: set-car! too much args
;; feed
(set-car! '(1) '(4) 2)
;; pause feed
;; run command

;; description: set-car! empty list
;; feed
(set-car! '() 3)
;; pause feed
;; run command


;; description: set-cdr!
;; command expect success
;; feed
(define lst '(1 2 3))
(set-cdr! lst 4)
(write lst)

(define var 12)
(set-cdr! lst var)
(write lst)

(set! lst '(1))
(set-cdr! lst 3)
(write lst)

(write (set-cdr! lst 3))
;; check
(1 . 4)
(1 . 12)
(1 . 3)
<unspecified>
;; pause check
;; run command

;; description: set-cdr! not enough args
;; command expect failure
;; feed
(set-cdr!)
;; pause feed
;; run command

;; description: set-cdr! not enough args 2
;; command expect failure
;; feed
(set-cdr! '(1))
;; pause feed
;; run command

;; description: set-cdr! too much args
;; feed
(set-cdr! '(1) '(4) 2)
;; pause feed
;; run command

;; description: set-cdr! empty list
;; feed
(set-cdr! '() 3)
;; pause feed
;; run command




;; description: list
;; command expect success
;; feed
(write (list))
(write (list 1))
(write (list
        1
        (list #t 1)
        3))
(define var 123)
(write (list var))
;; check
()
(1)
(1 (#t 1) 3)
(123)
;; pause check
;; run command

;; description: list eval failed
;; command expect failure
;; feed
(list unk)
;; pause feed
;; run command

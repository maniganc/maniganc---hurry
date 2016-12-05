
;; command: ../scheme -
;; command timeout 1.0

;; description: define test
;; command expect success
;; feed
;; define 2 vars
(define str "answer")
(define var 42)
(write str)
(write var)
;; redefine 
(define str "foo")
;; str changes, but not var
(write str)
(write var)
;; check
"answer"
42
"foo"
42
;; pause check
;; run command

;; description: redefine only test
;; command expect success
;; feed
(define str 1) (write str)
;; redefine 
(define str "foo") (write str)
;; str changes, but not var
;; check
1
"foo"
;; pause check
;; run command

;; description: define from another var test
;; command expect success
;; feed
(define str "wow") (write str)
(define str2 str)
(write str)
(write str2)
;; str changes, but not var
;; check
"wow"
"wow"
"wow"
;; pause check
;; run command

;; description: define something other than an identifier
;; command expect failure
;; feed
(define 1 1)
;; run command

;; description: define from unknown var test
;; command expect failure
;; feed
(define var unkvar)
;; run command

;; description: referencing an unknwown var test
;; command expect failure
;; feed
(write unkvar)
;; run command

;; description: define, too few args 
;; command expect failure
;; feed
(define)
;; run command
;; feed
(define a)
;; run command

;; description: define, too much args
;; command expect failure
;; feed
;;  wrong, too much args
(define a 1 1)
;; run command




;; description: set! something other than an identifier
;; command expect failure
;; feed
(set! 1 1)
;; run command

;; description: set! changing a var value test
;; command expect success
;; feed
(define str 1) (write str)
(set! str 2) (write str)
;; str changes, but not var
;; check
1
2
;; pause check
;; run command

;; description: set!, too few args
;; command expect failure
;; feed
(set!)
;; run command
;; feed
(set! a)
;; run command

;; description: set!, too much args
;; command expect failure
;; feed
;; wrong, too much args
(set! a 1 2)
;; run command



;; description: eval, trivial eval test
;; command expect success
;; feed
(eval '(write "p3x-492"))
;; check
"p3x-492"
;; pause check
;; run command


;; description: eval, indirect eval
;; command expect success
;; feed
(define expr '(write "hey"))
(eval expr)
;; check
"hey"
;; pause check
;; run command


;; description: eval, on current env (warning: rely on a debug function)
;; command expect success
;; feed
(define expr '(write "hey"))
(eval expr (_env))
;; check
"hey"
;; pause check
;; run command


;; description: eval, on custom env
;; command expect success
;; feed
(write (eval 'var '(() (var1 . 12) (varm . "tirlibibi") (var . "this") (e . 1) )))
;; check
"this"
;; pause check
;; run command



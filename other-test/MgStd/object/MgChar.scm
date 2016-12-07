
;; command: ../scheme -
;; command timeout 1.0

;; description: char?
;; command expect success
;; feed
(write (char? #t))
(write (char? #\a))
(write (char? 'a))
(write (char? 123))
;; check
#f
#t
#f
#f
;; pause check
;; run command

;; description: char? too few args
;; command expect failure
;; feed
(char?)
;; pause feed
;; run command

;; description: char? too much args
;; command expect failure
;; feed
(char? \#a \#b)
;; pause feed
;; run command




;; description: char->string
;; command expect success
;; feed
(write (char->string #\a))
(define var #\b)
(write (char->string var))
;; check
"a"
"b"
;; pause check
;; run command

;; description: char->string too few args
;; command expect failure
;; feed
(char->string)
;; pause feed
;; run command

;; description: char->string too much args
;; command expect failure
;; feed
(char->string #\a #\b)
;; pause feed
;; run command

;; description: char->string not a char
;; command expect failure
;; feed
(char->string "no")
;; pause feed
;; run command

;; description: char->string not a char 2
;; command expect failure
;; feed
(define var "noo")
(char->string var)
;; pause feed
;; run command




;; description: char->integer
;; command expect success
;; feed
(write (char->integer #\a))
(define var #\b)
(write (char->integer var))
;; check
97
98
;; pause check
;; run command

;; description: char->integer too few args
;; command expect failure
;; feed
(char->integer)
;; pause feed
;; run command

;; description: char->integer too much args
;; command expect failure
;; feed
(char->integer #\a #\b)
;; pause feed
;; run command

;; description: char->integer not a char
;; command expect failure
;; feed
(char->integer "no")
;; pause feed
;; run command

;; description: char->integer not a char 2
;; command expect failure
;; feed
(define var "noo")
(char->integer var)
;; pause feed
;; run command

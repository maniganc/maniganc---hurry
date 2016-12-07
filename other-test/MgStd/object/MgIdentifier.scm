
;; command: ../scheme -
;; command timeout 1.0

;; description: symbol?
;; command expect success
;; feed
(write (symbol? #t))
(write (symbol? #f))
(write (symbol? 'a))
(write (symbol? 123))
;; check
#f
#f
#t
#f
;; pause check
;; run command

;; description: symbol? too few args
;; command expect failure
;; feed
(symbol?)
;; pause feed
;; run command

;; description: symbol? too much args
;; command expect failure
;; feed
(symbol? 1 2)
;; pause feed
;; run command




;; description: symbol->string
;; command expect success
;; feed
(write (symbol->string 'hello))
(define var 'woo)
(write (symbol->string var))
;; check
"hello"
"woo"
;; pause check
;; run command

;; description: symbol->string too few args
;; command expect failure
;; feed
(symbol->string)
;; pause feed
;; run command

;; description: symbol->string too much args
;; command expect failure
;; feed
(symbol->string 'one 'two)
;; pause feed
;; run command

;; description: symbol->string not a symbol
;; command expect failure
;; feed
(symbol->string "no")
;; pause feed
;; run command

;; description: symbol->string not a symbol 2
;; command expect failure
;; feed
(define var "noo")
(symbol->string var)
;; pause feed
;; run command

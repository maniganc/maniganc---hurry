
;; command: ../scheme -
;; command timeout 1.0

;; description: string?
;; command expect success
;; feed
(write (string? #t))
(write (string? #\a))
(write (string? "woo"))
(write (string? 'a))
;; check
#f
#f
#t
#f
;; pause check
;; run command

;; description: string? too few args
;; command expect failure
;; feed
(string?)
;; run command

;; description: string? too much args
;; command expect failure
;; feed
(string? "one" "2")
;; run command


;; description: string->symbol
;; command expect success
;; feed
(write (string->symbol "foo"))
;; check
foo
;; pause check
;; run command

;; description: string->symbol no args
;; command expect failure
;; feed
(string->symbol)
;; run command

;; description: string->symbol too much args
;; command expect failure
;; feed
(string->symbol "a" "dfk")
;; run command

;; description: string->symbol not a string
;; command expect failure
;; feed
(string->symbol 142)
;; run command



;; description: string->list
;; command expect success
;; feed
(write (string->list "foo"))
;; check
(#\f #\o #\o)
;; pause check
;; run command

;; description: string->list no args
;; command expect failure
;; feed
(string->list)
;; run command

;; description: string->list too much args
;; command expect failure
;; feed
(string->list "a" "dfk")
;; run command

;; description: string->list not a string
;; command expect failure
;; feed
(string->list 24)
;; run command

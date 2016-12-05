
;; command: ../scheme -
;; command timeout 1.0

;; description: quote test
;; command expect success
;; feed
(write (quote 1))
(write '1)
(write '())
(write '(1))
(write '"hello")
(write '123)
(write 'sym)
;; check
1
1
()
(1)
"hello"
123
sym
;; pause check
;; run command

;; description: several quotes test
;; command expect success
;; feed
(write '''1)
;; check
(quote (quote 1))
;; pause check
;; run command

;; description: quote, wrong syntax test
;; command expect failure
;; feed
(quote)
;; run command

;; feed
(quote 1 2)
;; run command

;; feed
'
;; run command

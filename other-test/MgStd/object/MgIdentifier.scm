
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
;; run command

;; description: symbol? too much args
;; command expect failure
;; feed
(symbol? 1 2)
;; run command

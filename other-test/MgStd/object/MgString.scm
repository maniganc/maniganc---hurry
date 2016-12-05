
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

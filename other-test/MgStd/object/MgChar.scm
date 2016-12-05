
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
;; run command

;; description: char? too much args
;; command expect failure
;; feed
(char? \#a \#b)
;; run command

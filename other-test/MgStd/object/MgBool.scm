
;; command: ../scheme -
;; command timeout 1.0

;; description: bool?
;; command expect success
;; feed
(write (bool? #t))
(write (bool? #f))
(write (bool? 'a))
(write (bool? 123))
;; check
#t
#t
#f
#f
;; pause check
;; run command

;; description: bool? too few args
;; command expect failure
;; feed
(bool?)
;; run command

;; description: bool? too much args
;; command expect failure
;; feed
(bool? 1 2)
;; run command

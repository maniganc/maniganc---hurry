
;; command: ../scheme -
;; command timeout 1.0

;; description: if
;; command expect success
;; feed
(write (if 1 2 3))
(write (if #f 2 3))
(write (if #t #f 4))
(write (if () 5 4))
;; check
2
3
#f
5
;; pause check
;; run command

;; description: if, wrong syntax
;; command expect failure
;; feed
(if)
;; run command

;; feed
(if 1)
;; run command

;; feed
(if 2)
;; run command


;; description: or
;; command expect success
;; feed
(write (or))
(write (or 2))
(write (or "bar" 2 3 "foo"))
(write (or #f 3))
(write (or #f #f))
;; check
#f
2
"bar"
3
#f
;; pause check
;; run command

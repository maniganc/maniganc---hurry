
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

;; description: if short form
;; command expect success
;; feed
(write (if 1 1))
(write (if #f 2))
(write (if #t 3))
;; check
1
<unspecified>
3
;; pause check
;; run command

;; description: if, wrong syntax
;; command expect failure
;; feed
(if)
;; pause feed
;; run command

;; feed
(if 1)
;; pause feed
;; run command

;; feed
(if 2 3 4 5)
;; pause feed
;; run command


;; description: and
;; command expect success
;; feed
(write (and))
(write (and 2))
(write (and "bar" 2 3 "foo"))
(write (and #f 3))
(write (and #f #f))
;; check
#t
2
"foo"
#f
#f
;; pause check
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

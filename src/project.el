
(interactive)

(setq project:dir (file-name-directory load-file-name))

;; include directories
(setq project:include-path
      (list
       (string-join (list project:dir "MgCore/include"))
       (string-join (list project:dir "lib/debug/include"))
       (string-join (list project:dir "lib/std/include"))
       (string-join (list project:dir "MgStd"))
       ))

;; definitions
(setq project:definition
      (list
       "DEBUG"))

(setq project:cflags-include
      (mapcar (lambda (x) (string-join (list "-I" x))) project:include-path))

(setq project:cflags-definition
      (mapcar (lambda (x) (string-join (list "-D" x))) project:definition))

(message "--project dir:%s" project:dir)
(message "--project include dir:%s" project:include-path)
(message "--project defs:%s" project:definition)

(defun project:semantic-hook ()
  (when (string-match project:dir (buffer-file-name))
    (mapcar (lambda (x) (semantic-add-system-include x 'c-mode))
            project:include-path)
    (message "--project: semantic include paths added")))
(add-hook 'semantic-init-hook 'project:semantic-hook)

(defun project:c-mode-hook ()
  (when (string-match project:dir (buffer-file-name))

    (semantic-mode +1)

    (setq irony--compile-options
          (append project:cflags-definition project:cflags-include))

    (setq flycheck-clang-include-path project:include-path)
    (setq flycheck-clang-definitions project:definition)

    (message "--project: project file detected, irony & flycheck added")
    (c-guess)))
(add-hook 'c-mode-common-hook 'project:c-mode-hook)

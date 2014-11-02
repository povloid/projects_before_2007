(defconst author "pacman")

;; Scroll Bar gets dragged by mouse butn 1
(global-set-key [vertical-scroll-bar down-mouse-1] 'scroll-bar-drag)

;; Paste at point NOT at cursor
(setq mouse-yank-at-point 't)

;;колесо мышки
(mouse-wheel-mode 1)

;; 
(setq scroll-margin 4)

;;Настройка внешнего вида редактора
;;установка размеров экрана
(set-frame-height (selected-frame) 47)
(set-frame-width (selected-frame) 80)
;;установка левого верхнего угла фрейма 
(set-frame-position (selected-frame) 200 0)

;; настройка клавиатуры как в Windows
;;Delete (and its variants) delete forward instead of backward.
;;C-Backspace kills backward a word (as C-Delete normally would).
;;M-Backspace does undo.
;;Home and End move to beginning and end of line
;;C-Home and C-End move to beginning and end of buffer.
;;C-Escape does list-buffers." 
(pc-bindings-mode)


;; Выделение парных скобок
(show-paren-mode 1)
 
;;Настройки проверка правописания Ispell
;(setq ispell-dictionary "russianw"); default dictionary
(setq ispell-local-dictionary "russian"); default dictionary
(add-hook 'text-mode-hook 'flyspell-mode)
(setq flyspell-default-dictionary "russian")
(setq flyspell-delay '1)

;; show column & line numbers in status bar
(setq column-number-mode t)
(setq line-number-mode t)

;;Следущее, что мне не нужно, это стартовое сообщение, его отключение:
;; Prevent the startup message
(setq inhibit-startup-message t)

;;compilation window shall scroll down
(setq compilation-scroll-output 1)

;; ido
(require 'ido)
(ido-mode t)

;;; setnu.el
;;  ~/Emacs/Extensions/setnu.el
;;  M-x setnu-mode
(require 'setnu)
;(add-hook 'text-mode-hook 'turn-on-setnu-mode)

;;; ibuffer.el
;;  ~/Emacs/Extensions/ibuffer.el
;;  C-x C-b
(require 'ibuffer)

;; Сортируем буфер
;;


(defun ibufferShow()
  ;; ido kill emacs hook      
  "Open file list - >"
  (interactive)
  (ibuffer) 
  (ibuffer-do-sort-by-alphabetic))

;;(defun nexttab()
  ;; ido kill emacs hook      
;;  "Next TAB - >"
;;  (interactive) 
;;  (tabbar-forward ) 
;;  (tabbar-scroll-left-button-callback nil))

;;(global-set-key [f12] 'nexttab)

;;(global-set-key [f11] '((tabbar-backward)
;;			(tabbar-scroll-left-button-callback)))

;; Привязка файлов к режимам

;; setting some f[1-12] keys
(global-set-key [f1]    'help)
(global-set-key [f2]    'save-buffer)
(global-set-key [f3]    'flyspell-mode)
(global-set-key [f4]    'ispell-buffer)

(global-set-key [M-f12]    'buffer-menu)
(global-set-key [f12]    'ibufferShow)

(global-set-key [f9]    'compile)
(global-set-key [M-f11]   'speedbar)
(global-set-key [f10]    'save-buffers-kill-emacs)
(global-set-key [f7]    'isearch-forward)


(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(auto-fill-inhibit-regexp "on")
 '(blink-cursor-delay 2.0)
 '(blink-cursor-interval 0.2)
 '(blink-cursor-mode nil)
 '(case-fold-search t)
 '(cursor-in-non-selected-windows t)
 '(default-input-method "cyrillic-jcuken")
 '(display-hourglass t)
 '(global-font-lock-mode nil nil (font-lock))
 '(global-hl-line-mode t)
 '(ibuffer-default-sorting-mode (quote recency))
 '(imenu-sort-function (quote imenu--sort-by-name))
 '(indicate-empty-lines nil)
 '(ispell-local-dictionary "russian" t)
 '(scroll-conservatively 50)
 '(semanticdb-project-roots nil)
 '(setq scroll-preserve-screen-position t)
 '(speedbar-activity-change-focus-flag t)
 '(speedbar-indentation-width 1)
 '(speedbar-use-images t)
 '(tabbar-cycling-scope (quote groups))
 '(tool-bar-mode nil nil (tool-bar))
 '(track-eol nil)
 '(transient-mark-mode "on")
 '(truncate-lines "no")
 '(view-scroll-auto-exit nil)
 '(x-stretch-cursor nil))
 '(font-lock-comment-face  "MistyRose1")

(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:stipple nil :background "PeachPuff1" :foreground "black" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil))))
 '(cursor ((t (:background "black" :family unspecified))))
 '(ecb-default-general-face ((((class color) (background light)) (:height 0.7))))
 '(eshell-ls-readonly-face ((((class color) (background dark)) (:foreground "red"))))
 '(eshell-prompt-face ((t (:foreground "gray70" :weight bold))))
 '(font-lock-builtin-face ((t (:foreground "red"))))
 '(font-lock-comment-face ((t (:foreground "tomato3" :weight normal))))
 '(font-lock-constant-face ((((class color) (background light)) (:foreground "dark red"))))
 '(font-lock-function-name-face ((t (:foreground "purple" :weight bold))))
 '(font-lock-keyword-face ((t (:foreground "dark blue" :weight bold))))
 '(font-lock-string-face ((t (:foreground "dark green"))))
 '(font-lock-type-face ((t (:foreground "brown4"))))
 '(font-lock-variable-name-face ((((class color) (background light)) (:foreground "magenta3"))))
 '(font-lock-warning-face ((t (:background "Yellow" :foreground "Blue4" :weight bold))))
 '(fringe ((t (:background "dark blue" :foreground "cyan"))))
 '(highlight ((((class color) (background light)) (:background "PeachPuff"))))
 '(hl-line ((t (:inherit highlight :background "bisque"))))
 '(lm-font-lock-face-X ((((class color)) (:foreground "green" :family "screen-fixed"))))
 '(menu ((t (:background "gray80" :box (:line-width 1 :color "grey75" :style released-button) :weight normal :height 1.1))))
 '(mode-line ((t (:background "grey80" :foreground "black" :box (:line-width -1 :style released-button)))))
 '(mouse ((t (:background "black"))))
 '(region ((t (:background "cyan"))))
 '(scroll-bar ((t (:background "gray60"))))
 '(speedbar-button-face ((nil nil)))
 '(speedbar-directory-face ((nil (:weight bold :height 80 :family "helvetica"))))
 '(speedbar-file-face ((nil (:weight normal :height 80 :family "helvetica"))))
 '(speedbar-highlight-face ((((class color) (background light)) (:background "black" :foreground "white"))))
 '(speedbar-selected-face ((nil (:background "blue" :foreground "white"))))
 '(tabbar-default-face ((t (:inherit variable-pitch :background "gray72" :foreground "black" :weight normal :height 0.7 :family "helvetica"))))
 '(tabbar-selected-face ((t (:inherit tabbar-default-face :background "white" :foreground "black" :box (:line-width 2 :color "blue" :style released-button) :weight bold))))
 '(tabbar-unselected-face ((t (:inherit tabbar-default-face :box (:line-width 1 :color "black" :style pressed-button))))))

(put 'upcase-region 'disabled nil)

;; Привязки файлов к режимам
(setq auto-mode-alist (cons '("\\.h" . c++-mode) auto-mode-alist))
(setq auto-mode-alist (cons '("\\.tcpp" . c++-mode) auto-mode-alist))
(setq auto-mode-alist (cons '("\\.jl"  . sawfish-mode) auto-mode-alist))
(setq auto-mode-alist (cons '("\\.ml?" . caml-mode) auto-mode-alist))

(global-font-lock-mode t)
(toggle-truncate-lines t)
;;(toggle-iswitchb-shit-buffer-display)
(pc-selection-mode)

;; Функция для compile проверяющий успешно ли закончилась компиляция. 
;; Если успешно - то закрывает окно и выводит надпись "No problemo" в modeline 
;; В противном случае выводит строку "There are errors!" в modeline 

(defun myfn-compilation-finished (compilation-buffer result-string)
  "Handle compilation finished!"
  (interactive)
  (if (string-match "finished" result-string) (and (message "No problemo!") (delete-window (get-buffer-window "*compilation*"))) (message "There are errors in your program!")))


(setq compilation-finish-function (function myfn-compilation-finished))


;; функции для  проекта "XXXXXX"


(defun create-constructor-cpp-file (file-name desc-string)
  "Make .cpp header"
  (interactive "FFile name: \nsDescription: ")
  (find-file file-name)

  (let* ((file-name-string (file-relative-name file-name default-directory))
 (date-string (current-time-string))
 (h-file-name (replace-string file-name-string "\\.cpp" ".h")))
    (insert (concat 
     
     "/////////////////////////////////////////////////////////////"
     "\n// Имя : " file-name-string
     "\n// Авторы : " author 
     "\n// Дата создания : "date-string
     "\n// CVS-ID: $Id:  $"
     "\n// " desc-string
     "\n//////////////////////////////////////////////////////////////"
     "\n"
     "\n#include \"" h-file-name "\""
     "\n"
     "\nBEGIN_PARSER_NAMESPACE"
     "\n\n"
     "\nEND_PARSER_NAMESPACE\n"
     "\n\n"))))

(defun create-constructor-h-file (file-name desc-string)
  "Make .h  header"
  (interactive "FFile name: \nsDescription: ")

  (find-file file-name)

  (let* ((file-name-string (file-relative-name file-name default-directory))
 (date-string (current-time-string))
 (file-name-upcasing (replace-string (upcase file-name-string) "\\." "_")))

    (insert (concat
     "/////////////////////////////////////////////////////////////"
     "\n// Имя : " file-name-string
     "\n// Авторы : " author
     "\n// Дата создания : " date-string
     "\n// CVS-ID: $Id:  $"
     "\n// " desc-string 
     "\n//////////////////////////////////////////////////////////////"
             "\n"
     "\n#ifndef " file-name-upcasing
     "\n#define " file-name-upcasing
     "\n"
     "\nBEGIN_PARSER_NAMESPACE"
     "\n\n"
     "\nEND_PARSER_NAMESPACE"
     "\n\n"
     "\n#endif"
     "\n"))))


;; Нашлепки для работы с Си++
;; Примочки для Си++ 

(add-hook 'c++-mode-hook 
  (function (lambda () 
      (define-key c++-mode-map [(meta kp-0)] 'cpp-hpp-switch))))

(define-skeleton c++-for-stmt 
  "Abbrev for for statement"
  nil
  > "for( "_";"_";"_" )"
  \n > "{" _ "}"
)

(define-skeleton c++-while-stmt 
  "Abbrev for while statement"
  nil
  > "while( "_" )"
  \n > "{" _ "}"
)

(define-skeleton c++-switch-stmt 
  "Abbrev for switch statement"
  nil
  > "switch( "_" )"
  \n > "{"
  \n > "case " _ ":" _ "break;"
  \n > "case " _ ":" _ "break;"
  \n > "default :" _ "break;"
  \n > "}"
)

(define-skeleton c++-if-stmt
  "Abbrev for if statement"
  nil
  > "if ( "_" )"
  \n > "{" _ "}"
)

(define-skeleton c++-ifelse-stmt
  "Abbrev for if ... else  statement"
  nil
  > "if ( "_" )"
  \n > "{" _ "}"
  \n > "else"
  \n > "{" _ "}"
)

(define-skeleton c++-class-stmt
  "Abbrev for class  statement"
  nil
  > "class " _
  \n > "{" _ "};"
)

(define-skeleton c++-struct-stmt
  "Abbrev for class  statement"
  nil
  > "struct " _
  \n > "{" _ "};"
)

(define-skeleton c++-uint 
  "Abbrev for unsigned int"
  nil
  "unsigned int"
)

(defmacro c++-add-keyword (keywords-table keyword-regexp keyword-param)
  `(setq ,keywords-table 
 (append ,keywords-table
 (list (list (c-make-font-lock-search-function ,keyword-regexp ',keyword-param))))))


(add-hook 'c++-mode-hook 
  (function (lambda () 
      (progn 

(define-abbrev c++-mode-abbrev-table "forr"    ""  `c++-for-stmt)
(define-abbrev c++-mode-abbrev-table "whilee"  ""  `c++-while-stmt)
(define-abbrev c++-mode-abbrev-table "switchh" ""  `c++-struct-stmt)
(define-abbrev c++-mode-abbrev-table "iff"     ""  `c++-if-stmt)    
(define-abbrev c++-mode-abbrev-table "ifelse"  ""  `c++-ifelse-stmt)
(define-abbrev c++-mode-abbrev-table "classs"  ""  `c++-class-stmt)
(define-abbrev c++-mode-abbrev-table "uuint"   "unsigned int")

(c++-add-keyword c++-font-lock-keywords   "bind_v" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-1 "bind_v" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-2 "bind_v" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-3 "bind_v" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords   "bind_r" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-1 "bind_r" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-2 "bind_r" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-3 "bind_r" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords   "requires" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-1 "requires" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-2 "requires" (0 font-lock-keyword-face t))
(c++-add-keyword c++-font-lock-keywords-3 "requires" (0 font-lock-keyword-face t))

(define-key c++-mode-map [(control c) (control c)] 'compile)
(define-key c++-mode-map [(control c) (control d)] 'gdb)
(define-key c++-mode-map [(meta kp-0)] 'cpp-hpp-switch)

))))

;;; Save session and desktop
;;  ~/Emacs/Extensions/session.el
;   /usr/share/emacs/21.4/lisp/desktop.el(c)
(add-to-list 'load-path "~/.emacs.d")
(require 'session)
(add-hook 'after-init-hook 'session-initialize)
(load "desktop")
(desktop-load-default)
(desktop-read)

;; Mule cyrillic config
(prefer-coding-system 'utf-8)
(set-language-environment 'utf-8)

;; Cedet
;;(setq semantic-load-turn-useful-things-on t)
;;(load-file "~/myemacs/cedet-1.0pre3/common/cedet.el")
;;(global-set-key [?\C- ] 'semantic-ia-complete-symbol)

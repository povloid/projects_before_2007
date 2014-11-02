  .alpha
  .MODEL tiny
  .286

   assume cs:_code
  _code   segment para public "CODE"


  org 100h

  proc obmlom


                jmp beg_ini

  ;------------ Переменные заполняемые программой main ----------------
  mode          db 0            ;Выбор режима(печать-1 или обмен-0)
  n_zaprosa     db 0
  f_sost        db 0            ;Код состояния Ломиконта
  null_0        db ?
  adr_in        dd ?            ;Адрес буфера приема

  ;--------------------------------------------------------------------

  ;-------------Внутрение переменные-----------------------------------

  Buf_out             DB 01h,01h,0c0h,0c2h,05h,8ah         ;запрос состояния(0)
  coun_out            EQU $ - Buf_out
  str_out             DB 9 dup(0)                          ;  передаваемое в Ломиконт
                                                           ;  сообщение(1)
  coun_out_s          EQU $ - str_out
  Buf_out_s           DB 1h,4h,0c4h,0c9h,6h,09h,2h,7h,0abh ;запрос состояния(1)
  Buf_out_p           DB 1h,4h,0c8h,0cdh,6h,08h,0h,7h,0b0h ;запрос состояния(2)
 
  out_com1_sth        DB "sth"                             ;передача CH
  out_com1_stl        DB "stl"                             ;передача CL  
  out_com1_pow        DB "pow"                             ;передача мощности  
     
                       add_com       DW ?                  ; адрес COM порта
                       add_com1      DW ?                  ; адрес COM1 порта
                       len_pak       DW ?                  ; длина пакета 
                       summa         DB ?                  ;контрольная сумма 
        noy1          DB " Сбой связи",13,10,"$"
        noy2          DB " Предыдущий символ не считан",13,10,"$"
        noy3          DB " Ошибка паритета",13,10,"$"
        noy4          DB " Ошибка при приеме заголовка",13,10,"$"
        noy5          DB " Сбой при приеме кадра  ЭВМ",13,10,"$"
        noy6          DB " ПП в Ломиконте не хватило памяти",13,10,"$"
        noy7          DB " Ошибка по контрольной сумме заголовка",13,10,"$"
        noy8          DB " Ошибка по контрольной сумме кадра",13,10,"$"

   ;************************************************************
   ;   точка вызова драйвера

     punkt_call:


               mov bp,sp
               push es
               push ds
               push cs
               push cs
               pop ds
               pop es
               sti
               cld

    ;----------------------------------------------------------------------
    ;                сохранить параметры
    ;----------------------------------------------------------------------
               mov cs:mode,ah
               mov cs:n_zaprosa,al
               mov word ptr cs:adr_in,00h
               mov word ptr cs:adr_in+2,dx
               mov word ptr cs:len_pak,cx
               mov cs:summa,0
    ;----------------------------------------------------------------------
               or ah,ah
               jz _obmen
               jmp _print

   _obmen:
               or al,al
               jnz _cont0
               jmp _sost
   _cont0:            
               cmp cs:n_zaprosa,1
               jz _schet
               cmp cs:n_zaprosa,2
               jz _pow
               cmp cs:n_zaprosa,3
               jz _scthigh
               cmp cs:n_zaprosa,4
               jz _sctlow 
   ;--------------------------------------------------
   ;передача мощностей
                mov cx,2
                mov di,offset cs:str_out
                mov si,offset cs:out_com1_pow
                rep movsb
                mov cx,cs:len_pak
                mov word ptr cs:str_out+di,cx
                MOV CX,4

               jmp short  output_beg


  ;----------------------------------------------------
  ;передача старшего разряда счетчика
     _scthigh:  
                mov cx,2
                mov di,offset cs:str_out
                mov si,offset cs:out_com1_sth
                rep movsb
                mov cx,cs:len_pak               
                mov word ptr cs:str_out+di,cx
                MOV CX,4

               jmp short  output_beg
 
  ;------------------------------------------------------
  ;передача младшего разряда счетчика
    _sctlow:
                mov cx,2
                mov di,offset cs:str_out
                mov si,offset cs:out_com1_stl 
                rep movsb
                mov cx,cs:len_pak 
                mov word ptr cs:str_out+di,cx
                MOV CX,4

               jmp short  output_beg
     
  ;*********************************************************************
  ;  запрос мощности
   _pow:
                mov cx,coun_out_s
                mov di,offset cs:str_out
                mov si,offset cs:buf_out_p
                rep movsb
                MOV CX,coun_out_s

               jmp short  output_beg

   ;*********************************************************************
   _schet:
               mov cx,coun_out_s
               mov di,offset cs:str_out
               mov si,offset cs:buf_out_s
               rep movsb
               MOV CX,coun_out_s
               jmp short output_beg
   ;*********************************************************************

   ; начало  передачи сообщения 0-запрос состояния
    _sost:
            MOV CX,coun_out           ;заносим СХ количество символов (байтов) вывода
            mov di,offset cs:str_out
            mov si,offset cs:buf_out
            rep movsb
            MOV CX,coun_out
   ;***********************************************************************
    output_beg:
            XOR BX,BX                      ;и bх указывает начало
            mov ah,cs:mode
            or  ah,cs:mode
            jz add_com_m  
            MOV DX,cs:add_com1            ;базовый адрес
            jmp short chek_read           
  add_com_m:    
            MOV DX,cs:add_com            ;базовый адрес 
  chek_read:
            in al,dx
  beg_loop:

            MOV AH,cs:str_out+BX         ;вывод 1-символа
            CALL out_beg
            JAE cont
            JMP out_for_error

   cont:
          MOV AH,cs:str_out+BX         ;вывод 1-символа
          add cs:summa,ah
          adc cs:summa,0  
            INC BX
            LOOP beg_loop
   ; конец  цикла передачи сообщения

   ;*************************************************************
   ;начало цикла приема сообщения
            CALL test_in_zag
            JNC nach_in
            mov cs:f_sost,ah
            jmp out_for_error
   nach_in:
            rol bl,1                     ;удваиваем КС заголовка
            cmp n_zaprosa,0
            jz zap_sost
            mov si,word ptr cs:adr_in
            mov ax,word ptr cs:adr_in+2
            mov ds,ax
  zap_sost:
            mov di,cx
            jz zap_sost_1
            sub di,4
            jmp short beg_in_loop
  zap_sost_1:
            dec di
  beg_in_loop:
             CALL in_beg
             JAE contin
             mov cs:f_sost,ah
             JMP out_for_error


   contin:

             add bl,al
             adc bl,0


              cmp cx,di
              ja reper

             cmp n_zaprosa,0
             jz reper

              MOV ds:[si],AL          ;ввод 1-символа
  ;----------------------------------------------------
             test cx,0001h
             jz end_loop_beg
             mov ax,word ptr ds:[si-1]
             shr ax,1
             mov word ptr ds:[si-1],ax
  ;----------------------------------------------------


   end_loop_beg:

            INC si
  reper:



            LOOP beg_in_loop
            cmp n_zaprosa,0
            jnz in_chek_sum
            mov cs:f_sost,al


     in_chek_sum:
             call in_beg
             JAE chek_sum
             mov cs:f_sost,ah
             JMP out_for_error

    chek_sum:
             cmp bl,al
             jnz chek_no
             jmp out_for_norm

    chek_no:
                mov ah,40h
                mov cs:f_sost,ah
                jmp short out_for_error
   out_for_norm:
                pop ds
                pop es
                mov cx,[bp+4]
                and cx,0fffeh
                mov [bp+4],cx
                
                
                
                iret
  out_for_error:

               pop ds
               pop es
               mov cx,[bp+4]
               or cx,0001h
               mov [bp+4],cx
               iret

   ;конец цикла приема сообщения
   ;*************************************************************
    _print:
            jmp  out_for_norm

   ;*************************************************************



  out_beg:
            PUSH SI
            PUSH DI
            PUSH ES
            PUSH AX
            XOR AX,AX
            MOV ES,AX
            POP AX
            MOV SI,046ch                ;адрес данных таймера
   time_aut:
            CLI                         ;запретить прерывания
            MOV DI,ES:[SI]              ;di указывает на текущее значения таймера
            STI                         ;разрешить прерывания
            ADD DI,36
            JB time_aut                 ;повторять пока меньше 2 сек.

            ADD DX,5                    ;указываем на регистр статуса линии
   retry:
            IN AL,DX                    ;получаем байт статуса
            TEST AL,00100000b           ;проверяем готовность к передаче
            JNZ _out_char
            CLI
            CMP DI,ES:[SI]      ;сравниваем время
            STI
            JAE retry
            SUB dx,5
            STC
            JMP err_out

  _out_char:
            XCHG AL,AH                   ;обмен значении
            SUB DX,5
            OUT DX,AL
            CLC
 err_out:
            POP ES
            POP DI
            POP SI
            RET


  in_beg:
            PUSH SI
            PUSH DI
            PUSH ES
            PUSH AX
            XOR AX,AX
            MOV ES,AX
            POP AX
            MOV SI,046ch                ;адрес данных таймера
   time_in_aut:
            CLI                         ;запретить прерывания
            MOV DI,ES:[SI]              ;di указывает на текущее значения таймера
            STI                         ;разрешить прерывания
            ADD DI,36
            JB time_in_aut              ;повторять пока меньше 2 сек.

            ADD DX,5                    ;указываем на регистр статуса линии
   retry_in:
            IN AL,DX                    ;получаем байт статуса
            TEST AL,00000001b           ;проверяем готовность к приему
            JNZ  rr1_in
            CLI
            CMP DI,ES:[SI]      ;сравниваем время
            STI
            JAE retry_in
            SUB DX,5
            STC
            MOV AH,0                    ;код ощибки по времени
            JMP err_in
 rr1_in:
            TEST AL,00000010b
            JZ rr2_in
            SUB DX,5
            STC
            MOV AH,1                    ;перезаписан символ
            JMP err_in

 rr2_in:   TEST AL,00000100b
            JZ _in_char
            SUB DX,5
            STC
            MOV AH,2                    ;ошибка паритета
            JMP err_in

  _in_char:
            SUB DX,5
            IN AL,DX
            CLC
 err_in:
            POP ES
            POP DI
            POP SI
            RET

 ;*******************************************************************
 ;**** процедура обработки ответа загаловка ****
 ;********************************************************************
  test_in_zag:

  XOR CX,CX
  XOR BX,BX
  CALL in_beg
  JAE test01                         ;проверка приема 1-символа
  JMP end_in_err

 test01:

  CMP AL,01h
  JZ in_2
  MOV AH,04h                         ;проверка начало загаловка
  JMP end_in_err

 in_2:
  MOV bl,AL                          ;сложили и сохранили 1-символ
  CALL in_beg
  JAE test02
  JMP end_in_err

 test02:

 ADD bl,AL
 ADC bl,0
 mov cl,al
 CALL in_beg
 JAE test03
 JMP end_in_err

 test03:

   TEST AL,01000000b
 JZ goy1
 MOV AH,08h                ;принять со сбоем
 JMP end_in_err

 goy1:
  TEST AL,00100000b

 JZ goy2

 MOV AH,10h                ;нет памяти
 JMP end_in_err

 goy2:

 TEST AL,00000001b
 JZ in_4
 INC CH                    ;ПД пакета загаловка

 in_4:

 ADD BL,AL
 ADC BL,0
 CALL in_beg
 JAE test04
 JMP end_in_err

 test04:

     cmp al,bl
    jz end_in
    MOV AH,20H                   ;Ошибка по контрольной сумме загаловка

 end_in_err:

            stc
            ret

            end_in:

            CLC
            ret




  beg_ini:

  ;***********************************************************************
  ;                 проверка  и настройка COM2
  ;***********************************************************************

                         XOR AX,AX
                         MOV ES,AX       ;ES указывает на область данных BIOS

                         cmp AX,ES:[0402h]
                         JNZ  Norma

                         MOV DX,OFFSET cs:Mes
                         MOV AH,9H
                         INT 21H
                         JMP short end_prog

        Norma:
                         mov ax,es:[0402h]
                         MOV cs:add_com,AX               ;cохраняем СОМ2
                         MOV DX,OFFSET cs:Messade        ;dx указывает на строку сообщения
                         MOV AH,9H                       ;вывод строки
                         INT 21H

    ; Инициализация порта
    ;********************************************************************
                         MOV DX,1                       ;выбираем СОМ2
                         XOR AH,AH                      ;функция 0
                         MOV AL,0DFH                    ;байт инициализирующих данных
                         INT 14H                        ;инициализация порта

 ;**********************************************************************
  ;***********************************************************************
  ;                 проверка  и настройка COM1
  ;***********************************************************************

                         XOR AX,AX
                         MOV ES,AX       ;ES указывает на область данных BIOS

                         cmp AX,ES:[0400h]
                         JNZ  Normay

                         MOV DX,OFFSET cs:Mes
                         MOV AH,9H
                         INT 21H
                         JMP short end_prog

        Normay:
                         mov ax,es:[0400h]
                         MOV cs:add_com1,AX               ;cохраняем СОМ1
                         MOV DX,OFFSET cs:Messade        ;dx указывает на строку сообщения
                         MOV AH,9H                       ;вывод строки
                         INT 21H

    ; Инициализация порта
    ;********************************************************************
                         MOV DX,0                       ;выбираем СОМ1
                         XOR AH,AH                      ;функция 0
                         MOV AL,0DFH                    ;байт инициализирующих данных
                         INT 14H                        ;инициализация порта

 ;**********************************************************************

 ;    инициализация адреса общих переменных
 ;**********************************************************************
             xor ax,ax
             mov es,ax
             mov bx,04f0h
             mov cx,offset cs:mode
             push cs
             pop ax
             mov es:[bx],cx
             mov es:[bx+2],ax
  ;*********************************************************************
  ;          Установка вектора прерывания int 62h
  ;*********************************************************************
             mov dx,offset cs:punkt_call
             push cs
             pop ds
             mov ax,2562h
             int 21h
   ;********************************************************************
                       mov dx,offset cs:beg_ini
                       shr dx,4
                       inc dx
                       mov ax,3100h
                       int 21h

   ;***********************************************************************
    end_prog:
                         mov ax,4c01h
                         int 21h
  ;************************************************************************

  Messade   DB "              ╔══════════════════════════════╗  ",13,10
            db "              ║   Драйвер печати и обмена    ║  ",13,10
            db "              ║   c контроллером ЛОМИКОНТ    ║  ",13,10
            db "              ║          загружен.           ║  ",13,10
            db "              ╟──────────────────────────────╢  ",13,10
            db "              ║  Жамбыл  НОДФОС  10/11/96г.  ║  ",13,10
            db "              ╚══════════════════════════════╝  ",13,10,"$"

  Mes       DB   13,10,"               COM2 не инициализирован",13,10
            DB         "                аварийный  завершение",13,10,"$"


  ENDP obmlom

  ENDS

  END obmlom


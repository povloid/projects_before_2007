  .alpha
  .MODEL tiny
  .286

   assume cs:_code
  _code   segment para public "CODE"


  org 100h

  proc obmlom


                jmp beg_ini
                
  ;------------ ���������� ����������� ���������� main ----------------
  mode          db 0            ;����� ������(������-1 ��� �����-0)
  n_zaprosa     db 0
  f_sost        db 0            ;��� ��������� ���������  
  null_0        db ?                     
  adr_in        dd ?            ;����� ������ ������

  ;--------------------------------------------------------------------

  ;-------------��������� ����������-----------------------------------

  Buf_out             DB 01h,01h,0c0h,0c2h,05h,8ah ;������ ���������(0)
  coun_out            EQU $ - Buf_out
  str_out             DB 9 dup(0)                 ;  ������������ � ��������
                                                   ;  ���������(1)
  coun_out_s          EQU $ - str_out 
 ; Buf_out_s          DB 1h,4h,0c4h,0c9h,6h,9h,2h,8h,0ach ;������ ���������(1)
   Buf_out_s          DB 1h,4h,0c4h,0c9h,6h,8h,2h,6h,0a9h
  Buf_out_p           DB 1h,4h,0c4h,0c9h,6h,09h,0h,7h,0a9h ;������ ���������(2)

                      add_com       DW ?           ; ����� COM �����

        noy1          DB " ���� �����",13,10,"$"
        noy2          DB " ���������� ������ �� ������",13,10,"$"
        noy3          DB " ������ ��������",13,10,"$"
        noy4          DB " ������ ��� ������ ���������",13,10,"$"
        noy5          DB " ���� ��� ������ �����  ���",13,10,"$"
        noy6          DB " �� � ��������� �� ������� ������",13,10,"$"
        noy7          DB " ������ �� ����������� ����� ���������",13,10,"$"
        noy8          DB " ������ ��� ?????????? ",13,10,"$"

   ;************************************************************
   ;   ����� ������ ��������

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
    ;                ��������� ���������
    ;----------------------------------------------------------------------           
               mov cs:mode,ah
               mov cs:n_zaprosa,al
               mov word ptr cs:adr_in,00h
               mov word ptr cs:adr_in+2,ds
    ;----------------------------------------------------------------------           
               or ah,ah               
               jz _obmen
               jmp _print
               
   _obmen:      
               or al,al
               jz _sost
               cmp cs:n_zaprosa,1
               jz _schet
               
   ;*********************************************************************
   ;  ������ ��������
   
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
    
   ; ������  �������� ��������� 0-������ ���������
    _sost:
            MOV CX,coun_out           ;������� �� ���������� �������� (������) ������
            mov di,offset cs:str_out
            mov si,offset cs:buf_out
            rep movsb 
            MOV CX,coun_out
   ;***********************************************************************         
    output_beg:
            XOR BX,BX                    ;� b� ��������� ������
            MOV DX,cs:add_com            ;������� �����
            in al,dx
  beg_loop:

            MOV AH,cs:str_out+BX         ;����� 1-�������
            CALL out_beg
            JAE cont
            JMP out_for_error

   cont:
            INC BX
            LOOP beg_loop
   ; �����  ����� �������� ���������
   
   ;*************************************************************
   ;������ ����� ������ ���������
            CALL test_in_zag
            JNC nach_in
            mov cs:f_sost,ah             
            jmp out_for_error
   nach_in:
            shl bx,1                     ;��������� �� ���������
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
              
              MOV ds:[si],AL          ;���� 1-�������
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

   ;����� ����� ������ ���������
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
            MOV SI,046ch                ;����� ������ �������
   time_aut:
            CLI                         ;��������� ����������
            MOV DI,ES:[SI]              ;di ��������� �� ������� �������� �������
            STI                         ;��������� ����������
            ADD DI,36
            JB time_aut                 ;��������� ���� ������ 2 ���.

            ADD DX,5                    ;��������� �� ������� ������� �����
   retry:
            IN AL,DX                    ;�������� ���� �������
            TEST AL,00100000b           ;��������� ���������� � ��������
            JNZ _out_char
            CLI
            CMP DI,ES:[SI]      ;���������� �����
            STI
            JAE retry
            SUB dx,5
            STC
            JMP err_out

  _out_char:
            XCHG AL,AH                   ;����� ��������
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
            MOV SI,046ch                ;����� ������ �������
   time_in_aut:
            CLI                         ;��������� ����������
            MOV DI,ES:[SI]              ;di ��������� �� ������� �������� �������
            STI                         ;��������� ����������
            ADD DI,36
            JB time_in_aut              ;��������� ���� ������ 2 ���.

            ADD DX,5                    ;��������� �� ������� ������� �����
   retry_in:
            IN AL,DX                    ;�������� ���� �������
            TEST AL,00000001b           ;��������� ���������� � ������
            JNZ  rr1_in
            CLI
            CMP DI,ES:[SI]      ;���������� �����
            STI
            JAE retry_in
            SUB DX,5
            STC
            MOV AH,0                    ;��� ������ �� �������
            JMP err_in
 rr1_in:
            TEST AL,00000010b
            JZ rr2_in
            SUB DX,5
            STC
            MOV AH,1                    ;����������� ������
            JMP err_in

 rr2_in:   TEST AL,00000100b
            JZ _in_char
            SUB DX,5
            STC
            MOV AH,2                    ;������ ��������
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
 ;**** ��������� ��������� ������ ��������� ****
 ;********************************************************************
  test_in_zag:

  XOR CX,CX
  XOR BX,BX
  CALL in_beg
  JAE test01                         ;�������� ������ 1-�������
  JMP end_in_err

 test01:

  CMP AL,01h
  JZ in_2
  MOV AH,04h                         ;�������� ������ ���������
  JMP end_in_err

 in_2:
  MOV bl,AL                          ;������� � ��������� 1-������
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
 MOV AH,08h                ;������� �� �����
 JMP end_in_err

 goy1:
  TEST AL,00100000b

 JZ goy2

 MOV AH,10h                ;��� ������
 JMP end_in_err

 goy2:

 TEST AL,00000001b
 JZ in_4
 INC CH                    ;�� ������ ���������

 in_4:

 ADD BL,AL
 ADC BL,0
 CALL in_beg
 JAE test04
 JMP end_in_err

 test04:

     cmp al,bl
    jz end_in
    MOV AH,20H                   ;������ �� ����������� ����� ���������

 end_in_err:

            stc
            ret

            end_in:

            CLC
            ret




  beg_ini:

  ;***********************************************************************
  ;                 ��������  � ��������� COM
  ;***********************************************************************

                         XOR AX,AX
                         MOV ES,AX       ;ES ��������� �� ������� ������ BIOS
                         
                         cmp AX,ES:[0400h]
                         JNZ  Norma

                         MOV DX,OFFSET cs:Mes
                         MOV AH,9H
                         INT 21H
                         JMP short end_prog
                         
        Norma:         
                         mov ax,es:[0400h]
                         MOV cs:add_com,AX               ;c�������� ���2
                         MOV DX,OFFSET cs:Messade        ;dx ��������� �� ������ ���������
                         MOV AH,9H                       ;����� ������
                         INT 21H

    ; ������������� �����
    ;********************************************************************
                         MOV DX,0                       ;�������� ���2
                         XOR AH,AH                      ;������� 0
                         MOV AL,0DFH                    ;���� ���������������� ������
                         INT 14H                        ;������������� �����

 ;**********************************************************************
 ;    ������������� ������ ����� ����������
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
  ;          ��������� ������� ���������� int 62h
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

  Messade   DB "              ��������������������������������  ",13,10
            db "              �   ������� ������ � ������    �  ",13,10
            db "              �   c ������������ ��������    �  ",13,10
            db "              �          ��������.           �  ",13,10
            db "              ��������������������������������  ",13,10
            db "              �  ������  ������  10/11/96�.  �  ",13,10
            db "              ��������������������������������  ",13,10,"$"

  Mes       DB   13,10,"               COM2 �� ���������������",13,10
            DB         "                ���������  ����������",13,10,"$"


  ENDP obmlom

  ENDS

  END obmlom


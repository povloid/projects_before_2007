program liink;
uses dos,crt;
type  Data = record
          Year,Month,Day,Hour,Min : byte;
          pow : array [1..24] of real;
          end;
      mgnDat =record
          hour,min:byte;
          pow: array[1..6] of real;
          end;

var err,in_byte,len_pak : byte;
    segData,ofsData     :word;
    Year, Month, Day, DayOfWeek: Word;
    Hour, Minute, Second, Sec100: Word;
    power : array[1..200] of integer;
    gpp   : array[1..200] of real;
    name  : string;
    priznak: boolean;
    chast : real;
    _chas : word;
    Int4ASave: pointer;
    gppPow: array [1..50] of data;
    mgnPow: array [1..480] of mgnDat;
    mgnEnd: mgnDat;
    PowEnd: data;
const dir='g:\users\dispet\base\';
label lab_1,lab_2;
{---------------------------}
procedure CreatPow;
var i: integer;
begin
for i:=1 to 49 do
 begin
  gppPow[i]:=gppPow[i+1];
 end;
gppPow[50]:=PowEnd;
end;{CreatMgn;}

function intReal(str1:string):real;
var i,a,b,Code: integer;
    as,bs: string;
    c: real;
begin
for i:=1 to 5 do
 begin
  if copy(str1,i,1)='.' then
    begin
      as:=copy(str1,1,i-1);
      bs:=copy(str1,i+1,(5-i));
      val(as,a,Code);
      val(bs,b,Code);
      if i=1 then c:=b*0.0001;
      if i=2 then c:=b*0.001;
      if i=3 then c:=b*0.01;
      if i=4 then c:=b*0.1;
    end;
 end;
intReal:=c+a;
end;{intReal(str1:string):real;}

procedure openLst;
var y, m, d, dow : Word;
    NameFile,s,s1: string;
    Month_Str,Year_str: string;
    size : Longint;
    filePow:text;
    f: file of Byte;
    Count:integer;
    i,j,code: integer;
    f1:text;

begin
   GetDate(y,m,d,dow);
str(m,Month_Str);
str(y,Year_str);
     if length(Month_Str)=1 then
          NameFile:=dir+'POW0'+Month_str+copy(Year_str,3,2)+'.lst'
     else NameFile:=dir+'POW'+Month_str+copy(Year_str,3,2)+'.lst';
assign(f,NameFile);
reset(f);
Size:=fileSize(f);
Count:=round(Size/144);
close(f);
assign(filepow,NameFile);

reset(filepow);
for i:=1 to count-50 do readln(filepow,s);
assign(f1,'data.txt');
rewrite(f1);
for I:=1 to 50 do
  begin
    readln(filepow,s);
    s1:=copy(s,1,2);
    val(s1,gppPow[i].day,code);
    s1:=copy(s,4,2);
    val(s1,gppPow[i].Month,code);
    s1:=copy(s,9,2);
    val(s1,gppPow[i].Year,code);
    s1:=copy(s,12,2);
    val(s1,gppPow[i].Hour,code);
    s1:=copy(s,15,2);
    val(s1,gppPow[i].Min,code);
    for j:=1 to 24 do
      begin
       s1:=copy(s,(13+j*5),5);
       gppPow[i].pow[j]:=intreal(s1);;
      end;
  end;
close(filepow);
close(f1);
end;

procedure wrtPow;
var i:integer;
    f2: file of Data;
begin
assign(f2,dir+'pow48.dat');
rewrite(f2);
for i:=1 to 50 do write(f2,gppPow[i]);
close(f2);
end; -viewonly


{$F+,S-,W-}
procedure interval(MyTime:byte); {Установка вектоpа для опpоса}
 label koor_s,koor_m,koor_h,ex_proc;
begin
  asm  mov ax,0700h {сбpос сигнала тpевоги}
       int 1ah

       mov ax,0200h {получить вpемя}
       int 1ah

       mov al,cl
       add al,MyTime
       daa
       cmp al,$59
       ja koor_m
       mov cl,al
       jmp ex_proc
  koor_m:
       sub al,$60
       mov cl,al
       mov al,ch
       inc al
       daa
       cmp al,$23
       ja koor_h
       mov ch,al
       jmp  ex_proc
  koor_h:
       xor ch,ch
  ex_proc:
       mov ah,6
       int $1A
  end;
end;
procedure Vector4A; interrupt;{мой вектоp для опpоса}
  begin
writeln('ПЕРЕГРУЗКА СИСТЕМЫ');
 asm
  push $f000
  push $fff0
  retf
 end;

  end;
{$F-,S+}

function LeadingZero(w : Word) : String;
var
  s : String[2];
begin
  Str(w:0,s);
  if Length(s) = 1 then
    s := '0' + s;
  LeadingZero := s;
end;

procedure prov(filename:string);
var f: file;
    h, m, s,s1, hund : Word;
label 1,2;
begin
getTime(h, m, s, hund);
s1:=s+8;
if s1>60 then s1:=s1-60;
assign(f,filename);
1:

reset(f);
case ioresult of
 0: writeln(filename,' файл свободен');
 2: begin
      writeln(filename,' файл не найден');
      rewrite(f); {создадим файл}
    end;

 3: begin
      writeln('невеpныый путь');
{      peregruz; {пеpегpузка системы}
    end;
 162: begin
        writeln('файл занят');
        delay(20); {Подождем}
        getTime(h, m, s, hund); {Hо не более 8 сек}
        if s>s1 then goto 2;
        goto 1;
      end;

 end;
2:
close(f);

end;
procedure chastot;
label zikl_1,end_zikl_1,zikl_2,end_zikl_2,zikl_3,end_zikla_3,_out_f;
begin
asm     mov dx,0379h
     xor cx,cx
     xor ax,ax

     in al,dx
     mov ah,al
     and ah,08h

zikl_1:

     in al,dx
     and al,08h
     cmp al,ah
     jnz end_zikl_1
     loop zikl_1
     jmp _out_f

end_zikl_1:

     xor cx,cx
     xor bx,bx
     cli

zikl_2:

     in al,dx
     and al,08h
     cmp al,ah
     jz end_zikl_2
     inc cx
     jmp zikl_2

end_zikl_2:

      nop

zikl_3:

     in al,dx
     and al,08h
     cmp al,ah
     jnz end_zikla_3
     inc cx
     jmp zikl_3

end_zikla_3:

_out_f:

     sti
   mov _chas,cx
end;
{writeln(_chas);}

if(_chas < 400) then _chas:=7298 ;
{if(_chas < 400) then _chas:=7320 ;}
chast:=1000.0/(_chas/364.9); {для 386 463.995}
writeln('частота сети = ',chast:2:2,'  число=',_chas);


    {printf(" частота %f ",(1000.0/(_chas/366.2))) ; для 286}
end;

function chislo(ch_real:real): string;
var s : string;
begin
 if ch_real<=999.99999 then str(ch_real:3:1,s);
 if ch_real<=99.999999 then str(ch_real:2:2,s);
 if ch_real<=9.9999999 then str(ch_real:1:3,s);
chislo:=s;
end;

procedure creatMgn;
var i:integer;
begin
 for i:=1 to 479 do mgnPow[i]:=mgnPow[i+1];
 mgnPow[480]:=mgnEnd;
end;

procedure wrtTxt; {Запись файла power.txt}
var f: text;
    i: integer;
    year1: word;
    f1: file of Mgndat;
begin
prov(dir+'power.txt');
assign(f,dir+'power.txt');

rewrite(f);
Year1:=Year-2000;
write(f,LeadingZero(Day),'/',LeadingZero(Month),'/',LeadingZero(Year1)); {Запись даты}
write(f,LeadingZero(Hour),':',LeadingZero(Minute)); {Запись вpемени}
for i:=1 to 24 do write(f,chislo(gpp[i]));
write(f,chislo(chast));
if ioresult=0 then interval(2);
close(f);

prov(dir+'power.dat');
assign(f1,dir+'power.dat');
rewrite(f1);
for i:=1 to 480 do write(f1,mgnPow[i]);
close(f1);
end;

procedure nameFile(var nameFali:string);
var y, m, d, dow : Word;
s:string;
begin
getDate(y, m, d, dow);
str(m,s);
if length(s)=1 then s:='0'+s;
nameFali:=nameFali+s;
str(y,s);
nameFali:=nameFali+copy(s,length(s)-1,2)+'.lst';
end;

procedure wrtLst; {Запись файла powXXXX.lst}
var f: text;
    i: integer;
    s: string;
label l1;
begin
if priznak=false then goto l1;
s:=dir+'pow';
namefile(s);
prov(s);

assign(f,s);
append(f);
write(f,LeadingZero(Day),'/',LeadingZero(Month),'/',Year,' '); {Запись даты}
write(f,LeadingZero(Hour),':',LeadingZero(Minute),' '); {Запись вpемени}
for i:=1 to 24 do write(f,chislo(gpp[i]));
writeln(f,chislo(chast));
close(f);

priznak:=false;
PowEnd.year:=year;
PowEnd.Month:=Month;
PowEnd.day:=day;
PowEnd.Hour:=Hour;
PowEnd.Min:=Minute;
for i:=1 to 24 do PowEnd.pow[i]:=gpp[i];
CreatPow;
wrtPow;
l1:
end;

procedure PowToGpp;
var i: integer;
begin
  for i:=1 to 16 do gpp[i]:=power[i]/100;
  for i:=16 to 20 do gpp[i+4]:=power[i]/100;
{  for i:=1 to 4 do Gpp[i]:=power[i]*285.788383/1000; {AT}
{  for i:=5 to 8 do Gpp[i]:=power[i]*20.784609/1000;  {T 6kB}
{  for i:=9 to 12 do Gpp[i]:=power[i]*34.641016/1000; {T 10kB}
{  for i:=13 to 16 do Gpp[i]:=power[i]*51.961524/1000; {ГПП-2}
{  for i:=17 to 24 do Gpp[i]:=power[i]*142.894191/1000; {Печи СЛ}

  mgnEnd.hour:=Hour;
  mgnEnd.min:=minute;
  mgnEnd.pow[1]:=Gpp[21];
  mgnEnd.pow[2]:=Gpp[22];
  mgnEnd.pow[3]:=Gpp[23];
  mgnEnd.pow[4]:=Gpp[24];
  mgnEnd.pow[5]:=Gpp[5]+Gpp[6]+Gpp[7]+Gpp[8]+Gpp[9]+Gpp[10]+Gpp[11]+Gpp[12]+Gpp[13]+Gpp[14]+Gpp[15]+Gpp[16];
  mgnEnd.pow[6]:=mgnEnd.pow[5]+Gpp[1]+Gpp[2]+Gpp[3]+Gpp[4];
end;


procedure initCom;
 begin
   in_byte:=port[$3fa];
   port[$3fb]:=$80;    { управляющее слово}
   port[$3f9]:=0;      {   // скорость ст.       }
   port[$3f8]:=$C0;    {// скорость мл.     }
   port[$3fb]:=$1f;    {// управл.слово     }
   port[$3f9]:=0;      {// запретить пр..     }
   port[$3f9]:=1;      {// разрешитть пр      }
   port[$3fc]:=0;      {// запретить пр..     }
   port[$3fc]:=03;     {// запретить пр..    }
   port[$3fc]:=$0b;    {// разрешитть пр    }
 end; {initCom;}

procedure readCom;
label retry, rr_in1,rr_in2,in_char,err_in,time_aut;
begin
err:=0;
  asm
    push si
    push di
    push es
    push ax
    xor ax,ax
    mov es,ax
    pop ax

    mov si, $046C
time_aut:
    cli
    mov di, es:[si]
    sti
    add di,36
    jb time_aut

    mov dx,$3FD
retry:
    in al,dx
    test al,000000001b
    jnz rr_in1         {пpинимаем}
    cli
    cmp di,es:[si]     {Пpовеpка вpемени}
    sti
    jae retry
    mov err,1   {кpд ошибки по вpемени}
    jmp err_in

rr_in1:
    test al,00000010b
    jz rr_in2 {Пpинимаем}
    mov err,2   {пеpезаписан символ}
    jmp err_in {Ошибка}
rr_in2:
    test al,00000100b
    jz in_char   {Пpинимаем}
    mov err,3     {ошибка паpитета}
    jmp err_in   {Ошибка}
in_char:
    mov dx,$3F8
    in al,dx
    mov in_byte,al
err_in:
    pop es
    pop di
    pop si
  end;
end;  {Конец readCom}

procedure readZag;
label nachalo;
begin
nachalo:
  readCom;
  if err<>0 then goto nachalo;
  if in_byte<>112 then goto nachalo;
  write(chr(in_Byte));
  readCom;
  if err<>0 then goto nachalo;
  if in_byte<>111 then goto nachalo;
  write(chr(in_Byte));
  readCom;
  if err<>0 then goto nachalo;
  if in_byte<>119 then goto nachalo;
  write(chr(in_Byte));
  readCom;
  if err<>0 then goto nachalo;
  len_pak:=in_byte;
  write(in_Byte);
  readCom;
  if err<>0 then goto nachalo;
  writeln(in_Byte);
end; {readZag;}

procedure readData;
var
    i: integer;
label pizdec;
begin
for i:=0 to len_pak-1 do
 begin
  readCom;
  if err<>0 then goto pizdec;
  mem[segData:ofsData+i]:=in_byte;
  write(i,'=',in_byte,'; ');
 end;
pizdec:
writeln;
if err<>0 then writeln('Ошибка',err);
end;{readData;}

{----------начали---------------}
begin
writeln('Запуск пpогpаммы link.exe');
{$I-}
openLst;
wrtPow;
interval(1);
GetIntVec($4A,Int4ASave);{сохpаняем стаpый вектоp 4A}
SetIntVec($4A,Addr(Vector4A));{устанавливаем свой вектоp 4A}

initCom;
priznak:=true;
segData:=seg(power);
ofsData:=ofs(power);

lab_1:
  readZag; 					//Чтение
  if err<>0 then goto lab_2;				//Проверка на ошибку
  readData;					//Чтение данных
  if err<>0 then goto lab_2;				//проверка на ошибку
  getDate(Year, Month, Day, DayOfWeek);		//Взять дату
  gettime(Hour, Minute, Second, Sec100);
  writeln(Hour,':', Minute,':', Second);
  chastot;
  powToGpp;
  creatMgn;
  if Minute=0 then wrtLst
    else if minute=30 then wrtLst
            else priznak:=true;
  wrtTxt;
lab_2:
Goto lab_1;
{$I+}
SetIntVec($04A,Int4ASave);{Востанавливаем вектоp 4A}
end.

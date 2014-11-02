
program spider;

uses  crt,dos;

var   color,paper,two:       byte;
      stepsec,sec,min:       byte;
      stephun,hun,hour:      byte;
      number,i,q:            byte;
      opentime3:             byte;
      opentime1,trig1:       word;
      opentime2,trig2:       word;
      ciclemax,ciclemax0:    word;
      downborder1,upborder1: word;
      downborder2,upborder2: word;
      trigdown1,trigtime1:   word;
      trigdown2,trigtime2:   word;
      cicle,div1,div2,div3:  word;
      ch,proc:               char;
      res:                   longint;
      hard1,hardfix1,stop:   boolean;
      hard2,hardfix2,disk:   boolean;
      hardmode1,ending:      boolean;
      hardmode2,frozen:      boolean;
      border1,border2:       string[16];
      reg :                  registers;
      kl:                    array [1..6]   of byte;
      huntmr,timehun:        array [1..16]  of byte;
      sectmr,timesec:        array [1..16]  of word;
      timerOn:               array [1..16]  of boolean;
      timerOff:              array [1..16]  of boolean;
      circuit:               array [1..2]   of boolean;

const X:                     array [1..6]   of byte=(15,15,15,55,55,55);
      Y:                     array [1..6]   of byte=(13,17,21,13,17,21);
      Xtime:                 array [1..6]   of byte=(31,31,31,71,71,71);
      Ytime:                 array [1..6]   of byte=(14,18,22,14,18,22);
      ver:                   array [1..6]   of string[7]=('7  /  8','4  /  8',
                                                          '1  /  8','9  /  2',
                                                          '6  /  2','3  /  2');
      day:                   array [1..7]   of string[2]=('­','‚â','‘à','—â',
                                                          'â','‘¡','‚á');
      mon:                   array [1..12]  of string[4]=('ï­¢.','ä¥¢.','¬ à.',
                                                          ' ¯à.','¬ ï ','¨î­ï',
                                                          '¨î«ï',' ¢£.','á¥­.',
                                                          '®ªâ.','­®ï¡','¤¥ª.');
      Z:                     byte=3;
      open:                  byte=7;
      p:                     array[1..5] of char=('p','o','i','n','t');
      life:                  byte=15; { 255-¢à¥¬ï }
      mmx:                   byte=59; { EF }
      time=1080;
      ciclemaxDown=360;      { 360 }
      ciclemaxUp=1800;

{----------------------------------------------------------------------------}

procedure timer(i:byte);
begin
if timesec[i] >= sectmr[i] then
if timehun[i] >= huntmr[i] then begin
                                timerOn[i]:=false;
                                timerOff[i]:=true;
                                end;
timehun[i]:=timehun[i]+stephun;
if timehun[i] > 99 then begin
                   timehun[i]:=timehun[i]-100;
                   timesec[i]:=timesec[i]+1;
                   end;
end;

{----------------------------------------------------------------------------}

procedure timerGo(i,s,h:byte);
begin
if not timerOn[i] then
 begin
 sectmr[i]:= s;
 huntmr[i]:= h;
 timesec[i]:=  0;
 timehun[i]:=  0;
 timerOn[i]:=true;
 end;
end;

{----------------------------------------------------------------------------}

procedure alarm(tone: word; t: byte);
begin
sound(tone);
timerGo(10,0,t);
end;

{----------------------------------------------------------------------------}

procedure klapan(num_kl: byte; dir: byte);
{ num_kl - ­®¬¥à ª« ¯ ­ , dir=1  - ®âªàëâì, dir=0 - § ªàëâì }
var op,cl: byte;
begin
     case num_kl of
     1: begin op:=1;  cl:=254; end;
     2: begin op:=2;  cl:=253; end;
     3: begin op:=4;  cl:=251; end;
     4: begin op:=8;  cl:=247; end;
     5: begin op:=16; cl:=239; end;
     6: begin op:=32; cl:=223; end;
     7: begin op:=64; cl:=191; end;
     8: begin op:=128;cl:=127; end;
     end;
     case dir of
     0: port[$378]:= port[$378] and cl;
     1: port[$378]:= port[$378] or  op;
     end;
end;      { ª®­¥æ ¯à®æ¥¤ãàë klapan }

{----------------------------------------------------------------------------}

procedure current(num_cur,status: byte);
begin
     case status of
     0: case num_cur of
        1,4:     klapan(num_cur,1);
        2,3,5,6: klapan(num_cur,0);
        end;
     1: case num_cur of
        1: begin klapan(1,0); klapan(2,0); klapan(3,0); end;
        2: begin klapan(1,1); klapan(2,1); klapan(3,0); end;
        3: begin klapan(1,1); klapan(2,0); klapan(3,1); end;
        4: begin klapan(4,0); klapan(5,0); klapan(6,0); end;
        5: begin klapan(4,1); klapan(5,1); klapan(6,0); end;
        6: begin klapan(4,1); klapan(5,0); klapan(6,1); end;
        end;
     end;
if status = 1 then alarm(50,7);
end;
{----------------------------------------------------------------------------}

procedure nul(a:byte);
begin
if a<10 then write('0');
write(a);
end;

{----------------------------------------------------------------------------}

procedure probel(a:word;r:byte);
label 1,2,3;
begin
if r=1 then goto 1;
if r=2 then goto 2;
if r=3 then goto 3;
3: if a < 1000 then write(' ');
2: if a <  100 then write(' ');
1: if a <   10 then write(' ');
write(a);
end;

{----------------------------------------------------------------------------}

procedure prntime(s:word);
begin
probel(s div 60,1);
write(':');
nul(s mod 60);
end;

{----------------------------------------------------------------------------}

procedure readdate;
var numday: byte;
begin
reg.AH:=$2a;
intr($21,reg);
numday:=reg.AL;
if numday = 0 then numday:=7;
textcolor(3);
write(day[numday]);  write(',');
probel(reg.DL,1);    write(' ');
write(mon[reg.DH]);
textcolor(7);
end;

{----------------------------------------------------------------------------}

procedure readtime;
begin
reg.AH:=$2c;
intr($21,reg);
gotoxy(70,2); textcolor(3);
if reg.CH<10 then write(' ');
textbackground(0);
write(' ');
write(reg.CH);write(':');
nul(reg.CL);write(':');
nul(reg.DH);write(' ');
stepsec:= reg.DH+60-sec;
stephun:= reg.DL+100-hun;
if stepsec>59 then stepsec:= stepsec-60;
if stephun>99 then stephun:= stephun-100;
if not frozen then cicle:=cicle+stepsec;
if cicle >= ciclemax then cicle:=0;
sec:=reg.DH;
hun:=reg.DL;
end;              {  ª®­¥æ procedure readtime  }

{----------------------------------------------------------------------------}

function bit(word,n:byte):byte; { ¢ë¤¥«¥­¨¥ n-£® ¡¨â  ¨§ ç¨á«  word }
var b: byte;
begin
     case n of
          0: b:=1;
          1: b:=2;
          2: b:=4;
          3: b:=8;
          4: b:=16;
          5: b:=32;
          6: b:=64;
          7: b:=128;
     end;
if b and word > 0 then bit:=1
                  else bit:=0;
end;   { bit }

{----------------------------------------------------------------------------}

procedure cursor_off;
begin
     reg.AX:=$0100;
     reg.CX:=$2607;
     intr($10,reg);
end;    { cursor_off }

procedure print8(bit8: byte);
var i:byte;
begin
      for i:=7 downto 0 do write(bit(bit8,i));
end;    { ª®­¥æ ¯à®æ¥¤ãàë print8 }

{----------------------------------------------------------------------------}

procedure designcut;
begin
write('º                                                                              º');
write('º                                                                              º');
write('º                                                                              º');
write('ÌÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍ»ÉÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍ¹');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('ÇÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ¶ÇÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ¶');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('ÇÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ¶ÇÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄ¶');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('º            ³            ³            ºº            ³            ³            º');
write('ÈÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÊÊÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍÍ¼');
textcolor(14);
gotoXY(5,14);  write('‚II ƒI');
gotoXY(5,18);  write(' I/II');
gotoXY(5,22);  write('‚I ƒII');
gotoXY(44,14); write('‚IV ƒIII');
gotoXY(44,18); write(' III/IV');
gotoXY(44,22); write('‚III ƒIV');
end;

{----------------------------------------------------------------------------}

procedure design; { ®ä®à¬«¥­¨¥ }
begin
textbackground(0); clrscr; gotoXY(1,1); { ®ç¨áâª  íªà ­  ¢ ç¥à­ë© }
textbackground(1); textcolor(2);
write('ÉÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍ»');
write('º          º ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍ» ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ» ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍ» º          º');
write('ÌÍÍÍÍÍÍÍÍÍÍ¼ º              º º                  º º              º ÈÍÍÍÍÍÍÍÍÍÍ¹');
write('º ÚÄÄÄÄÄÄÄÄ¿ º              º ÇÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄ¶ º              º ÚÄÄÄÄÄÄÄÄ¿ º');
write('º ³        ³ ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼ º          ³       º ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼ ³        ³ º');
write('º ³        ³                  ÇÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄ¶                  ³        ³ º');
write('º ³        ³                  º          ³       º                  ³        ³ º');
write('º ÀÄÄÄÄÄÄÄÄÙ                  ÈÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍ¼                  ÀÄÄÄÄÄÄÄÄÙ º');
designcut;
textcolor(11);
gotoXY(35,3);write('‚à¥¬ï  æ¨ª« ');
gotoXY(33,5);write('’¥ªãé¥¥');
gotoXY(16,3);write('¥¦¨¬ 1 ¯ àë');
gotoXY(54,3);write('¥¦¨¬ 2 ¯ àë');
gotoXY(1,25); textbackground(0);
textcolor(7);  write('  F1');
textcolor(2);  write(' ‘¯à ¢ª   ');
textcolor(12); write(' Alt');
textcolor(7);  write('+');
textcolor(12); write('F3');
textcolor(2);  write(' ¥à¥å®¤  ');
textcolor(7);  write(' F5');
textcolor(2);  write(' ‚à¥¬ï  ');
textcolor(7);  write(' F7,F8');
textcolor(2);  write(' ¥¦¨¬  ');
textcolor(12); write(' F9');
textcolor(2);  write(' ‘â®¯!  ');
textcolor(7);  write(' F10');
textcolor(2);  write('  « ¤ª ');
textbackground(1);
end;    { design }

{----------------------------------------------------------------------------}

procedure hint(n,o:byte);
var d: byte;
begin
if n=2 then d:=39 else d:=0;
gotoXY(16+d,6);
if o=1 then begin
            textcolor(15);
            write('ˆá¯®«ì§ã©â¥ ');
            gotoXY(18+d,7);
            write('ª« ¢¨è¨');
            gotoXY(19+d,8);
            textcolor(30); write(' ');
            textcolor(15); write('¨ ');
            textcolor(30); write(' ');
            end
       else begin
            textcolor(1);
            write('           ');
            gotoXY(18+d,7);
            write('       ');
            gotoXY(19+d,8);
            write('     ');
            end;
end;

{----------------------------------------------------------------------------}

procedure cictime;
begin
ciclemax:=ciclemax0;
upborder1:= round(ciclemax/12);
div1:= upborder1+upborder1+upborder1;
div2:=        div1+div1;
div3:=        div2+div1;
downborder1:= div2-upborder1;
upborder2:=   div1+upborder1;
downborder2:= div3-upborder1;
trigdown1:=div2+upborder1;
trigdown2:=div3+upborder1;
end;

{----------------------------------------------------------------------------}

procedure changecicle;
begin
hint(2,1);
if stephun>0 then inc(two); if two=30 then two:=0;
if stepsec=1 then if paper<>0 then paper:=0 else paper:=4;
textcolor(23); color:=14;
if (ch=#72) or (ch=#80) then inc(q);
if q = 10 then begin
 if ch = #72 then ciclemax0:=ciclemax0+10;
 if ch = #80 then ciclemax0:=ciclemax0-10;
 if ciclemax0 > ciclemaxUp   then ciclemax0:= ciclemaxUp;
 if ciclemax0 < ciclemaxDown then ciclemax0:= ciclemaxDown;
 case ciclemax0 of
      ciclemaxDown,time,ciclemaxUp: alarm(200,14);
 end;
 q:=0;
end;
 if ch<>#0 then begin
               timerOn[9]:=false;
               timerGo(9,5,0);
               end;
 case ciclemax0 of
     ciclemaxDown,time,ciclemaxUp: color:=15;
 end;
 if  timerOff[9] or (ch=#27) or (ch=#13) then begin
      { “á«®¢¨¥ ®ª®­ç ­¨ï }                   hint(2,0);
                                              textcolor(11);
                                              paper:=1;
                                              color:=10;
                                              timerOn[9]:=false;
                                              proc:=#0; q:=0;
                                              end;
gotoXY(33,7);write('‡ ¤ ­­®¥');
textcolor(color);
textbackground(paper);
gotoXY(44,7);
prntime(ciclemax0);
textbackground(1);
if paper=1 then paper:=15;
end;

{----------------------------------------------------------------------------}

procedure changemode1;
begin
hint(1,1);
textcolor(30);
if ch<>#0 then begin
               timerOn[8]:=false;
               timerGo(8,5,0);
               end;
if timerOff[8] or (ch=#27) or (ch=#13) then
               begin
               hint(1,0);
               if ch=#13 then hardfix1:=hard1 else hard1:=hardfix1;
               if hardfix1 then begin
                                 textcolor(12);
                                 textbackground(0);
                                 end
                            else textcolor(10);
               timerOn[8]:=false;
               proc:=#0;
               end;
gotoXY(15,4);
if (ch=#72) then hard1:=true;
if (ch=#80) then hard1:=false;
if hard1 then write('   †¥áâª¨©    ') else write('€¢â®¬ â¨ç¥áª¨©');
textbackground(1);
end;                { changemode1 }

{----------------------------------------------------------------------------}

procedure changemode2;
begin
hint(2,1);
textcolor(30);
if ch<>#0 then begin
               timerOn[8]:=false;
               timerGo(8,5,0);
               end;
if timerOff[8] or (ch=#27) or (ch=#13) then
               begin
               hint(2,0);
               if ch=#13 then hardfix2:=hard2 else hard2:=hardfix2;
               if hardfix2 then begin
                                 textcolor(12);
                                 textbackground(0);
                                 end
                            else textcolor(10);
               timerOn[8]:=false;
               proc:=#0;
               end;
gotoXY(53,4);
if (ch=#72) then hard2:=true;
if (ch=#80) then hard2:=false;
if hard2 then write('   †¥áâª¨©    ') else write('€¢â®¬ â¨ç¥áª¨©');
textbackground(1);
end;                { changemode2 }

{----------------------------------------------------------------------------}

procedure quadrat(n,color: byte);
var i: byte;
begin
textcolor(color);
for i:= 0 to 2 do begin
                  gotoXY(X[n],Y[n]+i);
                  write('ÛÛÛÛÛÛÛÛÛÛÛÛ');
                  end;
end;

{----------------------------------------------------------------------------}

procedure changenumber(para:byte; g:byte);  { g = 0/1 }
var c1,c2,g1,g2:byte;
begin
case para of
     0:   begin c1:=g;  c2:=g;  g1:=g;  g2:=g  end;
     1:   begin c1:=10; c2:=g;  g1:=13; g2:=g  end;
     2:   begin c1:=g;  c2:=10; g1:=g;  g2:=13 end;
end;
gotoXY(4,11); textcolor(c1);textbackground(g1);write(' 1 ¯ à  ');
gotoXY(44,11);textcolor(c2);textbackground(g2);write(' 2 ¯ à  ');
textbackground(1);
end;    { changenumber }

{----------------------------------------------------------------------------}

procedure restart;
begin
textbackground(0); design;
gotoxy(2,2); readdate;
timerOff[9]:=true;
changecicle;
timerOff[8]:=true;
changemode1;
changemode2;
changenumber(number,1);
end;    { restart }

{----------------------------------------------------------------------------}

function fix(n:byte): boolean;
var b: byte;
begin
fix:=false;
case n of
     1: begin b:=bit(port[$379],4); end;
     2: begin b:=bit(port[$379],5); end;
end;
if (not circuit[n]) and ( b = 1 ) then begin
                                       fix:=true ;
                                       circuit[n]:=true;
                                       end;
if b = 0 then circuit[n]:=false;
end;

{----------------------------------------------------------------------------}

procedure exfile;
var       pauk:         file of char;
          buf:          char;
begin
disk:=false;
assign(pauk,'xspider.exe');
reset(pauk);
seek(pauk,res);
read(pauk,buf);
i:=ord(buf);
inc(i);
if i > 255 then i:=255;
hour:=255-i;
buf:=chr(i);
seek(pauk,res);
write(pauk,buf);
close(pauk);
end;

{----------------------------------------------------------------------------}

procedure erasefile;
var       pauk: file of char;
          pos:  word;
          size: word;
const     nul:  char=#0;
begin
assign(pauk,'xspider.exe');
reset(pauk);
size:=filesize(pauk);
for pos:=0 to size do begin;
                      seek(pauk,pos);
                      write(pauk,nul);
                      end;
rename(pauk,'__privet.ura');
erase(pauk);
alarm(3000,7);                   {   !!!   }
end;

{----------------------------------------------------------------------------}

procedure basic(ground:byte);
var klapOn: byte;
label helpmode;
begin
readtime; ch:=#0;
if keypressed then ch:=readkey;
if fix(1) then begin
               alarm(5000,7);
               if (cicle>=upborder1) and (cicle<div1) then
                  trigtime1:= div2 - (div1-cicle) div 2;
               if cicle = div1 then trigtime1:=div2; { div1 }
               if (cicle>div1) and (cicle<=downborder1) then
                  trigtime1:= div2 + (cicle-div1) div 2;
               if (cicle<upborder1) or (cicle>=div3) then
                  trigtime1:=downborder1;;
               if (cicle>downborder1) and (cicle<div3) then
                  trigtime1:=trigdown1;
               end;
if fix(2) then begin
               alarm(5000,7);
               if (cicle>=upborder2) and (cicle<div2) then
                  trigtime2:= div3 - (div2-cicle) div 2;
               if cicle = div2 then trigtime2:=div3; { div2 }
               if (cicle>div2) and (cicle<=downborder2) then
                  trigtime2:= div3 + (cicle-div2) div 2;
               if cicle<upborder2   then trigtime2:=downborder2;
               if cicle>downborder2 then trigtime2:=trigdown2;
               end;

{----------------------------------------------------------------------------}

 if (cicle = 0) and (stepsec = 1) then
    begin                                                { 1 }
    current(1,0); timerGo(3,Z,0); number:=0;
    if not timerOn[9] then cictime;         { !!! }
    trigtime1:=div2;
    end;
 if timerOff[3] then begin
                     current(2,1); timerGo(11,open,0); opentime3:=0;
                     end;
 if timerOff[11] then begin
                     current(2,0); timerGo(2,Z,0);
                     end;
 if timeroff[2] then begin
                     current(3,1); opentime1:=0; number:=2;
                     end;

{----------------------------------------------------------------------------}

 if (cicle = div1) and (stepsec = 1) then
    begin                                                { 2 }
    current(4,0); timerGo(4,Z,0);
    number:=0;
    trigtime2:=div3;
    end;
 if timerOff[4] then begin
                     current(5,1); timerGo(16,open,0); opentime3:=0;
                     end;
 if timerOff[16] then begin
                     current(5,0); timerGo(5,Z,0);
                     end;
 if timerOff[5] then begin
                     current(6,1); opentime2:=0; number:=1;
                     end;

{---------------------------------------------------------------------------}

 if (cicle = trig1) and (stepsec = 1) then
    begin                                                { 3 }
    current(3,0); timerGo(1,Z,0); number:=0;
    end;
 if timerOff[1] then begin
                     current(2,1); timerGo(13,open,0); opentime3:=0;
                     end;
 if timerOff[13] then begin
                     current(2,0); timerGo(12,Z,0);
                     end;
 if timeroff[12] then begin
                     current(1,1); opentime1:=0;number:=2;
                     end;

{---------------------------------------------------------------------------}

 if (cicle = trig2) and (stepsec = 1) then
    begin
    current(6,0); timerGo(6,Z,0); number:=0;
    end;                                                 { 4 }
 if timerOff[6] then begin
                     current(5,1); timerGo(14,open,0); opentime3:=0;
                     end;
 if timerOff[14] then begin
                     current(5,0); timerGo(15,Z,0);
                     end;
 if timerOff[15] then begin
                     current(4,1); opentime2:=0;number:=1;
                     end;

{---------------------------------------------------------------------------}

if (stepsec = 1) and (sec = 0) then begin
                                    min:=min+1;
                                    if min > mmx then begin
                                                      min:=0;
                                                      disk:=true;
                                                      end;
                                    end;
if disk {and (cicle > downborder2)} and (number <> 0{= 1}) then exfile;
if hour = 0    then erasefile;

if ground = 2 then goto helpmode;
for i:=1 to 6  do begin
                  if bit(port[$378],i-1) = 1 then case i of
                                                  2,3,5,6:       klapOn:=6;
                                                  1,4:      klapOn:=ground;
                                                  end
                                             else case i of
                                                  2,3,5,6:  klapOn:=ground;
                                                  1,4:      klapOn:=6;
                                                  end;
                  quadrat(i,klapOn);
                  end;
helpmode:
for i:=1 to 16 do begin
                  timerOff[i]:=false;
                  if timerOn[i] then timer(i);
                  end;
if timerOff[10] then nosound;
end;   {   BASIC   }

{---------------------------------------------------------------------------}

procedure opentime;
begin
 for i:=1 to 6 do  begin
                   kl[i]:=bit(port[$378],i-1);
                   if (i=1) or (i=4) then if kl[i]=0 then kl[i]:=1
                                                     else kl[i]:=0;
                   gotoXY(Xtime[i],Ytime[i]);
                   if kl[i]=0 then begin
                                   textcolor(1);
                                   write('      ');
                                   end
                              else begin
                                   textcolor(0); textbackground(3);
                                   if (i=1) or (i=3) then prntime(opentime1);
                                   if (i=4) or (i=6) then prntime(opentime2);
                                   if (i=2) or (i=5) then prntime(opentime3);
                                   end;
                   textbackground(1);
                   end;
 if stepsec = 1 then begin
                     inc(opentime1);
                     inc(opentime2);
                     inc(opentime3);
                     end;
end;

{---------------------------------------------------------------------------}

procedure indication;
begin
textbackground(0);
gotoxy(45,2); textcolor(7);
if bit(port[$379],4)=0 then write(' §®¬ª­ãâ')
                       else begin
                            textcolor(12);
                            write(' ‡ ¬ª­ãâ ');
                            end;
gotoxy(45,4); textcolor(7);
if bit(port[$379],5)=0 then write(' §®¬ª­ãâ')
                       else begin
                            textcolor(12);
                            write(' ‡ ¬ª­ãâ ');
                            end;
end;

{----------------------------------------------------------------------------}

{procedure stopcicle(paper:byte);
var y: byte;
begin
if paper = 1 then y:=9 else y:=7;
frozen:=true;  gotoxy(13,y);
textcolor(28); write('‚­¨¬ ­¨¥! ');
textcolor(14); write('–¨ª«®£à ¬¬  ®áâ ­®¢«¥­ ! ');
textcolor(10); write('‚®§¢à â - ª« ¢¨è  ');
textcolor(31); write('Esc');
repeat
readtime;
if paper = 1 then opentime else indication;
if keypressed then ch:=readkey;
if stepsec=1 then sound(4500) else nosound;
until ch = #27;
ch:=#0; nosound; gotoxy(13,y);
textbackground(paper);
write('                                                        ');
frozen:=false; proc:=#0;
end;}

procedure stopcicle(paper:byte);
var y: byte;
label ending;
begin
if paper = 1 then y:=9 else y:=7;
if number = 0 then goto ending;
frozen:=true;  gotoxy(13,y);
textcolor(28); write('‚­¨¬ ­¨¥! ');
textcolor(14); write('–¨ª«®£à ¬¬  ®áâ ­®¢«¥­ ! ');
textcolor(10); write('‚®§¢à â - ª« ¢¨è  ');
textcolor(31); write('Esc');
{if keypressed then ch:=readkey;}
if stepsec=1 then sound(4500) else nosound;
ending:
if (ch = #27) or (number = 0) then begin
                                   proc:=#0; nosound;
                                   ch:=#0; gotoxy(13,y);
                                   textbackground(paper);
                                   write('                                                        ');
                                   frozen:=false;
                                   end;
end;

{----------------------------------------------------------------------------}

procedure verify;     { ¯à®æ¥¤ãà  ¯à®¢¥àª¨ á ª« ¢¨ âãàë }
begin
timerOn[9]:=false; gotoXY(1,1);
textcolor(2); textbackground(0);
write('ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍÍÍÍËÍÍÍÍÍÍÍÍÍÍ»');
write('º                      º                  ³           º             º          º');
write('º                      ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄ¶             ÈÍÍÍÍÍÍÍÍÍÍ¹');
write('º                      º                  ³           º                        º');
write('º                      ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÏÍÍÍÍÍÍÍÍÍÍÍ¼                        º');
write('º                                                                              º');
write('º                                                                              º');
write('º                                                                              º');
designcut;
textcolor(3);
gotoXY(26,2);  write('Š«îç ‘’ 1-© ¯ àë');
gotoXY(26,4);  write('Š«îç ‘’ 2-© ¯ àë');
textcolor(2);
gotoXY(27,9);  write('ÉÍÍÍÍÍÍÍÍÍÍÍÍ»');
gotoXY(27,10); write('º');
textcolor(5);
gotoXY(29,10); write(' Š« ¢¨è¨');
textcolor(2);
gotoXY(40,10); write('º');
gotoXY(27,11); write('º');
textcolor(5);
gotoXY(29,11); write('ã¯à ¢«¥­¨ï');
textcolor(2);
gotoXY(40,11); write('º');
gotoXY(27,12); write('ØÍÍÍÍÍÍÍÍÍÍÍÍ¹');
gotoXY(67,9);  write('ÉÍÍÍÍÍÍÍÍÍÍÍÍ¹');
gotoXY(67,10); write('º');
textcolor(5);
gotoXY(69,10); write(' Š« ¢¨è¨');
textcolor(2);
gotoXY(67,11); write('º');
textcolor(5);
gotoXY(69,11); write('ã¯à ¢«¥­¨ï');
textcolor(2);
gotoXY(80,11); write('º');
gotoXY(67,12); write('ØÍÍÍÍÍÍÍÍÍÍÍÍ¹');
gotoXY(1,25);
textcolor(27); write('               Esc ');
textcolor(7);  write('- ');
textcolor(3); write('¢ëå®¤                      ');
textcolor(12); write('F9 ');
textcolor(7);  write('- ');
textcolor(3); write('áâ ­®¢ æ¨ª«®£à ¬¬ë       ');

for i:=1 to 6 do begin
                 textcolor(15);
                 gotoXY(Xtime[i],Ytime[i]); write(ver[i]);
                 end;
changenumber(number,0);
repeat
basic(0);
changenumber(number,0);
indication;
textcolor(3); gotoXY(9,2);
print8(port[$379]);
gotoXY(9,4);
if min > 0 then begin
                probel(hour-1,2);
                write('  ');
                probel(mmx+1-min,2)
                end
           else begin
                probel(hour,2);
                write('   ');
                probel(min,1);
                end;
gotoXY(9,6);
print8(port[$378]);
 case ch of

 '7':                     current(1,1);
 '4':                     current(2,1);
 '1':                     current(3,1);
 '9':                     current(4,1);
 '6':                     current(5,1);
 '3':                     current(6,1);
 '8':                     begin
                          current(1,0);
                          current(2,0);
                          current(3,0);
                          end;
 '2':                     begin
                          current(4,0);
                          current(5,0);
                          current(6,0);
                          end;
 '5':                     begin
                          current(1,0);
                          current(2,0);
                          current(3,0);
                          current(4,0);
                          current(5,0);
                          current(6,0);
                          end;
 end;
if ch = #67 then proc:=#67;
if proc = #67 then stopcicle(0);
until ch=#27;
ch:=#0;
proc:=#0;
restart;
end;          { ª®­¥æ ¯à®æ¥¤ãàë ¯à®¢¥àª¨ verify }

{----------------------------------------------------------------------------}

procedure zapusk;
begin
textbackground(1);
textcolor(15); gotoxy(22,10);
write('‡ ¯ãáâ¨âì ¯à®æ¥¤ãàã ®â« ¤ª¨ ? ( ');
textcolor(28); write('Y');
textcolor(15); write(' / ');
textcolor(28); write('N');
textcolor(15); write(' )');
if (ch='y') or (ch= #13) then verify;
if (ch='n') or (ch= #27) or timerOff[8] then
   begin
   proc:=#0;
   gotoXY(22,10);
   write('                                       ');
   end;
end;    { zapusk }

{----------------------------------------------------------------------------}

procedure help;
begin
textbackground(0); textcolor(1);
clrscr;           { ®ç¨áâª  íªà ­  ¢ ç¥à­ë© }
write('  ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»');
write('ÉÍ¼                                                                            º');
write('º                                                                              º');
for i:=1 to 20 do
write('º                                                                              º');
write('º                                                                            ÉÍ¼');
write('ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼');
textcolor(2); gotoXY(15,2);
writeln('ˆ­áâàãªæ¨ï ¯® ¯®«ì§®¢ ­¨î ¯à®£à ¬¬®© SPIDER');
textcolor(7);
gotoXY(5,4);  write('1) “áª®à¥­­ë© ¯¥à¥å®¤ ­  á®á¥¤­îî ¯ àã ®áãé¥áâ¢«ï¥âáï ª« ¢¨è ¬¨     ¨   ;');
gotoXY(5,6);  write('2) ˆ§¬¥­¥­¨¥ ¢à¥¬¥­¨ æ¨ª«  § ¯ãáª ¥âáï ª« ¢¨è¥©   , § â¥¬ ¬®¦­®:');
gotoXY(5,7);  write('   ''áâà¥«ª®© ¢¢¥àå'' - ã¢¥«¨ç¨¢ âì ¢à¥¬ï,');
gotoXY(5,8);  write('   ''áâà¥«ª®© ¢­¨§''  - ã¬¥­ìè âì; § ¢¥àè¥­¨¥ ®¯¥à æ¨¨ - ª« ¢¨è  ENTER;');
gotoXY(5,10); write('3) “áâ ­®¢ª  à¥¦¨¬  I  ¯ àë ¢ª«îç ¥âáï ª« ¢¨è¥©   ,');
gotoXY(5,11); write('   ãáâ ­®¢ª  à¥¦¨¬  II ¯ àë ¢ª«îç ¥âáï ª« ¢¨è¥©   ,');
gotoXY(5,12); write('   ¯®á«¥ íâ®£® ¬®¦­® ãáâ ­®¢¨âì:');
gotoXY(5,13); write('   ª« ¢¨è¥© ''áâà¥«ª  ¢¢¥àå'' - ¦¥áâª¨© à¥¦¨¬ ¢ë¡à ­­®© ¯ àë,');
gotoXY(5,14); write('   ª« ¢¨è¥© ''áâà¥«ª  ¢­¨§''  -  ¢â®¬ â¨ç¥áª¨© à¥¦¨¬;');
gotoXY(5,15); write('   ¯®¤â¢¥à¦¤¥­¨¥ ¢ë¡à ­­®£® à¥¦¨¬  - ª« ¢¨è  ENTER;');
gotoXY(5,17); write('4) Š« ¢¨è¥©    ¯à®¨§¢®¤¨âáï ®áâ ­®¢ª  æ¨ª«®£à ¬¬ë,');
gotoXY(5,18); write('   ¢®§¢à â ¢ ­®à¬ «ì­ë© à¥¦¨¬ - ª« ¢¨è  ESC;');
gotoXY(5,20); write('5) Š« ¢¨è¥©     ¯à¨ ¯®¤â¢¥à¦¤¥­¨¨ ª« ¢¨è ¬¨ Y ¨«¨ ENTER § ¯ãáª ¥âáï');
gotoXY(5,21); write('   à¥¦¨¬ €‹€„Šˆ, ¢ ª®â®à®¬ ¬®¦­® ¢àãç­ãî ª®¬¬ãâ¨à®¢ âì ¯®â®ª¨');
gotoXY(5,22); write('   ¯à¨ ¯®¬®é¨ ª« ¢¨è, ®¡®§­ ç¥­­ëå ­  íªà ­¥ ¢ á®®â¢¥âáâ¢ãîé¨å £à ä å.');
textcolor(12);
gotoXY(69,4);  write('Alt');
gotoXY(75,4);  write('F3');
gotoXY(53,6);  write('F5');
gotoXY(53,10); write('F7');
gotoXY(53,11); write('F8');
gotoXY(17,17); write('F9');
gotoXY(17,20); write('F10');
textcolor(3);
gotoXY(68,8);  write('ENTER');
gotoXY(50,15); write('ENTER');
gotoXY(45,18); write('Esc');
gotoXY(49,20); write('Y');
gotoXY(55,20); write('ENTER');
gotoXY(35,24); write('‚®§¢à â - ');
textcolor(27); write('Esc');
repeat
basic(2); gotoXY(5,24); readdate;
until ch = #27;
proc:=#0;
restart;
end;

{----------------------------------------------------------------------------}

procedure jump;
begin
 case number of
     0: alarm(3000,7);
     1:begin if cicle > downborder2 then cicle:=ciclemax-1
                                    else cicle:=trig1-1;
        alarm(1320,25);
       end;
     2:begin if cicle <= div1       then cicle:=div1-1
                                    else cicle:=trig2-1;
        alarm(1320,25);
       end;
 end;
proc:=#0;
end;

{----------------------------------------------------------------------------}

procedure prepare;
var       pauk:         file of char;
          pos:     word;
          multi,tone:        real;
          endp:         boolean;
begin
pos:=0;
res:=0;
tone:=200;
multi:=sqrt(2);
multi:=multi*multi;
endp:=false;
assign(pauk,'xspider.exe');
reset(pauk);
gotoXY(17,10); textcolor(30);
write('®¤®¦¤¨â¥,¯®¦ «ã©áâ , ¨¤¥â ¯®¤£®â®¢ª  ª à ¡®â¥ !');
repeat
endp:=true;
if pos mod 300 = 0 then begin
                         if tone < 2000 then sound(round(tone));
                         tone:=tone*multi;
                         end;
if tone > 9000000 then tone:=200;
for i:=1 to 5 do begin
                 inc(pos);
                 read(pauk,ch);
                 if ch <> p[i] then begin
                                    endp:=false;
                                    break;
                                    end;
                 end;
if (pos-100) mod 300 = 0 then nosound;
until (endp = true) or (pos >50000);
res:=pos;        { +0 }
seek(pauk,res);
read(pauk,ch);
hour:=255-ord(ch);
close(pauk);
gotoXY(17,10);
write('                                                ');
end;

{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}
{----------------------------------------------------------------------------}
{ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ}

begin                   { ***  ç «® ¯à®£à ¬¬ë *** }
nosound;
min:=0;
number:=1;
disk:=false;
port[$378]:=0;
frozen:=false;
hard1:=false;
hard2:=hard1;
hardfix1:=hard1;
hardfix2:=hard2;
hardmode1 :=hard1;
hardmode2 :=hard2;
circuit[1]:=false;
circuit[2]:=false;
ending:=false;
ciclemax:=time;        { 1080 }
ciclemax0:=ciclemax;
cursor_off;
readtime;
cicle:=ciclemax-15;
cictime;
restart;
trigtime1:=div2;
trigtime2:=div3;
trig1:=trigtime1;
trig2:=trigtime2;
prepare;

{---------------------------------------------------------------------------}
{   ************************   á­®¢­®©  ¡«®ª   ************************    }

repeat
 basic(1);     changenumber(number,1);
 opentime;     textcolor(7);
 gotoXY(44,5); prntime(cicle);
 gotoXY(5,5);  prntime(0);         textcolor(3); write(' †'); textcolor(7);
 gotoXY(5,6);  prntime(div2);      textcolor(3); write(' †'); textcolor(7);
 gotoXY(5,7);  prntime(trigtime1); textcolor(3); write(' '); textcolor(7);
 gotoXY(71,5); prntime(div1);      textcolor(3); write(' †'); textcolor(7);
 gotoXY(71,6); prntime(div3);      textcolor(3); write(' †'); textcolor(7);
 gotoXY(71,7); prntime(trigtime2); textcolor(3); write(' '); textcolor(7);
 if bit(port[$379],4)=0 then textcolor(1) else textcolor(5);
 gotoXY(4,5);  write('');
 if bit(port[$379],5)=0 then textcolor(1) else textcolor(5);
 gotoXY(70,5); write('');
 textcolor(7); gotoxy(2,2); readdate;
 case ch of
 #59,#63,#65,#66,#67,#68,#106,#117:  case proc of
 #59,#63,#65,#66,#67,#68,#106,#117: alarm(2000,10);
                                     else proc:=ch;
                                     end;
 end;
 if proc = ch then case ch of
                   #68: timerGo(8,7,0);
                   end;
 case proc of
 #59:  help;
 #63:  changecicle;
 #65:  changemode1;
 #66:  changemode2;
 #67:  stopcicle(1);
 #68:  zapusk;
 #106:  jump;
 #117: ending:=true;
 end;

{----------------------------------------------------------------------------}

 if cicle=upborder1-2   then begin
                             timerGo(7,2,0);
                             border1:='¥à¥¤­ïï £à ­¨æ ';
                             end;
 if cicle=downborder1-2 then begin
                             timerGo(7,2,0);
                             border1:=' ‡ ¤­ïï £à ­¨æ  ';
                             end;
 if cicle=upborder2-2   then begin
                             timerGo(7,2,0);
                             border2:='¥à¥¤­ïï £à ­¨æ ';
                             end;
 if cicle=downborder2-2 then begin
                             timerGo(7,2,0);
                             border2:=' ‡ ¤­ïï £à ­¨æ  ';
                             end;
 if timerOff[7] then begin
                     border1:='                ';
                     border2:='                ';
                     end;
 if timerOn[7] or timerOff[7] then begin
                                   textcolor(23);
                                   gotoXY(13,11); write(border1);
                                   gotoXY(53,11); write(border2);
                                   end;
{----------------------------------------------------------------------------}

 if (cicle < downborder1) or (cicle > trigdown1) then hardmode1:=hardfix1;
 if (cicle < downborder2) or (cicle > trigdown2) then hardmode2:=hardfix2;
 if hardmode1 then trig1:=div2 else trig1:=trigtime1;
 if hardmode2 then trig2:=div3 else trig2:=trigtime2;

{----------------------------------------------------------------------------}

until ending;
textcolor(15);
end. {-------------------- *** Š®­¥æ ¯à®£à ¬¬ë *** -------------------------}
{----------------------------------------------------------------------------}

;浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融
;�             GSORT.EXE's Standard default sort order Profile                �
;�          The profile must be in the same directory as GSORT.EXE            �
;�                      /P activates GSORT.PRO                                �
;�             /p.xxx activates your modified profile GSORT.xxx               �
;藩様様様様様様様様様様様様様様様様様様様曜様様様様様様様様様様様様様様様様様郵
0   r9, 13                       ; 0 - 8  � Comments follow semicolon         �
9   9                            ; 9      � when not in quotes.               �
10  r x'16',13                   ;0A - 20 �                                   �
32 32,'!"#$%&',"'()*+,-./"       ;20 - 2F � Line format:                      �
48 "0123456789:;<=>?"            ;30 - 3F � nn orderlist                      �
64 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;40 - 5A � nn = starting ASCII value         �
91 "(/)!!!"                      ;5B - 60 � orderlist= a list of values       �
97 "abcdefghijklmnopqrstuvwxyz"  ;61 - 7A � to be substituted during          �
123 "(/)!!"                      ;7B - 7F � the sorting process.              �
;      0123456789ABCDEF                   把陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
x'80' "cueaaaa!eeeiiiAA"         ;80 - 8F � Formats                           �
144   "EaFooouuyOU!$!Pf"         ;90 - 9F �  decimal numbers   0, 1,... 255   �
160   "aiounNao!!!!!!!!"         ;A0 - AF �  hex numbers       x'0',...x'FF   �
176  r x'10',"!"                 ;B0 - BF �  characters        'a', "a"       �
192  r 16,'!'                    ;C0 - CF �  strings           "ABC" ,'ABC'   �
208   "!!!!!!!!!!!!!!!!"         ;D0 - DF � - - - - - - - - - - - - - - - - - �
224  "!",'s',r 14,"!"            ;E0 - EF �  Repeating chr     r10,61, r6,'a' �
240   "!!!!!!!!!!!!!!!!"         ;F0 - FF �    r count,chr     rx'A',"A"      �
                                 ;        藩様様様様様様様様様様様様様様様様様�

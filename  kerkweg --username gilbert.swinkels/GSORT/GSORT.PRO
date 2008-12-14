;ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
;บ             GSORT.EXE's Standard default sort order Profile                บ
;บ          The profile must be in the same directory as GSORT.EXE            บ
;บ                      /P activates GSORT.PRO                                บ
;บ             /p.xxx activates your modified profile GSORT.xxx               บ
;ศออออออออออออออออออออออออออออออออออออออออหอออออออออออออออออออออออออออออออออออน
0   r9, 13                       ; 0 - 8  บ Comments follow semicolon         บ
9   9                            ; 9      บ when not in quotes.               บ
10  r x'16',13                   ;0A - 20 บ                                   บ
32 32,'!"#$%&',"'()*+,-./"       ;20 - 2F บ Line format:                      บ
48 "0123456789:;<=>?"            ;30 - 3F บ nn orderlist                      บ
64 "@ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;40 - 5A บ nn = starting ASCII value         บ
91 "(/)!!!"                      ;5B - 60 บ orderlist= a list of values       บ
97 "abcdefghijklmnopqrstuvwxyz"  ;61 - 7A บ to be substituted during          บ
123 "(/)!!"                      ;7B - 7F บ the sorting process.              บ
;      0123456789ABCDEF                   วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
x'80' "cueaaaa!eeeiiiAA"         ;80 - 8F บ Formats                           บ
144   "EaFooouuyOU!$!Pf"         ;90 - 9F บ  decimal numbers   0, 1,... 255   บ
160   "aiounNao!!!!!!!!"         ;A0 - AF บ  hex numbers       x'0',...x'FF   บ
176  r x'10',"!"                 ;B0 - BF บ  characters        'a', "a"       บ
192  r 16,'!'                    ;C0 - CF บ  strings           "ABC" ,'ABC'   บ
208   "!!!!!!!!!!!!!!!!"         ;D0 - DF ว - - - - - - - - - - - - - - - - - ถ
224  "!",'s',r 14,"!"            ;E0 - EF บ  Repeating chr     r10,61, r6,'a' บ
240   "!!!!!!!!!!!!!!!!"         ;F0 - FF บ    r count,chr     rx'A',"A"      บ
                                 ;        ศอออออออออออออออออออออออออออออออออออผ

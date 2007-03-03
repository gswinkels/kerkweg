' Zwevingen
' Natuur en Techniek Aug 92 pp. 634-635
SCREEN 12: CLS
WINDOW (-.1, -6)-(1.1, 6)
t = 0
pi = 4 * ATN(1)
s1 = 30
s2 = 33
DO WHILE t < 1
   y1 = SIN(2 * s1 * pi * t)
   y2 = SIN(2 * s2 * pi * t)
   PSET (t, y1 + y2)
   t = t + .0001
LOOP
a$ = INPUT$(1)
END


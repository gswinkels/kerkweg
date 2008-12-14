OPEN "unsorted.dat" FOR BINARY AS #1
RANDOMIZE TIMER
FOR k = 1 TO 100
s$ = ""
FOR i = 1 TO 40
    j = INT(((90 - 65) + 1) * RND + 65)
    s$ = s$ + CHR$(j)
NEXT i
PUT #1, , s$
NEXT k
END


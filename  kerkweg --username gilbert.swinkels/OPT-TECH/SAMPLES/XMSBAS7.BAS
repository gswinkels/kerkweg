100 REM Sample MS BASIC 7 program that calls OPT-TECH SORT
110 INFILE$ = "SAMP.DAT"
120 OUTFILE$ = "SORTED.DAT"
130 CTLSTMT$ = "S(1,10,C,A)"
140 PRINT "CALLING OPT-TECH SORT..."
145 MEMAVAIL = SETMEM(-100000) 'FREES MEMORY SO THAT SORT CAN ALLOCATE
150 CALL SORTMSB7(INFILE$, OUTFILE$, CTLSTMT$, NUMRECS%, RETCODE%)
155 MEMAVAIL = SETMEM(100000) 'REALLOCATES MEMORY AFTER SORT
160 PRINT "BACK FROM OPT-TECH SORT"
170 PRINT "NUMBER OF RECORDS = ", NUMRECS%
180 PRINT "SORT STATUS       = ", RETCODE%
190 END


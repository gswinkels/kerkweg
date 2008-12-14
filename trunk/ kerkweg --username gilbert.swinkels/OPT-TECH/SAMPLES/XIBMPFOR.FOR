C IBM PROFESSIONAL & RM FORTRAN PROGRAM THAT 
C CALLS OPT-TECH SORT
      
      CHARACTER*15    INFILE,OUTFILE
      CHARACTER*25    CTLSTMT
      INTEGER*2       NUMRECS,RETCODE

      INFILE   = 'SAMP.DAT'
      OUTFILE  = 'SORTED.DAT'
      CTLSTMT  = 'SORT(1,10,C,A)'

      WRITE(*,5000) 

 CALL SORTPF(INFILE,OUTFILE,CTLSTMT,NUMRECS,RETCODE)

      WRITE(*,5001) 
      WRITE(*,5002) NUMRECS
      WRITE(*,5003) RETCODE

      STOP

 5000 FORMAT(1X,'Calling Opt-Tech Sort ...')
 5001 FORMAT(1X,'Back from Opt-Tech Sort')
 5002 FORMAT(1X,'Number of Records =',i4)
 5003 FORMAT(1X,'Sort Status       =',i4)

      END

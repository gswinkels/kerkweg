      PROGRAM   XMSFOR                               

C MICROSOFT FORTRAN PROGRAM THAT CALLS OPT-TECH SORT
      
      CHARACTER*25    INFILE,OUTFILE,CTLSTMT
      INTEGER         NUMRECS,RETCODE

      INFILE   = 'SAMP.DAT/'
      OUTFILE  = 'SORTED.DAT/'
      CTLSTMT  = 'SORT(1,10,C,A)/'
C THE '/' AFTER THE ABOVE PARAMETERS IS REQUIRED

      WRITE(*,5000) 

      CALL SORTF(INFILE,OUTFILE,CTLSTMT,NUMRECS,RETCODE)

      WRITE(*,5001) 
      WRITE(*,5002) NUMRECS
      WRITE(*,5003) RETCODE
      STOP

 5000 FORMAT(1X,'Calling Opt-Tech Sort ...')
 5001 FORMAT(1X,'Back from Opt-Tech Sort')
 5002 FORMAT(1X,'Number of Records ='i4)
 5003 FORMAT(1X,'Sort Status       ='i4)
      END

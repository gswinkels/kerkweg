PROGRAM XTPAS3;
TYPE
  LSTRING = STRING[50];
VAR
   INFILE,
   OUTFILE,
   CTL_STMT: LSTRING;
   RET_CODE,
   NUM_RECS:    INTEGER;

PROCEDURE SORTBTP(VAR INFILE,
                      OUTFILE,
                      CTL_STMT:LSTRING;
                  VAR NUM_RECS,
                      RET_CODE:INTEGER);
                      EXTERNAL 'SORTBTP.COM';

BEGIN (*SAMPBTP - MAIN PROGRAM*)

   INFILE    := 'SAMP.DAT';
   OUTFILE   := 'SORTED.DAT';
   CTL_STMT  := 'S(1,10,C,A)';

   WRITELN('CALLING OPT-TECH SORT...');

   SORTBTP(INFILE,
           OUTFILE,
           CTL_STMT,
           NUM_RECS,
           RET_CODE);

   WRITELN('BACK FROM OPT-TECH SORT');
   WRITELN('NUMBER OF RECORDS =',NUM_RECS);
   WRITELN('SORT STATUS       =',RET_CODE);
END.

PROGRAM XMSPAS(INPUT,OUTPUT);                   
VAR 
  INFILE,
  OUTFILE,
  CTL_STMT,    :LSTRING(30); (*ANY LENGTH IS OK*)
  RET_CODE,
  NUM_RECS     :WORD;
VALUE
  INFILE        := 'SAMP.DAT';
  OUTFILE       := 'SORTED.DAT';
  CTL_STMT      := 'S(1,10,C,A)';

PROCEDURE SORTIBMP (VAR INFILE,
                        OUTFILE, 
                        CTL_STMT:LSTRING; 
                VAR NUM_RECS,
                    RET_CODE:WORD);EXTERNAL;

BEGIN (*XMSPAS - MAIN PROGRAM*)
  WRITELN(OUTPUT,'CALLING OPT-TECH SORT...');

  SORTIBMP(INFILE,                                      
           OUTFILE,
           CTL_STMT,
           NUM_RECS,
           RET_CODE);

  WRITELN(OUTPUT,'BACK FROM SORT');
  WRITELN(OUTPUT,'NUMBER OF RECORDS  =',NUM_RECS);
  WRITELN(OUTPUT,'SORT STATUS        =',RET_CODE);
END. (*XMSPAS*)

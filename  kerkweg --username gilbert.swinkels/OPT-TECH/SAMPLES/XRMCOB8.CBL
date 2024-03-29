 IDENTIFICATION DIVISION.
 PROGRAM-ID. XRMCOB8.
 ENVIRONMENT DIVISION.
 CONFIGURATION SECTION.
 SOURCE-COMPUTER.  IBM-PC.  
 OBJECT-COMPUTER.  IBM-PC.  
******************************************************
 DATA DIVISION.
 WORKING-STORAGE SECTION.
 01  INFILE        PIC X(15) VALUE "SAMP.DAT".
 01  OUTFILE       PIC X(15) VALUE "SORTED.DAT".
 01  CTL-STMT      PIC X(40) VALUE "S(1,10,C,D)".
 01  NUM-RECS      PIC 99999 COMP-1.
 01  RET-CODE      PIC 99999 COMP-1.
 01  NUMBER-RECORDS-DISP PIC 99999.
 01  STATUS-CODE-DISP    PIC 99999.
******************************************************
 PROCEDURE DIVISION.
 MAINLINE.
  DISPLAY "CALLING OPT-TECH SORT...".
  CALL "SORTRM8X.EXE" USING INFILE,       
                            OUTFILE,
                            CTL-STMT,
                            NUM-RECS, 
                            RET-CODE.
  MOVE NUM-RECS TO NUMBER-RECORDS-DISP.
  MOVE RET-CODE TO STATUS-CODE-DISP.
  DISPLAY "NUMBER OF RECORDS =", NUMBER-RECORDS-DISP.
  DISPLAY "STATUS CODE       =", STATUS-CODE-DISP.
  STOP RUN.
 END PROGRAM.

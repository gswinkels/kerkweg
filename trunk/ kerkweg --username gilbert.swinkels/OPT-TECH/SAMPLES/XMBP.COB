       IDENTIFICATION DIVISION.
       PROGRAM-ID. SAMP.
       AUTHOR. DENNIS OLENICK, OPT-TECH DATA PROCESSING.
       ENVIRONMENT DIVISION.
       CONFIGURATION SECTION.
       SOURCE-COMPUTER. IBM-PC.
       OBJECT-COMPUTER. IBM-PC.
      ***************************************************
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  INFILE  PIC X(15) VALUE "SAMP.DAT".
       01  OUTFILE PIC X(15) VALUE "SORTED.DAT".
      *
      * THE CONTROL STATEMENT MUST CONTAIN THE FILETYPE
       01  CTLSTMT PIC X(40) VALUE "S(1,10,C,A)".
       01  NUMRECS COMP-1.
       01  RETCODE COMP-1.
       01  SORT-NUMREC-DISP  PIC 99999.
       01  SORT-STATUS-DISP  PIC 99999.
      ***************************************************
       PROCEDURE DIVISION.
       MAINLINE.
           DISPLAY "CALLING OPT-TECH SORT...".
           CALL "SORTMBP" USING INFILE,
                                OUTFILE,
                                CTLSTMT,
                                NUMRECS,
                                RETCODE.
           MOVE NUMRECS TO SORT-NUMREC-DISP.    
           MOVE RETCODE TO SORT-STATUS-DISP.
           DISPLAY "RETURNED FROM SORT".
           DISPLAY "RECORDS SORTED= ", SORT-NUMREC-DISP.  
           DISPLAY "STATUS CODE=    ", SORT-STATUS-DISP.
           STOP RUN.

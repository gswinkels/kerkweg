       IDENTIFICATION DIVISION.
       PROGRAM-ID.      XSMFCB.
       AUTHOR.          OPT-TECH.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       77  INFILE           PIC X(15)  VALUE "SAMP.DAT/".
       77  OUTFILE          PIC X(15)  VALUE "SORTED.DAT/".
       77  CTL-STMT         PIC X(30)  VALUE "S(1,10,C,A)/".
      *-> THE '/' AFTER THE ABOVE PARAMETERS IS REQUIRED <-
       77  NUM-RECS         PIC 9(4)   COMP-5.
       77  RET-CODE         PIC 9(4)   COMP-5.
       77  SORT-STATUS-DISP PIC 9(5).
       77  NUM-RECS-DISP    PIC 9(5).
       PROCEDURE DIVISION.
       100-MAINLINE.
           DISPLAY "CALLING OPT-TECH SORT..."
           CALL "__SMFCB" USING INFILE, 
                                  OUTFILE,
                                  CTL-STMT, 
                                  NUM-RECS,
                                  RET-CODE.
       DISPLAY "BACK FROM OPT-TECH SORT"
       MOVE NUM-RECS TO NUM-RECS-DISP.
       MOVE RET-CODE TO SORT-STATUS-DISP.
       DISPLAY "NUMBER OF RECORDS = ", NUM-RECS-DISP.
       DISPLAY "SORT STATUS       = ", SORT-STATUS-DISP.
       STOP RUN.

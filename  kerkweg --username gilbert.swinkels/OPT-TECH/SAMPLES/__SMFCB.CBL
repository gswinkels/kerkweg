       Identification Division.
          Program-id. SMCOB4.
       Environment Division.
       Data Division.
         Working-storage section.
       77 INFILE-LEN       PIC 9(4) COMP-5 VALUE 0.
       77 OUTFILE-LEN      PIC 9(4) COMP-5 VALUE 0.
       77 CTL-STMT-LEN      PIC 9(4) COMP-5 VALUE 0.

         Linkage section.
       77  INFILE           PIC X(4095).
       77  OUTFILE          PIC X(4095).
       77  CTL-STMT         PIC X(4095).
       77  NUM-RECS         PIC 9(4)   COMP-5.
       77  RET-CODE         PIC 9(4)   COMP-5.

       Procedure Division using INFILE, 
                                OUTFILE, 
                                CTL-STMT, 
                                NUM-RECS, 
                                RET-CODE.
       Begin-program.
           CALL "__SMFCB" USING INFILE, 
                                  OUTFILE,
                                  CTL-STMT, 
                                  NUM-RECS,
                                  RET-CODE.

       Exit-Program.
            Goback.


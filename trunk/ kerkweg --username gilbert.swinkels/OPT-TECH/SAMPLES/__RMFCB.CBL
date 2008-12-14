       Identification Division.
          Program-id. RSMFCOB.
       Environment Division.
       Data Division.
       Working-storage section.

       Linkage section.
       77  FUNC   PIC 999 COMP-5.
       77  IO-AREA       PIC X(4000).
       77  LEN           PIC 999 COMP-5.
       77  RET-CODE      PIC 999 COMP-5.

       Procedure Division using FUNC, IO-AREA, LEN, RET-CODE.
       Begin-program.
      *     DISPLAY "__RSMFCB, FUNC=",FUNC.
      *     DISPLAY "__RSMFCB, IO-AREA=",IO-AREA.
           CALL '__RMFCB' USING FUNC, IO-AREA, LEN, RET-CODE.
      *     DISPLAY "__RSMFCB", BACK FROM _MAINS".
       Exit-Program.
            Goback.


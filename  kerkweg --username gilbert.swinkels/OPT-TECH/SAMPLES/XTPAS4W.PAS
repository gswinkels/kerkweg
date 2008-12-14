PROGRAM XTPAS4W; (* TURBO PASCAL 4, 5 & 6 SAMPLE *)

(* This is a Turbo Pascal sample program that calls
Opt-Tech Sort (file sort).  This sample allocates
a memory area on the heap and passes its address
and size to Opt-Tech Sort.  It is not necessary to
allocate a memory workarea on the heap.  If the
heap size is limited with the $M directive, Opt-Tech
Sort can allocate it workarea from the area of DOS
memory outside your program.  See sample XTPAS4.PAS *)

USES SORTBTPU;
VAR
   INFILE,
   OUTFILE,
   CTL_STMT: STRING;
   NUM_RECS: INTEGER;
   RET_CODE: INTEGER;
   WORKAREA_PTR:POINTER;
   WORKAREA_SIZE: LONGINT;

(* NUM_RECS CAN BE A LONGINT IF YOU USE
   THE 4BYTE PARAMETER IN YOUR CTL_STMT *)

BEGIN (*XTP4 - MAIN PROGRAM*)

   INFILE         := 'SAMP.DAT';
   OUTFILE        := 'SORTED.DAT';
   CTL_STMT       := 'S(1,10,C,A)';

(* ALLOCATE WORKAREA. A SAMPLE SIZE OF 40000 IS USED HERE.  ANY SIZE
   > 9600 CAN BE USED.  LARGER WORKAREAS ALLOW MORE EFFICIENT SORTING *)
   WORKAREA_SIZE := 40000;
   GetMem(WORKAREA_PTR, 40000);

   WRITELN('CALLING OPT-TECH SORT...');

   SORTBTPW(INFILE,
           OUTFILE,
           CTL_STMT,
           NUM_RECS,
           RET_CODE,
           WORKAREA_PTR,
           WORKAREA_SIZE);

   WRITELN('BACK FROM OPT-TECH SORT');
   WRITELN('NUMBER OF RECORDS =',NUM_RECS);
   WRITELN('SORT STATUS       =',RET_CODE);
END.

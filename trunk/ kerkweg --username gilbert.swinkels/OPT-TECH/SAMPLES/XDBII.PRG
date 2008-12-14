STORE "SAMPDBII.DBF"   TO INFILE
STORE "SORTED.DBF"     TO OUTFILE
STORE "SORT(2,24,C,A) F(DBII)" TO CTLSTMT
DO SORTDBII                                       
? "RETCODE =",RETCODE                                
? "NUMREC  =",NUMREC
? "SAMPDBII.DBF SORTED TO SORTED.DBF"

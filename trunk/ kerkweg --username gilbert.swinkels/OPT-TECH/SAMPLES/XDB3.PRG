? 'Sample dBASE III command file calling Opt-Tech Sort' 
RUN SORT SAMPDB3.DBF SORTDB3.DBF /S(2,9,C,A) F(DB3,M)
RESTORE FROM OPTCOUNT ADDITIVE
RESTORE FROM OPTSTAT  ADDITIVE
? 'Number of records output',OPTCOUNT
? 'Sort status code        ',OPTSTAT

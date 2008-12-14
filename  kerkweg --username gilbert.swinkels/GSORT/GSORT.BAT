copy c:\sort\output.gs c:\sort\input
tm start /L
gsort c:\sort\input c:\sort\input /+6 7
tm stop /L
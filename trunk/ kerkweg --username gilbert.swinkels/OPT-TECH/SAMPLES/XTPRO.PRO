/*  Sample Turbo Prolog program that calls Opt-Tech Sort */

global domains
  Infile, Outfile, Ctl_stmt = string
  Numrec, Retcode = integer
  
global predicates
  sorttp(Infile, Outfile, Ctl_stmt, Numrec, Retcode) - (i,i,i,o,o) language c

goal
  clearwindow,
  Infile = "samp.dat",
  Outfile = "sorted.dat",
  Ctl_stmt = "s(1,10,c,a)",
  sorttp(Infile, Outfile, Ctl_stmt, Numrec, Retcode),
  write("\nCalling Opt-Tech Sort..."),
  write("\nBack from Opt-Tech Sort"),
  write("\nNumber of records = ", Numrec),
  write("\nReturn code = ", Retcode).

main()
{
  /*Sample TURBOC program that call Opt-Tech Sort*/
    char infile[15],    /* any length is ok */
         outfile[15],
         ctl_stmt[50];

    int  num_recs, ret_code;

    strcpy(infile,"SAMP.DAT");
    strcpy(outfile,"SORTED.DAT");
    strcpy(ctl_stmt,"S(1,10,C,A)");

    printf("calling Opt-Tech Sort...\n");

    SORTTCC(infile,
           outfile,
           ctl_stmt,
           &num_recs,
           &ret_code);

    printf("Back from Opt-Tech Sort\n");

    printf("number records=%d\n",num_recs);
    printf("status code   =%d\n",ret_code);
}


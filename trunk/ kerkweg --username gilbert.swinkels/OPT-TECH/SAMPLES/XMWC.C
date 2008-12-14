/*XMWC.C - A MARK WILLIAMS C PROGRAM THAT LINKS TO
 OPT-TECH SORT'S SMALL MS-DOS INTERFACE MODULE. */
main()
{
        char    infile[60],
                outfile[60],
                ctl_stmt[150];
        int     num_recs, ret_code;

        strcpy(infile,"SAMP.DAT");
        strcpy(outfile,"SORTED.DAT");
        strcpy(ctl_stmt,"S(1,10,C,A)");

        printf("Calling Opt-Tech Sort...\n");

        SORTMWS(infile,
                outfile,
                ctl_stmt,
                &num_recs,
                &ret_code); 

    printf("Back from Opt-Tech Sort\n");

    printf("number records=%d\n",num_recs);
    printf("status code   =%d\n",ret_code); 
}

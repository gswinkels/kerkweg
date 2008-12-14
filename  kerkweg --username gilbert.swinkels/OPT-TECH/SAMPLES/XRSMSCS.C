#include "stdio.h"
main()
{
  /* Microsoft C program for calling Opt-Tech Sort with records */

        char io_area[100], *result;
        unsigned int func, io_len, ret_code;
        FILE *f1, *fopen();

        /* set up for initialization call func=1 */
        func = 1;                                        
        strcpy(io_area,"s(1,10,c,a)");  
        io_len = strlen(io_area);
        RSMSCS(func, io_area, &io_len, &ret_code);
        if (ret_code != 0) sorterr(ret_code);

        if((f1 = fopen("samp.dat","r")) == NULL) {
                printf("can not open file \n");
                exit(1);        
        }
        /* pass records to sort func=2 */
        func = 2;
                result = fgets(io_area, 100, f1);
        while(result != NULL) {
                io_len = strlen(io_area);
                RSMSCS(func, io_area, &io_len, &ret_code);
                if (ret_code != 0) sorterr(ret_code);
                result = fgets(io_area, 100, f1);
        }

        /* get sorted records back from sort func=3 */
        func = 3;
        while(ret_code != 1) {
                RSMSCS(func, io_area, &io_len, &ret_code);
                if (ret_code == 1) break;
                if (ret_code > 1) sorterr(ret_code);
                io_area[io_len-1] = '\0';
                printf("%s\n",io_area);
        }
}                

sorterr(code)
        unsigned int code;
{
        printf("sort return code=%d\n",code);
        exit(code);
}


/* ---------------------- csort.h --------------------------- */

#define NOFLDS 5        /* maximum number of fields to sort   */
#define MOSTMEM 50000U  /* most memory for sort buffer        */
#define LEASTMEM 10240  /* least memory for sort buffer       */

struct s_prm {              /* sort parameters                */
    int rc_len;             /* record length                  */
    struct  {
        int f_pos;          /* 1st position of field (rel 1)  */
        int f_len;          /* length of field                */
        char ad;            /* a = ascending; d = descending  */
    } s_fld [NOFLDS];       /* one per field                  */
};

struct bp   {        /* one for each sequence in merge buffer */
    char *rc;        /* -> record in merge buffer             */
    int rbuf;        /* records left in buffer this sequence  */
    int rdsk;        /* records left on disk this sequence    */
};

int init_sort(struct s_prm *prms);  /* Initialize the sort    */
void sort(char *rcd);               /* Pass records to Sort   */
char *sort_op(void);                /* Retrieve sorted records*/
void sort_stats(void);              /* Display sort statistics*/


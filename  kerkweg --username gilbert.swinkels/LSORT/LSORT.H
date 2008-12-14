#define C88 Y
#ifdef MSC
#define LINT_ARGS Y
#define UNSIGNED unsigned
#else
#define UNSIGNED
#endif
#define BIGFILE 0xFFFFFFF
#define MAXFI 16
#define CPMEOF 26
#define MAXSTR 1024
#define MAXLRCL 4096
#define NWAY 9
#define MAXREC 16003
#define SBUFLEN (64000l)
#define MBUFLEN (SBUFLEN)
/* Note MIN_SORTRECS * 4 must be big enough to hold 2*MAXLRCL + 4*MAXSTR */
/*      MIN_SBUFLEN must be big enough to hold NWAY*MAXLRCL+10KB (32KB) */
#define MIN_SORTRECS (MAXLRCL + MAXSTR)
#define MIN_SBUFLEN (MAXLRCL*NWAY+10240)
#define MAXFLDS 32
#define NWAY2P1 (2*NWAY + 1)
#define DBMAXFLD 32
#define DB3MAXFLD 128
#define DBFLDSI  16
#define DB3FLDSI 32
#define DBHEDLEN 521
#define WRITEBIN 1
#define READBIN (O_BINARY + O_RDONLY)
#define strmvc strcpy
#define submvc(to,from,len) _move(len,from,to)
#define length strlen
#define tget gets
#define tput(s) bdosputs(s),bdosputs("\r\n")
#define strclc strcmp
#define subclc strncmp
#define subcmp strncmp
#define subcpy(to,from,len) _move(len,from,to)
#define swrite(f,s) write(f,s,strlen(s)),write(f,"\r\n",2)
#define bswrite(f,s) write(f,s,strlen(s)),write(f,"\r\n",2)
#define sread bsread
#define c80puts puts
#ifdef MSC
#define _move(count,source,dest) memcpy(dest,source,count)
#define _setmem(dest,count,value) memset(dest,value,count)
#endif

#ifndef _MAX_DIR
#include <stdlib.h>
#endif

#ifndef min
#define min(a,b) (a<b ? a :b)
#endif

struct dbfld
{
	UNSIGNED char dbfld_name[11],dbfld_type,dbfld_len,fill_0[2],dbfld_dec;
};
struct dbhed
{
	UNSIGNED char db_id; /*=0x02*/
	int  db_nbr;
	UNSIGNED char db_dte[3];
	int  db_lrecl;
	struct dbfld db_flds[DBMAXFLD]; /*field attributes*/
	UNSIGNED char db_trail; /*should=0*/
};
struct db3fld {
	UNSIGNED char db3fld_name[11];
	UNSIGNED char db3fld_type;
	long db3fld_addr;
	UNSIGNED char db3fld_len;
	UNSIGNED char db3fld_dec;
	char db3fill[14];
};
struct db3hed {
	UNSIGNED char db3_id; /*=0x03 or =x083 (if dbt file) */
	UNSIGNED char db3_dte[3];
	long int db3_nbr;
	int  db3_headrln,db3_lrecl;
	UNSIGNED char db3_headfill[20];
	struct db3fld db3_flds[DB3MAXFLD]; /*field attributes*/
	UNSIGNED char db3_trail; /*should=0d*/
	UNSIGNED char db3_cpmeof; /* set to 26 if needed */
};
struct fieldinfo {
	int (*lcmpsub) () ,fstart,fcond;
	UNSIGNED char flen,ftype;
};
struct sortparm {
	long int rcount[MAXSTR],filesize;
	int lrecl,str_wri,maxlrecl,db3hdrln;
	struct fieldinfo lsdata[MAXFLDS];
	UNSIGNED char dbasebuf[DBHEDLEN],mfile[NWAY2P1][_MAX_DIR],infi[_MAX_DIR],otfi[_MAX_DIR];
	UNSIGNED char ansbuf[128],merg_inn,merg_pass,merg_drive,smi,nflds,nway;
	UNSIGNED char drive1,drive2,mount,phase,harddisk,cpmeof;
} 
;

struct mergebuf {
	char *mbuffer;
	unsigned int mbegin,mend,msize,meof,mchann; 
};

#ifdef LINT_ARGS
char *parsepp(char **,char);
int cscan(char *,char);
int slngcmp(long *,long *,int);
int slogcmp(char *,char *,int);
int shlfcmp(int *,int *,int);
int sfltcmp(float *,float *,int);
int sdblcmp(double *,double *,int);
int sxcmp(char *,char *,int);
int sycmp(char *,char *,int);
int szcmp(char *,char *,int);
int sintcmp(char [],char [],int);
int zonecmp(char [],char [],int);
void oterr(int otchan,char *msg);
int copyfi(struct sortparm *);
int lssort(struct sortparm *);
void sortout(char *[],int,struct sortparm *,int,char *);
int sortin(char *[],int *,char *[],long,int,int,int *,long *,struct sortparm *);
void mergrbuf(int lrecl,struct mergebuf *mbf);
char *mergread(struct mergebuf *,int,char);
int mbfint(struct mergebuf *,int,int,int,
char [NWAY2P1][_MAX_DIR],char *,char **,int,char);
int sortread(int,int,char *);
void mergwrite(int,char *,int,struct mergebuf *);
void mergflush(int,struct mergebuf *);
void mountout(char *drive,long size,char *outfile,char harddisk);
void mwork(char *drvie,long size,char harddisk);
int openfi(char *,int);
int create(char *);
int bsread(int,char *);
void fixinfi(char *);
void getsdat(struct sortparm *,struct db3hed *);
void mhelp(char);
void getfldf(struct sortparm *,struct db3hed *);
int setcmd(struct sortparm *);
int getstart(char *,struct db3hed *,int *,char *,int,int);
int getlrecl();
int gettype();
int sortcmp(char *,char *,struct sortparm *);
char *rtjust(char *,char *,char *,int *,int);
char *dlmfld(char *,int,int *);
int lsfloat(char *,int,char *);
void sortmove(char *tostr,char *frstr,int lrecl);
int userip(char *);
void userop(char *,int);
void lsmerge(struct sortparm *,struct db3hed *);
int sort(char **,int,struct sortparm *);
void qksort(int,int,char **,struct sortparm *);
void hpsort(char **,int,struct sortparm *);
void sift(int,int,struct sortparm *,char **);
void chkdbase(UNSIGNED char *,UNSIGNED char *,struct db3hed *);
int getdbase(struct dbhed *);
int getdb3(struct db3hed *,int,int *);
void setcmp(struct sortparm *);
void srchdbh(struct dbhed *,char *,int *,int *,char *);
void srchdb3(struct db3hed *,char *,int *,int *,char *);
int suprcmp(char *,char *,int);
char *pr(char *);
int prmpt(char *,char *);
#else
char *parsepp();
int prmpt();
int cscan();
char *pr();
int _os();
void oterr();
int copyfi();
int lssort();
void sortout();
int sortin();
int sortip();
int sortic();
int sortia();
void sortoa();
int sortib();
void sortoc();
void sortob();
void mergrbuf();
char *mergread();
int mbfint();
int sortread();
void mergwrite();
void mergflush();
void mountout();
int mwork();
int openfi();
int create();
int bsread();
void fixinfi();
void getsdat();
void mhelp();
void getfldf();
int setcmd();
int getstart();
int getlrecl();
int gettype();
int sortcmp();
char *rtjust();
char *dlmfld();
int lsfloat();
void sortmove();
int userip();
int userop();
void lsmerge();
void merge();
long merge1();
void saveparm();
int mrgorder();
void prtstat();
void setup();
int last_pass();
void merge0();
int sort();
void qksort();
void hpsort();
void sift();
void chkdbase();
int getdbase();
int getdb3();
void setcmp();
void srchdbh();
void srchdb3();
#endif


#ifdef MAIN
char temp[15],*blankline;
long int mtotbuflen;
unsigned max_sortrecs, max_sbuflen;
#else
extern char temp[15],*blankline;
extern long int mtotbuflen;
extern unsigned max_sortrecs, max_sbuflen;
#endif

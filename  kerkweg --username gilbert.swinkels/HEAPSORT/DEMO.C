/*
 *	Use hsort() to sort an array of strings read from input.
 */

#include	<stdio.h>


#define	MAXN	500
#define	MAXSTR	1000


cmp(p1, p2)
char **p1, **p2;
{
	return strcmp(*p1, *p2);
}

static	char	*string[MAXN];
static	char	buf[MAXSTR];

extern	char	*gets();
extern	char	*malloc();


main()
{
	char	*p;
	int	i, n;

	for (n = 0; gets(buf); ++n)
	{
		if (n == MAXN)
		{
			fprintf(stderr, "Too many strings\n");
			exit(1);
		}

		p = malloc(strlen(buf) + 1);
		if (p == (char *)NULL)
		{
			fprintf(stderr, "Out of memory\n");
			exit(2);
		}

		strcpy(string[n] = p, buf);
	}

	hsort(string, n, sizeof string[0], cmp);

	for (i = 0; i < n; ++i)
		puts(string[i]);

	exit(0);
}


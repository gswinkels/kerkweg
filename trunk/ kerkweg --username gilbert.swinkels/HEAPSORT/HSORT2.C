/*
 *	Generic Heapsort, derived from listing 1.
 */


#define	H(k)	(h + k * size)

static
swap(p1, p2, n)			/* swap n bytes */
char *p1, *p2;
unsigned n;
{
	char	tmp;

	while (n-- != 0)
	{
		tmp = *p1; *p1++ = *p2; *p2++ = tmp;
	}
}

static
fixheap(h, size, cmp, i, n)
char *h;
unsigned size, i, n;
int (*cmp)();
{
	unsigned k;

	while ((k = 2 * i) <= n)
	{
		if (k != n && (*cmp)(H(k+1), H(k)) > 0)
			++k;

		if ((*cmp)(H(i), H(k)) >= 0)
			return;

		swap(H(i), H(k), size);
		i = k;
	}
}

hsort(h, n, size, cmp)
char *h;
unsigned n, size;
int (*cmp)();
{
	unsigned i;

	h -= size;

	for (i = n/2; i > 1; --i)
		fixheap(h, size, cmp, i, n);

	while (n > 1)
	{
		fixheap(h, size, cmp, 1, n);
		swap(H(1), H(n), size);
		--n;
	}
}


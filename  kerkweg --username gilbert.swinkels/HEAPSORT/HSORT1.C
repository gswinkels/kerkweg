/*
 *  The Heapsort to sort an array of n integers.
 */

static
fixheap(h, i, n)
int *h;
unsigned i, n;
{
	unsigned k;
	int	 tmp;

	while ((k = 2 * i) <= n)		/* h[k] = left child of h[i] */
	{
		/*  Find maximum of left and right children */

		if (k != n && h[k+1] > h[k])
			++k;			/* right child is greater */

		/* Compare greater of children to parent */

		if (h[i] >= h[k])
			return;

		/* Parent is less than child, so swap */

		tmp = h[k]; h[k] = h[i]; h[i] = tmp;

		i = k;				/* move down tree */
	}
}


hsort(h, n)
int *h;
unsigned n;
{
	unsigned i;
	int	 tmp;

	--h;			/* adjust for zero-origin arrays in C */

	for (i = n/2; i > 1; --i)
		fixheap(h, i, n);	/* build heap, except for h[1] */

	while (n > 1)
	{
		fixheap(h, 1, n);	/* move max to h[1] */
		tmp = h[1];		/* move max to final position */
		h[1] = h[n];
		h[n] = tmp;
		--n;			/* reduce size of heap */
	}
}


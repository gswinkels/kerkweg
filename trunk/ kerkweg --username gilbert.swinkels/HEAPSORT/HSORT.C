/*
 *  Generic Heapsort.
 *
 *  Synopsis:
 *	hsort(char *base, unsigned n, unsigned size, int (*fn)())
 *  Description:
 *	Hsort sorts the array of `n' items which starts at address `base'.
 *	The size of each item is as given.  Items are compared by the function
 *	`fn', which is passed pointers to two items as arguments. The function
 *	should return < 0 if item1 < item2, == 0 if item1 == item2, and > 0
 *	if item1 > item2.
 *  Version:
 *	1988 April 28
 *  Author:
 *	Stephen Russell, Department of Computer Science,
 *	University of Sydney, 2006
 *	Australia.
 */

#ifdef INLINE

#define	swap(p1, p2, n)	{\
	register char *_p1, *_p2;\
	register unsigned _n;\
	register char _tmp;\
\
	for (_p1 = p1, _p2 = p2, _n = n; _n-- > 0; )\
	{\
		_tmp = *_p1; *_p1++ = *_p2; *_p2++ = _tmp;\
	}\
}\

#else

/*
 *   Support routine for swapping elements of the array.
 */

static
swap(p1, p2, n)
register char	 *p1, *p2;
register unsigned n;
{
	register char	ctmp;

	/*
	 *  On machines with no alignment restrictions for int's,
	 *  the following loop may improve performance if moving lots
	 *  of data. It has been commented out for portability.

	 register int	itmp;

	 for ( ; n > sizeof(int); n -= sizeof(int))
	 {
		itmp = *(int *)p1;
		*(int *)p1 = *(int *)p2;
		p1 += sizeof(int);
		*(int *)p2 = itmp;
		p2 += sizeof(int);
	}

	*/

	while (n-- != 0)
	{
		ctmp = *p1; *p1++ = *p2; *p2++ = ctmp;
	}
}

#endif

/*
 *	To avoid function calls in the inner loops, the code responsible for
 *	constructing a heap from (part of) the array has been expanded inline.
 *	It is possible to convert this common code to a function. The three
 *	parameters base0, cmp and size are invariant - only the size of the
 *	gap and the high bound of the array change. In phase 1, gap decreases
 *	while hi is fixed. In phase 2, gap == size, and hi decreases. The
 *	variables p, q, and g are only used in this common code.
 */

hsort(base, n, size, cmp)
char *base;
unsigned n;
unsigned size;
int (*cmp)();
{
	register char	 *p, *q, *base0, *hi;
	register unsigned gap, g;

	if (n < 2)
		return;

	base0 = base - size;		/* set up address of h[0] */

	/*
	 *  The gap is the distance, in bytes, between h[0] and h[i],
	 *  for some i. It is also the distance between h[i] and h[2*i];
	 *  that is, the distance between a node and its left child.
	 *  The initial node of interest is h[n/2] (the rightmost
	 *  interior node), so gap is set accordingly. The following is
	 *  the only multiplication needed.
	 */

	gap = (n >> 1) * size; 		/* initial gap is n/2*size */
	hi = base0 + gap + gap; 	/* calculate address of h[n] */
	if (n & 1)
		hi += size;		/* watch out for odd arrays */

	/*
	 *  Phase 1: Construct heap from random data.
	 *
	 *  For i = n/2 downto 2, ensure h[i] is greater than its
	 *  children h[2*i] and h[2*i+1]. By decreasing 'gap' at each
	 *  iteration, we move down the heap towards h[2]. The final step
	 *  of making h[1] the maximum value is done in the next phase.
	 */

	for ( ; gap != size; gap -= size)
	{
		/*  fixheap(base0, size, cmp, gap, hi) */

		for (p = base0 + (g = gap); (q = p + g) <= hi; p = q)
		{
			g += g;		/* double gap for next level */

			/*
			 *  Find greater of left and right children.
			 */

			if (q != hi && (*cmp)(q + size, q) > 0)
			{
				q += size;	/* choose right child */
				g += size;	/* follow right subtree */
			}

			/*
			 *  Compare with parent.
			 */

			if ((*cmp)(p, q) >= 0)
				break;		/* order is correct */
	
			swap(p, q, size);	/* swap parent and child */
		}
	}

	/*
	 *  Phase 2: Each iteration makes the first item in the
	 *  array the maximum, then swaps it with the last item, which
	 *  is its correct position. The size of the heap is decreased
	 *  each iteration. The gap is always "size", as we are interested
	 *  in the heap starting at h[1].
	 */

	for ( ; hi != base; hi -= size)
	{
		/* fixheap(base0, size, cmp, gap (== size), hi) */

		p = base;		/* == base0 + size */
		for (g = size; (q = p + g) <= hi; p = q)
		{
			g += g;
			if (q != hi && (*cmp)(q + size, q) > 0)
			{
				q += size;
				g += size;
			}

			if ((*cmp)(p, q) >= 0)
				break;
	
			swap(p, q, size);
		}

		swap(base, hi, size);		/* move largest item to end */
	}
}


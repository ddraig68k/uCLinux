
/*
 * This file lifted in toto from 'Dlibs' on the atari ST  (RdeBath)
 *
 * 
 *    Dale Schumacher                         399 Beacon Ave.
 *    (alias: Dalnefre')                      St. Paul, MN  55104
 *    dal@syntel.UUCP                         United States of America
 *  "It's not reality that's important, but how you perceive things."
 */
#include <stdio.h>

static int _bsearch;			/* index of element found, or where to

								   * insert */

char *bsearch(key, base, num, size, cmp)
char *key;				/* item to search for */
char *base;			/* base address */
int num;						/* number of elements */
int size;				/* element size in bytes */
int (*cmp) ();			/* comparison function */
{
	int a, b, c, dir;

	a = 0;
	b = num - 1;
	while (a <= b) {
		c = (a + b) >> 1;		/* == ((a + b) / 2) */
		if ((dir = (*cmp) (key, (base + (c * size))))) {
			if (dir < 0)
				b = c - 1;
			else				/* (dir > 0) */
				a = c + 1;
		} else {
			_bsearch = c;
			return (base + (c * size));
		}
	}
	_bsearch = b;
	return (NULL);
}

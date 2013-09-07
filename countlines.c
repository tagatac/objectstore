#include <stdio.h>

/*
 * Subroutine to count the total number of lines in a file.
 * An open file is passed to us; we do not close it before returning.
 * If an input error is encountered, -1 is returned.
 */

int
countlines(FILE *f)
{
	int numlines;	/* Number of input lines */
	int c;

	numlines = 0;
	while ((c = getc(f)) != EOF)
		if (c == '\n') numlines++;
	if (ferror(f))
		return -1;
	else return numlines;
}

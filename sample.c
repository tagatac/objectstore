#include <stdio.h>

/*
 * This is a program that prints each of its arguments on a separate
 * line, and then counts the number of lines on stdin.
 */

int
main(int argc, char *argv[])
{
	int c,		/* Input character */
	    i,
	    numlines;	/* Number of input lines */
	extern int countlines(FILE *);

	i = 1;
	while (*++argv)
		printf("arg %d: [%s]\n", i++, *argv);
	printf("\n");

	numlines = countlines(stdin);
	if (numlines < 0) printf("error on input\n");
	else printf("%d lines\n", numlines);
	return 0;
}

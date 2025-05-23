#include "basicio.h"
#include "myprintf.h" /* for myprintf, seperate printf function so stdio.h doesn't conflict */
#include <unistd.h>
#include <sys/time.h> /* for execution time measurement */

void filecopy(FILE *ifp, FILE *ofp);

int main(int argc, char **argv)
{
	/* start time */
	struct timeval t0, t1, td;
	gettimeofday(&t0, NULL);
	initstdio(); /* see if there is a way to have this function run before main */
	FILE *ifp, *ofp;
       	ifp = fopen(*++argv, "r");
	ofp = fopen(*++argv, "w");
	filecopy(ifp, ofp);
	fclose(ifp);
	fclose(ofp);
	gettimeofday(&t1, NULL);
	timersub(&t1, &t0, &td);
	myprintf("\nRuntime for fcopy using explicit bit ops: %ds %dµs\n", td.tv_sec, td.tv_usec);
}

/* filecopy: copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while((c = getc(ifp)) != EOF)
		putc(ofp, c);
}

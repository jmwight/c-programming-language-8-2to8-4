#include "basicio.h"
#include <unistd.h>

void filecopy(FILE *ifp, FILE *ofp);

int main(int argc, char **argv)
{
	FILE *ifp, *ofp;
       	ifp = fopen(*++argv, "r");
	ofp = fopen(*++argv, "w");
	filecopy(ifp, ofp);
	fclose(ifp);
	fclose(ofp);
}

/* filecopy: copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while((c = getc(ifp)) != EOF)
		putc(ofp, c);
}

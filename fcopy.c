#include "basicio.h"

int main(int argc, char **argv)
{
	FILE *ifp, *ofp;
       	ifp = fopen(*++argv, "r");
	ofp = foepn(*++argv, "w");
	filecopy(ifp, ofp);
}
/* filecopy: copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
	int c;

	while((c = getc(ifp)) != EOF)
		putc(c, ofp);
}

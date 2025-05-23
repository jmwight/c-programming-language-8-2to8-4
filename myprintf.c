#include <stdio.h>
#include <stdarg.h>

int myprintf(char *fmt, ...)
{
	va_list l;
	int vpret;
	va_start(l, fmt);
	vpret = vprintf(fmt, l);
	va_end(l);
	return vpret;
}

#ifndef BASICIO_H
#define BASICIO_H	1

#define NULL		0
#define	EOF		(-1)
#define	BUFSIZ		1024
#define	OPEN_MAX	20

struct _flags
{
	unsigned int read : 1;
	unsigned int write : 1;
	unsigned int unbuf : 1;
	unsigned int eof : 1;
	unsigned int err : 1;
};

typedef struct _iobuf
{
	int cnt; /* characters left in buffer */
	char *ptr; /* next character position */
	char *base; /* location of buffer */
	struct _flags flag;
	int fd;
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define stderr	(&_iob[2])

extern int _fillbuf(FILE *);
extern int _flushbuf(int, FILE *);

#define feof(p)		(p)->flag.eof
#define ferror(p)	(p)->flag.err
#define fileno(p)	((p)->fd)

#define getc(p)		(--(p)->cnt >= 0 ? \
		(unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(p, x)	(--(p)->cnt >= 0 ? \
		*(p)->ptr++ = (x) : _flushbuf((x), (p)))

#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)

extern void initstdio(void);
extern int fflush(FILE *stream);
extern FILE *fopen(char *name, char *mode);
extern int fclose(FILE *stream);

#endif /* BASICIO_H */

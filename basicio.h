#define NULL		0
#define	EOF		(-1)
#define	BUFSIZ		1024
#define	OPEN_MAX	20

typedef struct _iobuf
{
	int cnt; /* characters left in buffer */
	char *ptr; /* next character position */
	char *base; /* location of buffer */
	int flag;
	int fd;
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define stderr	(&_iob[2])

enum _flags
{
	_READ	= 01,	/* file open for reading */
	_WRITE	= 02,	/* file open for writing */
	_UNBUF	= 04,	/* file is unbuffered */
	_EOF	= 010,	/* EOF has occurred on this file */
	_ERR	= 020	/* error occured on this file */
};

extern int _fillbuf(FILE *);
extern int _flushbuf(int, FILE *);
extern void initstdio(void);
extern int fclose(FILE *);

#define feof(p)		(((p)->flag & _EOF) != 0)
#define ferror(p)	(((p)->flag & _ERR) != 0)
#define fileno(p)	((p)->fd)

#define getc(p)		(--(p)->cnt >= 0 ? \
		(unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(p, x)	(--(p)->cnt >= 0 ? \
		*(p)->ptr++ = (x) : _flushbuf((x), (p)))

#define getchar()	getc(stdin)
#define putchar(x)	putc((x), stdout)

FILE *fopen(char *name, char *mode);

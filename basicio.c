#include <fcntl.h>
#include "basicio.h"
#include "syscalls.h" /* XXX: figure out if I need to create this header or get rid of */
#define PERMS	0666	/* RW for owner, group, others */

//static void _seterror(int r, FILE *fp);

FILE _iob[OPEN_MAX];

/* _fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
	int bufsize;

	if((fp->flag & (_READ|_EOF|_ERR)) != _READ)
		return EOF;
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
	if(fp->base == NULL)		/* no buffer yet */
		if((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOF;	/* can't get buffer */
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if(--fp->cnt < 0)
	{
		if(fp->cnt == -1)
			fp->flag |= _EOF;
		else
			fp->flag |= _ERR;
		fp->cnt = 0;
		return EOF;
	}
	return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c, FILE *fp)
{
	if((fp->flag & (_WRITE|_EOF|_ERR)) != _WRITE)
		return EOF;
	size_t bufsize = fp->flag & _UNBUF ? 1 : BUFSIZ;
	if(fp->base == NULL)
	{
		if((fp->base = malloc(bufsize)) == NULL)
			return EOF;
	}
	else
	{
		/* write buffer (not the extra character (if there is one)) */
		int written;
		written = write(fp->fd, fp->base, bufsize - fp->cnt);
		if(written < 0)
			if(written == -1)
				fp->flag |= _EOF;
			else
				fp->flag |= _ERR;
	}

	/* reset ptr and base */
	fp->cnt = bufsize;
	fp->ptr = fp->base;

	/* add the last character that didn't fit into buffer before  */
	/* XXX: when we write our fclose function we will need to remember to run _flushbuf
	 * twice! This extra character is added to new buffer not written immediately. This will
	 * probably be faster in long run than running two write commands back to back just to get
	 * out that extra character but need to remember this! */
	if(c != EOF)
		*fp->ptr++ = c;

	return (unsigned char) *fp->ptr; /* TODO: figure out something better to return */
}

/* initializes stdin, stdout, and stderr */
void initstdio(void)
{
	/* set the stuff that is the same for each */
	FILE *fp;
	for(fp = _iob; fp < _iob + 3; fp++)
	{
		fp->cnt = 0;
		fp->base = malloc(BUFSIZ);
		fp->ptr = fp->base;
		fp->fd = fp - _iob;
	}

	/* set flags */
	_iob[0].flag = _READ; /* stdin */
	_iob[1].flag = _WRITE; /* stdout */
	_iob[2].flag = _WRITE; /* stderr */
}

/* fopen: open file, return file ptr */
FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;
	for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if((fp->flag & (_READ|_WRITE)) == 0)
			break; /* foudn free slot */
	if(fp >= _iob + OPEN_MAX) /* no free slots */
		return NULL;

	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a')
	{
		if((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	}
	else
		fd = open(name, O_RDONLY, 0);
	if(fd == -1)
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

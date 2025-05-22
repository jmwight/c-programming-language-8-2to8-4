#include <fcntl.h>
#include "basicio.h"
#include "syscalls.h" /* XXX: figure out if I need to create this header or get rid of */
#define PERMS	0666	/* RW for owner, group, others */

static void _seterror(int r, FILE *fp);

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
	if((fp->flag & _WRITE|_EOF|_ERR) != _WRITE)
		return EOF;
	else if(!(fp->flag & _UNBUF))
	{
		int written;
		written = write(fp->fd, fp->base, fp->cnt); /*write the buffer*/
		_seterror(written, fp);
	}

	/* write the last character that didn't fit into buffer */
	if(c != EOF)
	{
		int w = write(fp->fd, &c, 1);
		_seterror(w, fp);
	}

	/* reset the buffer in file */
	fp->cnt = 0;
	fp->ptr = fp->base;
	return EOF;
}

/* _seterror: set error if needed based on what write returns */
static void _seterror(int r, FILE *fp)
{
	/* set error or end of file bits if write had an issue */
	if(r == -1)
		fp->flag |= _EOF;
	else if(r < fp->cnt)
		fp->flag |= _ERR;
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

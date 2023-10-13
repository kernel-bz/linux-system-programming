//
//	file name : ch0403.c
//	comments  : read example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//int file_read (int fd);

//int open (const char *name, int flags);
//int open (const char *name, int flags, mode_t mode);
//ssize_t read (int fd, void *buf, size_t len);
#define SSIZE_MAX 0x7FFFFFFF
#define BUF_SIZE 30

//파일읽기
int file_read (int fd)
{
	int len = 0;
	char buf[BUF_SIZE];
	ssize_t rsize;
	//int len = 0;
	memset (buf, 0, sizeof(buf));
	while ((rsize = read (fd, buf, BUF_SIZE)) != 0) {
		if (rsize == -1) {
			//if (errno == EINTR) continue;
			printf ("read(): error; %d\n", rsize);
			break;
		}
		len += rsize;
		printf ("file_read(): %s(size=%d)\n", buf, rsize);
		memset (buf, 0, sizeof(buf));
	}
	len += rsize;
	printf ("%s", buf);
	return len;
}

int main (int argc, char **argv)
{
	int fd, len; //file descriptor
	char *fname;

	if (argc > 1) fname = argv[1];
	else fname = "./test2.txt";

	fd = open (fname, O_RDONLY);
	if (fd == -1) {
		printf ("open(): error; %d\n", fd);
		return -1;
	}
	len = file_read (fd);
	printf ("\n=========================\n");
	printf ("file read length = %d\n", len);

	if (close (fd) == -1) 
		printf ("close(): error\n");

	return 0;
}

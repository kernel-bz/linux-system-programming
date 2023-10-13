//
//	file name : ch0403_3.c
//	comments  : write, read, sync example
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

char total_buffer[200];

//파일읽기
int file_read (int fd)
{
	int len = 0;
	///char buf[BUF_SIZE];
	char *bp;
	ssize_t rsize;

	bp = total_buffer;

	///memset (buf, 0, sizeof(buf));
	while ((rsize = read (fd, bp, BUF_SIZE)) != 0) {
		//32-bit
		//1KB
		if (rsize == -1) {
			//if (errno == EINTR) continue;
			printf ("read(): error; %d\n", rsize);
			break;
		}
		len += rsize;
		printf ("file_read(): %s(size=%d)\n", bp, rsize);
		///memset (buf, 0, sizeof(buf));
		bp += rsize;
		if (len > sizeof(total_buffer)) break;
	}
	len += rsize;
	printf("Total Result ==========================\n");
	printf ("%s", total_buffer);
	return len;
}

int file_write (int fd)
{
	char buf[BUF_SIZE] = {0,};
	int i, ret, len=0;

	for (i=0; i<10; i++)
	{
		sprintf(buf, "[%d]%s", i, "1234567");
		ret = write(fd, buf, strlen(buf));
		if (ret < 0) {
			printf("error in file_write()\n");
			return -1;
		}
		len += ret;
	}

	ret = fsync(fd);

	printf("writing completed.(size=%d)\n", len);
	return len;
}

int main (int argc, char **argv)
{
	int fd, len; //file descriptor
	char *fname;
	int ret;

	if (argc > 1) fname = argv[1];
	else fname = "./test2.txt";

	///fd = open (fname, O_RDONLY);
	fd = open (fname, O_RDWR | O_CREAT | O_TRUNC);
	if (fd == -1) {
		printf ("open(): error; %d\n", fd);
		return -1;
	}

	len = file_write (fd);

	sync();

	ret = lseek (fd, 0, SEEK_SET);
	if (ret == (off_t) -1) //error
	{
		printf ("lseek(SEEK_SET) error!\n");
	}
	len = file_read (fd);
	printf ("\n=========================\n");
	printf ("file read length = %d\n", len);
	printf ("%s", total_buffer);

	if (close (fd) == -1) printf ("close(): error\n");

	return 0;
}

//
//	file name : ch0404.c
//	comments  : lseek example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main ()
{
	off_t ret;
	long int fd, rn;
	char buf[31] = {0, };

	//I am testing the write(). //25 bytes
	fd = open ("./test2.txt", O_RDONLY);

	ret = lseek (fd, (off_t)6, SEEK_SET);
	if (ret == (off_t) -1) //error
		printf ("lseek(SEEK_SET) error!\n");

	//buf[0] = '\0';
	memset (buf, 0, sizeof(buf));
	rn = read (fd, buf, sizeof(buf));
	if (rn == -1) printf ("(%ld)read error!\n", rn);
	else printf ("reading size(SEEK_SET)=%ld, %ld,%s\n", rn, ret, buf);

	ret = lseek (fd, 0, SEEK_END);
	if (ret == (off_t) -1) //error
		printf ("lseek(SEEK_END) error!\n");

	//buf[0] = '\0';
	memset (buf, 0, sizeof(buf));
	rn = read (fd, buf, sizeof(buf));
	if (rn == -1) printf ("(%ld)read error!\n", rn);
	else printf ("reading size(SEEK_END)=%ld, %ld,%s\n", rn, ret, buf);

	//int pos;
	ret = lseek (fd, -10, SEEK_CUR);
	if (ret == (off_t) -1) //error
		printf ("lseek(SEEK_CUR) error!\n");

	//buf[0] = '\0';
	memset (buf, 0, sizeof(buf));
	rn = read (fd, buf, sizeof(buf));
	if (rn == -1) printf ("(%ld)read error!\n", rn);
	else printf ("reading size(SEEK_CUR)=%ld, %ld, %s\n", rn, ret, buf);

	ret = lseek (fd, (off_t)10, SEEK_END);
	if (ret == (off_t) -1) //error
		printf ("lseek(SEEK_END) error!\n");

	//buf[0] = '\0';
	memset (buf, 0, sizeof(buf));
	rn = read (fd, buf, sizeof(buf));
	if (rn == -1) printf ("(%ld)read error!\n", rn);
	else printf ("reading size(SEEK_END)=%ld, %ld, %s\n", rn, ret, buf);

	return 0;
}

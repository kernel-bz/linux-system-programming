//
//	file name : ch0401.c
//	comments  : open, creat example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main ()
{
	int fd1, fd2;
	fd1 = open ("./test.txt", O_WRONLY | O_CREAT | O_TRUNC,
				S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);

	if (fd1 == -1) printf ("file(tset.txt) open error!\n");
	else printf ("file(test.txt):%d succeded\n", fd1);

	fd2 = creat ("./test2.txt", 0644);
	if (fd2 == -1) printf ("file(tset2.txt) open error!\n");
	else printf ("file(test2.txt):%d succeded!\n", fd2);

	close(fd1);
	close(fd2);

	return 0;
}


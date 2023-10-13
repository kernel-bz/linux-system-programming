//
//	file name : ch0402.c
//	comments  : write example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main ()
{
	int fd1, fd2;
	const char *buf = "I am testing the write().\n";
	ssize_t nr;

	fd1 = open ("./test.txt", O_WRONLY | O_CREAT | O_TRUNC,
			S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);

	if (fd1 == -1) printf ("file(tset.txt) open error!\n");
	else printf ("file(test.txt) %d succeded\n", fd1);

	fd2 = creat ("./test2.txt", 0644);
	if (fd2 == -1) printf ("file(tset2.txt) open error!\n");
	else printf ("file(test2.txt) %d succeded!\n", fd2);

	nr = write (fd2, buf, strlen (buf));
	if (nr == -1) printf ("test2.txt writing error!\n");
	else printf ("test2.txt writing succeed!(%d)\n", nr);

    	close(fd1);
    	close(fd2);
	return 0;
}

//
//	file name : ch0501.c
//	comments  : stream file open example
//	author	  : Jung,JaeJoon(rgbi3307@nate.com) on the www.kernel.bz
//

#include <stdio.h>
#include <limits.h>

FILE* fopen_mode (char *fname, int mode)
{
	FILE	*stream;

	switch (mode) {
		case 0: stream = fopen (fname, "r");	//read
				break;
		case 1: stream = fopen (fname, "r+");	//read
				break;
		case 2: stream = fopen (fname, "w");	//write
				break;
		case 3: stream = fopen (fname, "w+");	//read, write
				break;
		case 4: stream = fopen (fname, "a");	//append(write)
				break;
		case 5: stream = fopen (fname, "a+");	//append(write, read)
				break;
		default:
				stream = fopen (fname, "r+");
	}
	if (!stream) printf ("file(%s) open error!\n", fname);

	return stream;
}

int main (void)
{
	FILE	*stream;
	int		i, c;
	char	s[LINE_MAX];
	char	*data[] = {"apple\n", "bananna\n", "car\n", "dragon\n", "egloo\n", "fox\n"};

	printf ("LINE_MAX = %d\n", LINE_MAX);

	for (i = 0; i < 6; i++) {
		printf ("--%d--\n", i);
		stream = fopen_mode ("ch0501.dat", i);
		if (stream) {
			fputc (*data[i], stream);
			fputs (data[i], stream);

			printf ("positon=%d\n", ftell (stream));
			fseek (stream, 0, SEEK_SET);
			printf ("positon=%d\n", ftell (stream));

			c = fgetc (stream);
			if (!fgets (s, LINE_MAX, stream)) printf ("fgets error!\n");
			printf ("c=%c, s=%s\n", (char)c, s);

			fclose (stream);
		}
	}

	printf ("-------- data -----------\n");
	stream = fopen_mode ("ch0501.dat", 0);
	while (fgets (s, LINE_MAX, stream)) printf ("%s", s);

	fcloseall();	//for linux
	return 0;
}

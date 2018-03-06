//
// ���ϸ�		: /lib/fileio/sound.c
// ���			: ���� ����� ���̺귯��
// ��Ű����	: MIPS
// �ۼ���		: ������(rgbi3307@nate.com)
// ���۱�		: (c) Ŀ�ο���ȸ(http://www.kernel.bz/) ������
// �����ڵ�	: 2009-05-27
// �ֱټ���	: 2009-05-28
// ���ξ�		: snd_
// 

//#include <pthread.h>
#include "sound.h"

//PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP (������ ����� ���)
//pthread_mutex_t a_mutex = PTHREAD_MUTEX_INITIALIZER;


int snd_file_read (char *fname, unsigned char* data, unsigned int size)
{
	int fd;

	if ((fd = open (fname, O_RDONLY)) == -1) {
		printf ("file(%s) open error!\n", fname);
		return -1; //error
	}
	if (read (fd, data, size) == -1) return -1;

	close (fd);
	return 0;
}

int snd_file_write (char *fname, unsigned char* data, unsigned int size)
{
	int fd;

	//creat(fname, 0644)
	if ((fd = open (fname, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
		printf ("file(%s) open error!\n", fname);
		return -1;	//error
	}
	if (write (fd, data, size) == -1) return -1;	

	close (fd);
	return 0;
}

//�־��� ũ�⸸ŭ ����
int snd_file_fread (char *fname, unsigned char* data, unsigned int size)
{
	FILE*	fstream;
	int		c;

	fstream = fopen (fname, "r");
	if (!fstream) {
		printf ("file(%s) fopen error!\n", fname);
		return -1;	//error
	}
	while (size-- > 0 && (c = fgetc (fstream)) != EOF)
        *data++ = c;
	
	fclose (fstream);
	return 0;
}

int snd_file_fwrite (char *fname, unsigned char* data, unsigned int size)
{
	FILE*	fstream;

	//create
	fstream = fopen (fname, "w");
	if (!fstream) {
		printf ("file(%s) fopen error!\n", fname);
		return -1;	//error
	}

	while (size-- > 0 && (fputc (*data++, fstream)) != EOF);

	fclose (fstream);
	return 0;
}

//���� ��ġ ����
int snd_file_open (char* device, int flags, int sam_size, int sam_rate, int channel)
{
	int fd;			//sound device file descriptor
	int status;		//return status of system calls

	//open sound device
	fd = open (device, flags);
	if (fd < 0) {
		printf ("snd_file_open(); sound device open(%s) failed!\n", device);
		return -1;
	}

	//set sampling size
	status = ioctl (fd, SOUND_PCM_WRITE_BITS, &sam_size);
	if (status == -1) {
		printf ("snd_file_open(); SOUND_PCM_WRITE_BITS ioctl failed\n");
		return -1;
	}

	//set channel(mono or stereo)
	status = ioctl (fd, SOUND_PCM_WRITE_CHANNELS, &channel);
	if (status == -1) {
		printf ("snd_file_open(); SOUND_PCM_WRITE_CHANNELS ioctl failed\n");
		return -1;
	}

	//set sampling rate
	status = ioctl (fd, SOUND_PCM_WRITE_RATE, &sam_rate);
	if (status == -1) {
		printf ("snd_file_open(); SOUND_PCM_WRITE_RATE ioctl failed\n");
		return -1;
	}
	return fd;
}

//���� ����
//int snd_file_recording (char *fname, unsigned int length)
void* snd_file_recording (void *data)
{
	char			path_fname[128];
	int				status, second, size, ierr;
	unsigned char*	buf_snd;
	int				fd;	//sound device
	SND_FILE*		snd_file;

	//������� ����ǹǷ� ���ؽ� ���
	//-------------------------------------------------------------------------
	//rc = pthread_mutex_lock(&a_mutex);

	snd_file = (SND_FILE*)data;

	//������ġ ����
	fd = snd_file_open (SND_DEVICE, O_RDWR, SND_SIZE, SND_RATE, SND_CHANNEL);
	if (fd == -1) return NULL;

	//���� ���ϸ�
	sprintf (path_fname, "%s%s", PATH_DATA_VOICE, snd_file->fname);
	ierr = 0;
	
	//���� �Է�(�����Ҵ�)
	//second = length / 5 + 1;
	second = snd_file->length / 5 + 1;
	size = second * SND_SIZE * SND_RATE * SND_CHANNEL / 8;
	buf_snd = malloc (size);

	//���������б�
	//if (snd_file_fread (path_fname, buf_snd, size) == -1) {
		//printf ("sound file(%s) reading error\n", path_fname);		
	//}
	//���� �Է�
	status = read (fd, buf_snd, size);
	if (status != size) {
		printf ("sound input error!\n");
		ierr = -2;
	}

	//���� ���
	status = write (fd, buf_snd, size);
	if (status != size) {
		printf ("sound output error!\n");
		ierr = -3;
	}
		
	//wait for playback to complete before recording again
	status = ioctl (fd, SOUND_PCM_SYNC, 0); 
	if (status == -1) {
		printf ("SOUND_PCM_SYNC ioctl failed\n");
		ierr = -4;
	}

	//������������
	if (snd_file_fwrite (path_fname, buf_snd, size) == -1) {
		printf ("sound file(%s) writing error\n", path_fname);
		ierr = -5;
	}

	fsync (fd);

	free (buf_snd);
	close (fd);

	//���ؽ� ����
	//-------------------------------------------------------------------------
	//rc = pthread_mutex_unlock(&a_mutex);

	if (ierr == 0)
		printf ("snd_file_recording(): %s, size=%d\n", path_fname, size);

	//������ ���ֱ�(���Ͻ� �ڵ������)
    //pthread_exit(NULL);
}

//���� ���(���)
//int snd_file_listening (char *fname, unsigned int length)
void* snd_file_listening (void* data)
{
	char			path_fname[128];
	int				status, second, size, ierr;
	unsigned char*	buf_snd;
	int				fd;	//sound device
	SND_FILE*		snd_file;

	//������� ����ǹǷ� ���ؽ� ���
	//-------------------------------------------------------------------------
	//rc = pthread_mutex_lock(&a_mutex);

	snd_file = (SND_FILE*)data;

	//������ġ ����	
	fd = snd_file_open (SND_DEVICE, O_RDWR, SND_SIZE, SND_RATE, SND_CHANNEL);
	if (fd == -1) return NULL;

	//���� ���ϸ�
	sprintf (path_fname, "%s%s", PATH_DATA_VOICE, snd_file->fname);
	ierr = 0;
	
	//���� �Է�(�����Ҵ�)
	//second = length / 5 + 1;
	second = snd_file->length / 5 + 1;
	size = second * SND_SIZE * SND_RATE * SND_CHANNEL / 8;
	buf_snd = malloc (size);

	//���������б�
	if (snd_file_fread (path_fname, buf_snd, size) == -1) {
		printf ("sound file(%s) reading error\n", path_fname);		
		ierr = -1;
	} else {
		//���� ���
		status = write (fd, buf_snd, size);
		if (status != size) {
			printf ("sound output error!\n");
			ierr = -3;
		}
	}
		
	//wait for playback to complete before recording again
	status = ioctl (fd, SOUND_PCM_SYNC, 0); 
	if (status == -1) {
		printf ("SOUND_PCM_SYNC ioctl failed\n");
		ierr = -4;
	}

	free (buf_snd);
	close (fd);

	//���ؽ� ����
	//-------------------------------------------------------------------------
	//rc = pthread_mutex_unlock(&a_mutex);

	if (ierr == 0)
		printf ("snd_file_listening(): %s, size=%d\n", path_fname, size);

	//������ ���ֱ�(���Ͻ� �ڵ������)
    //pthread_exit(NULL);
}


//���� ��� ���ϱ�(���,�Է��÷���)
int snd_file_listening_say (char *fname, unsigned int length)
{
	char			path_fname[128];
	int				status, second, size, ierr;
	unsigned char*	buf_snd;
	int				fd;	//sound device

	//������ġ ����
	//-------------------------------------------------------------------------
	fd = snd_file_open (SND_DEVICE, O_RDWR, SND_SIZE, SND_RATE, SND_CHANNEL);
	if (fd == -1) return -1;

	//���� ���ϸ�
	sprintf (path_fname, "%s%s", PATH_DATA_VOICE, fname);
	ierr = 0;
	
	//���� �Է�(�����Ҵ�)
	second = length / 5 + 1;
	size = second * SND_SIZE * SND_RATE * SND_CHANNEL / 8;
	buf_snd = malloc (size);

	//���������б�
	if (snd_file_fread (path_fname, buf_snd, size) == -1) {
		printf ("sound file(%s) reading error\n", path_fname);		
	} else {
		//���� ���
		status = write (fd, buf_snd, size);
		if (status != size) {
			printf ("sound output error!\n");
			ierr = -3;
		}
	}

	//wait for playback to complete before recording again
	status = ioctl (fd, SOUND_PCM_SYNC, 0); 
	if (status == -1) {
		printf ("SOUND_PCM_SYNC ioctl failed\n");
		ierr = -4;
	}

	//���� �Է�
	status = read (fd, buf_snd, size);
	if (status != size) {
		printf ("sound input error!\n");
		ierr = -2;
	}

	//���� ���
	status = write (fd, buf_snd, size);
	if (status != size) {
		printf ("sound output error!\n");
		ierr = -3;
	}	

	free (buf_snd);
	close (fd);

	if (ierr < 0) return ierr;
	else return size;
}

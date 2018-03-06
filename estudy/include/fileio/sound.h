//
// ���ϸ�		: /include/fileio/sound.h
// ���			: ���� ����� ���̺귯��
// ��Ű����	: MIPS
// �ۼ���		: ������(rgbi3307@nate.com)
// ���۱�		: (c) Ŀ�ο���ȸ(http://www.kernel.bz/) ������
// �����ڵ�	: 2009-05-28
// �ֱټ���	: 2009-05-28
// 

#ifndef __SOUND_H
#define __SOUND_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>

//sam_size: ���ø� ũ��(8, 16��Ʈ)
//sam_rate: ���ø� ���ļ�(voice:20~8KHz, music:~20KHz)
//channel: ä��(���=1, ���׷���=2)
//����: "/dev/dsp", 8��Ʈ, 8000(8KHz), 1(���)
//����ũ��: �ð�(��) * sam_rate * sam_size * channel / 8

#define PATH_DATA_VOICE "/mnt/usb2/ebook/data/eng/voice/"

#define SND_DEVICE	"/dev/dsp"
#define SND_SIZE			8
#define SND_RATE		 8000
#define	SND_CHANNEL			1

//������������ ����ü
typedef struct
{
	char			fname[20];
	unsigned int	length;
} SND_FILE;

int snd_file_read (char *fname, unsigned char* data, unsigned int size);
int snd_file_write (char *fname, unsigned char* data, unsigned int size);
int snd_file_fread (char *fname, unsigned char* data, unsigned int size);
int snd_file_fwrite (char *fname, unsigned char* data, unsigned int size);

//����ī�� ��ġ ����
//device: ��ġ��
//flags:  O_RDONLY, O_WRONLY, O_RDWR
//sam_size: ���ø� ũ��(8, 16��Ʈ)
//sam_rate: ���ø� ���ļ�(voice:20~8KHz, music:~20KHz)
//channel: ä��(���=1, ���׷���=2)
int snd_file_open (char* device, int flags, int sam_size, int sam_rate, int channel);

//���� ���(���ϸ�, ���ڿ�����)
int snd_file_play (char *fname, unsigned int length);
void* snd_file_listening (void* data);
void* snd_file_recording (void* data);

#endif

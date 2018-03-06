//
// 파일명		: /include/fileio/sound.h
// 기능			: 사운드 입출력 라이브러리
// 아키텍쳐	: MIPS
// 작성자		: 정재준(rgbi3307@nate.com)
// 저작권		: (c) 커널연구회(http://www.kernel.bz/) 정재준
// 최초코딩	: 2009-05-28
// 최근수정	: 2009-05-28
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

//sam_size: 샘플링 크기(8, 16비트)
//sam_rate: 샘플링 주파수(voice:20~8KHz, music:~20KHz)
//channel: 채널(모노=1, 스테레오=2)
//음성: "/dev/dsp", 8비트, 8000(8KHz), 1(모노)
//버퍼크기: 시간(초) * sam_rate * sam_size * channel / 8

#define PATH_DATA_VOICE "/mnt/usb2/ebook/data/eng/voice/"

#define SND_DEVICE	"/dev/dsp"
#define SND_SIZE			8
#define SND_RATE		 8000
#define	SND_CHANNEL			1

//사운드정보파일 구조체
typedef struct
{
	char			fname[20];
	unsigned int	length;
} SND_FILE;

int snd_file_read (char *fname, unsigned char* data, unsigned int size);
int snd_file_write (char *fname, unsigned char* data, unsigned int size);
int snd_file_fread (char *fname, unsigned char* data, unsigned int size);
int snd_file_fwrite (char *fname, unsigned char* data, unsigned int size);

//사운드카드 장치 열기
//device: 장치명
//flags:  O_RDONLY, O_WRONLY, O_RDWR
//sam_size: 샘플링 크기(8, 16비트)
//sam_rate: 샘플링 주파수(voice:20~8KHz, music:~20KHz)
//channel: 채널(모노=1, 스테레오=2)
int snd_file_open (char* device, int flags, int sam_size, int sam_rate, int channel);

//사운드 재생(파일명, 문자열길이)
int snd_file_play (char *fname, unsigned int length);
void* snd_file_listening (void* data);
void* snd_file_recording (void* data);

#endif

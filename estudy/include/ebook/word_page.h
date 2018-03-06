//
// ���ϸ�		: /include/ebook/word_page.h
// ���			: /����å/�ܾ��н� ������
// ��Ű����	: MIPS
// �ۼ���		: ������(http://www.kernel.bz/),(rgbi3307@nate.com)
// �����ڵ�	: 2009-05-14
// �ֱټ���	: 2009-05-19
// 

#ifndef __WORD_PAGE_H
#define __WORD_PAGE_H

#define PATH_DATA "/mnt/usb2/ebook/data/"	//�����Ͱ��

//�н���� ����ü
typedef struct
{
	int	 idx;		
	int	 space;			//���ʿ���
	char title[60];		//�н�����
	char path [40];		//���
	char file [20];		//���������ϸ�
} CHAPTER;


//���������� ����ü
typedef struct
{
	int  idx;
	char query[80];	//����
	char path [40];	//�׸����(������)
	char pic1 [20];	//�׸����ϸ�
	char pic2 [20];
	char pic3 [20];
	char pic4 [20];
	int	 ans;		//�����ȣ
	int  timeout;	//���ѽð�(��)
} WORD1_PAGE;

QUEUE* wp_page_create (char* fname);
void wp_page_print (QUEUE* pQueue);


#endif

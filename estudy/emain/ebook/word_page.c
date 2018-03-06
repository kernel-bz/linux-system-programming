//
// ���ϸ�		: /ebook/word_page.c
// ���			: /����å/�ܾ��н� ������
// ��Ű����	: MIPS
// �ۼ���		: ������(http://www.kernel.bz/),(rgbi3307@nate.com)
// �����ڵ�	: 2009-05-14
// �ֱټ���	: 2009-05-21
// ���ξ�		: wp_
// 

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "list.h"
#include "word_page.h"

//�н� ���������� �б�
int _fread_word_page (char* fname, QUEUE* queue)
{
	FILE 	*fstream;
	char 	linebuf[128], data[128];
	int		tag_idx = 0, cnt = 0, is;
	char	*tag[] = {"query=", "path=", "pic1=", "pic2=", "pic3=", "pic4=", "ans=", "timeout="};
	WORD1_PAGE* wpage;

	if (access(fname, F_OK)) {
		printf ("%s not found!!\n", fname);
		return -1;
	}
	
	fstream = fopen (fname ,"r");
	if (!fstream) {
		printf ("%s file open error!!\n", fname);
		return -1;
	}

	//�޸� �Ҵ�
	wpage = (WORD1_PAGE*) malloc(sizeof(WORD1_PAGE));
	if (!(wpage)) {
		printf("wp_page_create(): \aOut of Memory\n");
		return -1;
	}
	while (!feof(fstream))
	{
		fgets (linebuf, sizeof(linebuf), fstream);
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //�ּ�

		//sscanf (linebuf, "%s", data);
		//strncpy(data, linebuf, strlen(linebuf)-1);
		strcpy(data, linebuf);
		if (strlen(data) < 3) continue;
		data[strlen(data)-2] = '\0'; //���ڿ� ���� ���๮�� ����

		
		if (data[0] == '#') {
			is = sizeof(tag) / sizeof(*tag) - 2;	//�迭����-2
			if (tag_idx > is) {
				cnt++;
				wpage->idx = cnt;
				//ť�� ���
				enqueue (queue, wpage);
				//�޸� �Ҵ�
				wpage = (WORD1_PAGE*) malloc(sizeof(WORD1_PAGE));
				if (!(wpage)) {
					printf("wp_page_create(): \aOut of Memory\n");
					return -1;
				}
			}
			tag_idx = -1;
			continue;
		}
		tag_idx++;
		is = strlen(tag[tag_idx]);
		
		if (tag_idx == 0) sprintf (wpage->query, "%s", &data[is]);
		else if (tag_idx == 1) sprintf (wpage->path, "%s", &data[is]);
		else if (tag_idx == 2) sprintf (wpage->pic1, "%s", &data[is]);
		else if (tag_idx == 3) sprintf (wpage->pic2, "%s", &data[is]);
		else if (tag_idx == 4) sprintf (wpage->pic3, "%s", &data[is]);
		else if (tag_idx == 5) sprintf (wpage->pic4, "%s", &data[is]);
		else if (tag_idx == 6) wpage->ans = atoi(&data[is]);
		else if (tag_idx == 7) wpage->timeout = atoi(&data[is]);
	}

	fclose (fstream);
	return 0;
}

//�ӽ�(������)
int _fread_chapter (char* fname)
{
	FILE 	*fstream;
	unsigned char linebuf[80];
	unsigned char data[80];
	int		icnt = 0;

	if (access(fname, F_OK)) {
		printf ("%s not found!!\n", fname);
		return -1;
	}
	
	fstream = fopen (fname ,"r");
	if (!fstream) {
		printf ("%s file open error!!\n", fname);
		return -1;
	}

	while (!feof(fstream))
	{
		fgets (linebuf, sizeof(linebuf), fstream);
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //�ּ�

		strcpy (data, linebuf);
		data[strlen(data)-2] = '\0'; //���ڿ� ���� ���๮�� ����
		//printf ("read_chapter=%s\n", data);

		icnt++;
	}
	fclose (fstream);
	return icnt;
}

//ť�� �����ϰ�, ����ü �޸� �Ҵ�
QUEUE* wp_page_create (char* fname)
{
	QUEUE*	queue;

	queue = createQueue ();	

	//fname ���Ͽ��� �н��ܾ� �������� �о ť�� ����
	_fread_word_page (fname, queue);

	return queue;
}

//ť�� ��� �ִ� ������ ���
void wp_page_print (QUEUE* pQueue)
{
	WORD1_PAGE* wpage;

	while (!emptyQueue (pQueue))
	{
		dequeue (pQueue, (void**)&wpage);
		printf("%d:%s:%d\n", wpage->idx, wpage->query, wpage->ans);
	}
	printf("\n");

	return;
}

//����Ʈ �ȿ��� ���Ǵ� �ε��� ���Լ�
//-----------------------------------------------------------------------------
int wp_chapter_cmp_index (void* ptr1, void* ptr2)
{
	return (((CHAPTER*)ptr1)->idx - ((CHAPTER*)ptr2)->idx);
}

//����Ʈ�� ��������, ����(fname)���� �н� ����� �Ҵ�
//fgets()�� ����Ͽ� ������ ����
//-----------------------------------------------------------------------------
LIST* wp_chapter_create (char *fname)
{
	CHAPTER*		Chapter;
	LIST*			list;
	FILE*			fstream;
	char			path_fname[128];
	char			linebuf[128], data[128];
	int				tag_idx = 0, cnt = 0, is, addResult;
	char			*tag[] = {"space=", "title=", "path=", "file="};

	list = createList (wp_chapter_cmp_index);
	if (!list) {
		printf ("\aCannot create list\n");
		return NULL;
	}	

	sprintf (path_fname, "%s%s", PATH_DATA, fname);
	if (access(path_fname, F_OK)) {
		printf ("%s not found!!\n", path_fname);
		return NULL;
	}

	fstream = fopen (path_fname, "r");
	if (!fstream) {
		printf ("\aError opening input file\n");
		return NULL;
	}

	//�޸��Ҵ�
	Chapter = (CHAPTER*) malloc(sizeof(CHAPTER));
	if (!Chapter) {
		printf ("\aOut of Memory in build list\n");
		return NULL;
	}

	while (!feof(fstream))
	{
		fgets (linebuf, sizeof(linebuf), fstream);
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //�ּ�

		//sscanf (linebuf, "%s", data);
		//strncpy(data, linebuf, strlen(linebuf)-1);
		strcpy(data, linebuf);
		if (strlen(data) < 3) continue;
		data[strlen(data)-2] = '\0'; //���ڿ� ���� ���๮�� ����
		
		if (data[0] == '#') {
			if (tag_idx > 0) {
				cnt++;
				Chapter->idx = cnt;
				//����Ʈ�� ���
				addResult = addNode (list, Chapter);
				if (addResult == 0) {
					//�޸��Ҵ�
					Chapter = (CHAPTER*) malloc(sizeof(CHAPTER));
					if (!Chapter) {
						printf ("\aOut of Memory in build list\n");
						return NULL;
					}
				} else {
					if (addResult == -1) {
						printf ("Memory overflow adding index(%d)\n", Chapter->idx); 
						return NULL;
					} else
						printf ("Duplicate index(%d)\n", Chapter->idx);
				}
			}
			tag_idx = -1;
			continue;
		}
		tag_idx++;
		is = strlen(tag[tag_idx]);

		//printf ("wp_chapter_create():%s\n", &data[is]);

		if (tag_idx == 0) Chapter->space = atoi(&data[is]);
		else if (tag_idx == 1) sprintf (Chapter->title, "%s", &data[is]);
		else if (tag_idx == 2) sprintf (Chapter->path, "%s", &data[is]);
		else if (tag_idx == 3) sprintf (Chapter->file, "%s", &data[is]);

	} //while

	fclose (fstream);
	return list;
}

//����Ʈ�� ��������, ����(fname)���� �н� ����� �Ҵ�
//fgetc()�� ����Ͽ� ������ ����
//fgetc�� ������ ȣ���ϴ� function call overhead�� ���� �� ����.
//������
//-----------------------------------------------------------------------------
LIST* wp_chapter_create2 (char *fname)
{
	char			path[128];
	CHAPTER*		Chapter;
	LIST*			list;
	FILE*			fstream;
	unsigned char	linebuf[80];
	unsigned char	idx[10], data[80];
	int				c, i, addResult;


	list = createList (wp_chapter_cmp_index);
	if (!list) {
		printf ("\aCannot create list\n");
		return NULL;
	}
	sprintf (path, "%s%s", PATH_DATA, fname);
	if (access(path, F_OK)) {
		printf ("%s not found!!\n", fname);
		return NULL;
	}

	fstream = fopen (path, "r");
	if (!fstream) {
		printf ("\aError opening input file\n");
		return NULL;
	}

	while (1)
	{
		i = 0;
		linebuf[i++] = fgetc (fstream);
		if (linebuf[i-1] == '/') {
			linebuf[i++] = fgetc (fstream);
			if (linebuf[i-1] == '/') {
				fgets (linebuf, sizeof(linebuf), fstream);
				continue; //�ּ��� ����
			}
		}

		if (linebuf[i-1] != '\t') {
			while ( (c=fgetc(fstream)) != EOF && ((linebuf[i++] = c) != '\t') );
		}

		if (c == EOF) break;
		strncpy (idx, &linebuf[0], i);
		idx[i] = '\0';
		//printf ("idx=%s\n", idx);

		i = 0;
		while ( (c=fgetc(fstream)) != EOF && ((linebuf[i++] = c) != '\r') );

		if (c == EOF) break;
		strncpy (data, &linebuf[0], i);
		data[i] = '\0';
		//printf ("data=%s\n", data);

		Chapter = (CHAPTER*) malloc(sizeof(CHAPTER));
		if (!Chapter) {
			printf ("\aOut of Memory in build list\n");
			return NULL;
		}		
		Chapter->idx = atoi(idx);
		//Chapter->title = data;
		sprintf (Chapter->title, "%s", data);

		//����Ʈ�� �߰�
		addResult = addNode (list, Chapter);
		if (addResult != 0) {
			if (addResult == -1) {
				printf ("Memory overflow adding index(%d)\n", Chapter->idx); 
				return NULL;
			} else
				printf ("Duplicate index(%d)\n", Chapter->idx);
		}
	}

	fclose (fstream);
	return list;
}




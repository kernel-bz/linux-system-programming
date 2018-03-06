//
// 파일명		: /ebook/word_page.c
// 기능			: /전자책/단어학습 페이지
// 아키텍쳐	: MIPS
// 작성자		: 정재준(http://www.kernel.bz/),(rgbi3307@nate.com)
// 최초코딩	: 2009-05-14
// 최근수정	: 2009-05-21
// 접두어		: wp_
// 

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "list.h"
#include "word_page.h"

//학습 데이터파일 읽기
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

	//메모리 할당
	wpage = (WORD1_PAGE*) malloc(sizeof(WORD1_PAGE));
	if (!(wpage)) {
		printf("wp_page_create(): \aOut of Memory\n");
		return -1;
	}
	while (!feof(fstream))
	{
		fgets (linebuf, sizeof(linebuf), fstream);
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //주석

		//sscanf (linebuf, "%s", data);
		//strncpy(data, linebuf, strlen(linebuf)-1);
		strcpy(data, linebuf);
		if (strlen(data) < 3) continue;
		data[strlen(data)-2] = '\0'; //문자열 끝에 개행문자 제거

		
		if (data[0] == '#') {
			is = sizeof(tag) / sizeof(*tag) - 2;	//배열개수-2
			if (tag_idx > is) {
				cnt++;
				wpage->idx = cnt;
				//큐에 등록
				enqueue (queue, wpage);
				//메모리 할당
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

//임시(사용않함)
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
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //주석

		strcpy (data, linebuf);
		data[strlen(data)-2] = '\0'; //문자열 끝에 개행문자 제거
		//printf ("read_chapter=%s\n", data);

		icnt++;
	}
	fclose (fstream);
	return icnt;
}

//큐를 생성하고, 구조체 메모리 할당
QUEUE* wp_page_create (char* fname)
{
	QUEUE*	queue;

	queue = createQueue ();	

	//fname 파일에서 학습단어 페이지를 읽어서 큐에 저장
	_fread_word_page (fname, queue);

	return queue;
}

//큐에 들어 있는 내용을 출력
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

//리스트 안에서 사용되는 인덱스 비교함수
//-----------------------------------------------------------------------------
int wp_chapter_cmp_index (void* ptr1, void* ptr2)
{
	return (((CHAPTER*)ptr1)->idx - ((CHAPTER*)ptr2)->idx);
}

//리스트를 생성한후, 파일(fname)에서 학습 목록을 할당
//fgets()를 사용하여 파일을 읽음
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

	//메모리할당
	Chapter = (CHAPTER*) malloc(sizeof(CHAPTER));
	if (!Chapter) {
		printf ("\aOut of Memory in build list\n");
		return NULL;
	}

	while (!feof(fstream))
	{
		fgets (linebuf, sizeof(linebuf), fstream);
		if ((linebuf[0] == '/') && (linebuf[1] == '/')) continue; //주석

		//sscanf (linebuf, "%s", data);
		//strncpy(data, linebuf, strlen(linebuf)-1);
		strcpy(data, linebuf);
		if (strlen(data) < 3) continue;
		data[strlen(data)-2] = '\0'; //문자열 끝에 개행문자 제거
		
		if (data[0] == '#') {
			if (tag_idx > 0) {
				cnt++;
				Chapter->idx = cnt;
				//리스트에 등록
				addResult = addNode (list, Chapter);
				if (addResult == 0) {
					//메모리할당
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

//리스트를 생성한후, 파일(fname)에서 학습 목록을 할당
//fgetc()를 사용하여 파일을 읽음
//fgetc를 여러번 호출하는 function call overhead가 있을 수 있음.
//사용않함
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
				continue; //주석문 무시
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

		//리스트에 추가
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




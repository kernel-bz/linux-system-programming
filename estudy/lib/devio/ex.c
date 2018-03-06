//------------------------------------------------------------------------------
// �� �� �� : ex.c
// ������Ʈ : fwp
// ��    �� :
// �� �� �� : ����â, �̿���, �����
// �� �� �� : 2007�� 5�� 25��
// �� �� �� :
// �� �� �� :
// ��    �� :
// ���̼��� : GPL V2
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <sys/poll.h>

#define _EX_VAR_ 1
#include "ex.h"
//--------------------------------------------------------------
// ���� ���� 
//--------------------------------------------------------------
static 	ex_mng_t		ex_mng_list[EX_POLL_LIST_MAX];				// ���� �̺�Ʈ ����Ʈ
static 	int 			ex_mng_count	= 0;						// �� ����ü ���� ���� ��ũ���� ����

static 	pollfd_t		ex_poll_list[EX_POLL_LIST_MAX];				// �̺�Ʈ ��� ó�� �� ����ü
static  int				ex_poll_index[EX_POLL_LIST_MAX];            // �̺�Ʈ�� ����ü�� ���� ���ؽ��� �����ϱ� ���� ����
static 	int 			ex_poll_count	= 0;						// �� ����ü ���� ���� ��ũ���� ����
static 	int 			ex_adjust_poll_req	= 0;					// ex_adjust_poll_implement()�� ȣ�⿩�� Ȯ�� flag

//--------------------------------------------------------------
// �Լ�
//--------------------------------------------------------------
static 	int 			ex_wait_time			= EX_WAIT_TIME_DEFAULT;		// �ƹ��� ����� ������ �Լ��� Ż���� Ÿ�� �ƿ�
static 	int				ex_error_code			= EXERR_NONE;				// �����ڵ�

		int 			(*ex_on_poll_timeout)	(void)				= NULL;
		int				(*ex_on_hooking_loop)	(int poll_state)	= NULL;
		
static	int				ex_adjust_poll_implement(void);						// poll ó���� �����ϵ��� ����ȭ �Ѵ�. 
//--------------------------------------------------------------
// ���� : event ���̺귯���� �ʱ�ȭ�Ѵ�.
// �Ű� : wait_time poll ��� �ð� ����, ����[ms], 0�̻��� ��
// ��ȯ : ����0, ����-1
//--------------------------------------------------------------
int ex_init( int wait_time )
{
	int lp;
	if(wait_time<0)
	{
		ex_error_code = EXERR_WAIT_TIME;
		return -1;
	}

	ex_wait_time = wait_time;

	memset(ex_mng_list,0,sizeof(ex_mng_list));
	for( lp = 0; lp < EX_POLL_LIST_MAX; lp++ )
	{
		ex_mng_list[lp].fd = -1;										// �̺�Ʈ ��� ó�� �� ����ü
	}
	ex_mng_count = 0;										// �� ����ü ���� ���� ��ũ���� ����

	memset(ex_poll_list,0,sizeof(ex_poll_list));
	for( lp = 0; lp < EX_POLL_LIST_MAX; lp++ )
	{
		ex_poll_list[lp].fd = -1;										// �̺�Ʈ ��� ó�� �� ����ü
	}
	ex_poll_count = 0;										// �� ����ü ���� ���� ��ũ���� ����

	return 0;
}

//--------------------------------------------------------------
// ���� : ex_error_code�� ���� ���� ���� ���ڿ��� ��ȯ
//--------------------------------------------------------------
static char  *ex_error_string( void)
{
	char *error_string[] ={ "���� ����",                                    		//EXERR_NONE
							"ex_init�Լ��� ���ð� �� ����",               		//EXERR_WAIT_TIME
							"poll �Լ� ����"	     								//EXERR_POLL_FUNC
							"ex_add_fd�Լ��� ����� fd ���� ",						//EXERR_POLL_ADD_FD
							"ex_add_fd�Լ��� ����� �޸� ���� "					//EXERR_POLL_ADD_MEM
                        };
   return( error_string[ex_error_code]);
}

//--------------------------------------------------------------
// ���� : exlib�� �����ڵ�, �������ڿ� �� ����� �޼��� ���
// �Ű� : ����� �޼���
//--------------------------------------------------------------
void ex_print_error( char *msg )
{
	printf( "\r[gx error:%d]%s %s\n", ex_error_code, ex_error_string(), msg);
	//return gx_error_code;
}

//--------------------------------------------------------------
// ���� :
//--------------------------------------------------------------
int ex_loop( void )
{
	int 			poll_state;
	int				mng_index;
	int 			lp;
	struct pollfd	*ex_poll_item;
	ex_mng_t		*ex_mng_item;



	if( ex_adjust_poll_req )	
	{
		ex_adjust_poll_implement();			// poll ó���� �����ϵ��� ����ȭ �Ѵ�. 
		ex_adjust_poll_req = 0;				// flag �ʱ�ȭ 
	}		

	// poll()�� ȣ���Ͽ� event �߻� ���� Ȯ��(int�� ��ȯ)
	poll_state = poll( ex_poll_list,   		// event ��� ����(struct pollfd *)
                        ex_poll_count,    	// üũ�� pollfd ����(int)
                        ex_wait_time   		// time out �ð�(int)
                       );

	if( poll_state < 0 )
	{
		ex_error_code = EXERR_POLL_FUNC;
		return -1;
	}

	if( ex_on_hooking_loop != NULL )
	{
		if(ex_on_hooking_loop(poll_state))	return 0;
	}

	if( poll_state == 0)
	{
		if( ex_on_poll_timeout != NULL) ex_on_poll_timeout();
	}

	for( lp = 0; lp < ex_poll_count; lp++ )
	{
		ex_poll_item	= &ex_poll_list[lp];
		mng_index		= ex_poll_index[lp];
		ex_mng_item		= &ex_mng_list[mng_index];

		if( ex_poll_item->revents & POLLIN 	&& ex_mng_item->on_read  != NULL ) ex_mng_item->on_read ( ex_mng_item );
		if( ex_poll_item->revents & POLLOUT && ex_mng_item->on_write != NULL ) ex_mng_item->on_write( ex_mng_item );
		//if( ex_poll_item.revents & POLLERR 	&& ex_event_t->on_poll_err != NULL ) on_poll_err( );
		//if( ex_poll_item.revents & POLLHUP 	&& ex_event_t->on_poll_hup != NULL ) on_poll_hup( );
	}
	return 0;
}


//--------------------------------------------------------------
// ���� : fd�� ������� ���Խ�Ű��, fd�� �����ϴ� ex_mng_t ����ü�� ��ȯ�Ѵ�.
// ��ȯ : NULL�̸� ����, �� �� ex_mng_t ����ü �ּ�
// ���� : ���� fd�� ��ϺҰ�
// ���� : �Լ�ȣ�� �� ex_adjust_poll�Լ� ȣ���� �ʿ�.
//--------------------------------------------------------------
ex_mng_t * ex_add_fd( int fd )
{
	int lp;

	if(fd <0)
	{
		ex_error_code = EXERR_POLL_ADD_FD;
		return NULL;
	}

	for(lp=0;lp<ex_mng_count;lp++)
	{
		if(ex_mng_list[lp].fd == fd)
		{
			return &ex_mng_list[lp];
		}
	}

	if(ex_mng_count >=EX_POLL_LIST_MAX)
	{
		ex_error_code = EXERR_POLL_ADD_MEM;
		return NULL;
	}

	ex_mng_list[ex_mng_count].fd = fd;
	return &ex_mng_list[ex_mng_count++];
}

//--------------------------------------------------------------
// ���� : fd�� ������� ������Ű��
// ���� : ex_del_fd()�� ������ �� ex_adjust_poll()�� �ݵ�� ���־��.
//--------------------------------------------------------------
void	ex_del_fd(int fd)
{
	int find_lp, del_lp;

	if(ex_mng_count <1)		return;

	for(find_lp=0; find_lp < ex_mng_count ;find_lp++)
	{
		if(ex_mng_list[find_lp].fd ==fd)
		{
			// �����Ѵ�.
			for(del_lp =find_lp ; del_lp < ex_mng_count-1 ;del_lp++ )
			{
				ex_mng_list[del_lp]= ex_mng_list[del_lp+1];
			}
			ex_mng_count--;		

			// �� ������ ���� clear�Ѵ�.
			ex_mng_list[del_lp].fd 			=-1;
			ex_mng_list[del_lp].priv 		= NULL;
			ex_mng_list[del_lp].on_read		= NULL;
			ex_mng_list[del_lp].on_write	= NULL;
		}
	}

}

//--------------------------------------------------------------
// ���� : poll ó���� �����ϵ��� ����ȭ �ϰ� �÷��׸� �����. 
//       :ex_loop() POLL �ϱ� ���� �ʿ��ϴ�.
//--------------------------------------------------------------
int ex_adjust_poll( void)
{	
	ex_adjust_poll_req = 1;	
	return 0;	
}

//--------------------------------------------------------------
// ���� : poll ó���� �����ϵ��� ����ȭ �Ѵ�. 
//       :ex_loop() POLL �ϱ� ���� �ʿ��ϴ�. 
//--------------------------------------------------------------
static int ex_adjust_poll_implement( void)
{
	int lp;
	short events;
	
	ex_poll_count = 0;								// �ʱ�ȭ 

	for(lp=0;lp<ex_mng_count;lp++)
	{
		if(ex_mng_list[lp].fd < 0) continue;

		events = 0;

		if(ex_mng_list[lp].on_read  !=NULL) events |= POLLIN;			//  read �Լ��� ��ϵ� ������ POLLIN�� ���
		if(ex_mng_list[lp].on_write !=NULL) events |= POLLOUT;			//  on_write �Լ��� ��ϵ� ������ POLLIN�� ���	

		if(events == 0) continue;

		ex_poll_list[ex_poll_count].fd      = ex_mng_list[lp].fd ;
		ex_poll_list[ex_poll_count].events  = events;
		ex_poll_list[ex_poll_count].revents = 0;

		ex_poll_index[ex_poll_count] 		= lp;

		ex_poll_count++;
	}

	return 0;
}

//------------------------------------------------------------------------------
// �� �� �� : ix.h
// ������Ʈ : fwp
// ��    �� :
// �� �� �� : ����â, �̿���, ����� , �̿���,
// �� �� �� : 2009�� 1�� 17��
// �� �� �� :
// �� �� �� :
// ��    �� :
// ���̼��� : GPL V2
//------------------------------------------------------------------------------

#ifndef _IX_HEADER_
#define _IX_HEADER_


#define	FILE_NAME_INPUT_INFO_BASE		"/proc/bus/input/devices"					// ����̽� ������ġ
#define	IX_TMP_PATH_DEFAULT				"/tmp"										// mknod�� �ϱ����� �ӽ� ����

#define FLAG_SET						1
#define FLAG_RELEASE					0

//#define FILE_TOUCH_CAL_INFO				"/etc/ixlib/ts_calibration_info.db"			// ��ġ ��ũ�� Į���� ���̼� ������ 
#define FILE_TOUCH_CAL_INFO				"./ts_calibration_info.db"			// ��ġ ��ũ�� Į���� ���̼� ������ 

#define IX_KEY_PRESS_MAX		54			// KEY PRESS �� ó���� ������ ���� �ִ밪 
// �ƽ�Ű ���̺� ǥ 
#define	 ASKII_RESERVED 		0x0			// 
#define	 ASKII_TAP				0x09                
#define	 ASKII_RETURN			0x0d                
#define  ASKII_DELETE			0x10                
#define  ASKII_ESCAPE			0x1b                
#define  ASKII_SPACE			0x20                
#define  ASKII_EXCLAM			0x21 			// !
#define  ASKII_QUOTES			0x22			// "
#define  ASKII_HASH				0x23			// #
#define  ASKII_DOLOR			0x24			// $
#define  ASKII_PERCENT			0x25			// %
#define  ASKII_AMPERSAND		0x26			// &
#define  ASKII_SINGLEQUOTE 		0x27			// '
#define  ASKII_OPENPARAN		0x28			// (
#define  ASKII_CLOSEPARAN		0x29			// )
#define  ASKII_STAR				0x2A			// *
#define  ASKII_PLUS				0x2B			// + 
#define  ASKII_COMMA			0x2C			// ,
#define  ASKII_MINUS			0x2D			// - 
#define  ASKII_DOT				0x2E			// . 
#define  ASKII_FORDWARDSLASH	0x2F			// / 
#define  ASKII_ZERO				0x30                
#define  ASKII_1				0x31                
#define  ASKII_2				0x32                
#define  ASKII_3				0x33                
#define  ASKII_4				0x34                
#define  ASKII_5				0x35                
#define  ASKII_6				0x36                
#define  ASKII_7				0x37                
#define  ASKII_8				0x38                
#define  ASKII_9				0x39                
#define  ASKII_COLON			0x3a			// :
#define  ASKII_SEMICOLON		0x3b			// ; 
#define  ASKII_OPENANGLE		0x3c			// <
#define  ASKII_EQUAL			0x3d			// =
#define  ASKII_CLOSEANGLE		0x3e			// >
#define  ASKII_QUESTION			0x3f			// ?
#define  ASKII_AT				0x40  			// @              
#define  ASKII_A				0x41                
#define  ASKII_B				0x42                
#define  ASKII_C				0x43                
#define  ASKII_D				0x44                
#define  ASKII_E				0x45                
#define  ASKII_F				0x46                
#define  ASKII_G				0x47                
#define  ASKII_H				0x48                
#define  ASKII_I				0x49                
#define  ASKII_J				0x4a                
#define  ASKII_K				0x4b                
#define  ASKII_L				0x4c                
#define  ASKII_M				0x4d                
#define  ASKII_N				0x4e                
#define  ASKII_O				0x4f                
#define  ASKII_P				0x50                
#define  ASKII_Q				0x51                
#define  ASKII_R				0x52                
#define  ASKII_S				0x53                
#define  ASKII_T				0x54                
#define  ASKII_U				0x55                
#define  ASKII_V				0x56                
#define  ASKII_W				0x57                
#define  ASKII_X				0x58                
#define  ASKII_Y				0x59                
#define  ASKII_Z				0x5a                
#define  ASKII_OPENSQURE		0x5b			// [
#define  ASKII_BACKSLASH		0x5c			// "\"  
#define  ASKII_CLOSESQUARE		0x5d			// ]
#define  ASKII_CARAT			0x5e			// ^ 
#define  ASKII_UNDERBAR			0x5f			// _ 
#define  ASKII_GRAVE			0x60			// `
#define  ASKII_a				0x61                
#define  ASKII_b				0x62                
#define  ASKII_c				0x63                
#define  ASKII_d				0x64                
#define  ASKII_e				0x65                
#define  ASKII_f				0x66                
#define  ASKII_g				0x67                
#define  ASKII_h				0x68                
#define  ASKII_i				0x69                
#define  ASKII_j				0x6a                
#define  ASKII_k				0x6b                
#define  ASKII_l				0x6c                
#define  ASKII_m				0x6d                
#define  ASKII_n				0x6e                
#define  ASKII_o				0x6f                
#define  ASKII_p				0x70                
#define  ASKII_q				0x71                
#define  ASKII_r				0x72                
#define  ASKII_s				0x73                
#define  ASKII_t				0x74                
#define  ASKII_u				0x75                
#define  ASKII_v				0x76                
#define  ASKII_w				0x77                
#define  ASKII_x				0x78                
#define  ASKII_y				0x79                
#define  ASKII_z				0x7a                
#define  ASKII_CURLYOPEN		0x7b			// {
#define  ASKII_PIPE				0x7c			// |
#define  ASKII_CURLYCLOSE		0x7d			// }
#define  ASKII_TILDE			0x7e			// ~  




#define IX_ALPABAT_MAX					26					// ���ĺ� �ִ� ���� A~Z
#define IX_NUMBER_MAX					10					// ����  �ִ� ����  0~9 

#define IX_INPUT_DEVICE_MAX				16

#define IXERR_NONE						0                   //IXERR_NONE
#define IXERR_READ						1                   //IX READ ERROR
#define IXERR_FD						2                   //IX FD ERROR
#define IXERR_DEV_INFO_OPEN				3  	                //IXERR_DEV_INFO_OPEN
#define IXERR_AUTOSCAN_SCANDEVICE   	4					//ix_auto_scan_open �Լ� ���� ix_scan_device ����
#define IXERR_AUTOSCAN_MKNOD			5					//ix_auto_scan_open �Լ� ���� mknod ����",
#define IXERR_AUTOSCAN_DEVICE_OPEN  	6                   //ix_auto_scan_open �Լ� ���� ����̽� ���� ����"
#define IXERR_AUTOSCAN_EX_ADD        	7                   //ix_auto_scan_open �Լ� ���� ex �Լ� ��� ����",
#define IXERR_WRITE_EVENT 		       	8                   //ix_write_event_low �Լ� ���� �߸��� ���� ����",
#define IXERR_WRITE_MOUSE_BUTTON_EVENT 	9                   //ix_write_mouse_button_action �Լ� ���� �߸��� ���� ����",
#define IXERR_WRITE_KEYBOARD_EVENT		10					//ix_write_keyboard_key_action �Լ� ���� �߸��� ���� ����",


#define	IX_KEY_VAULE_UP_LOW				0					// Ű��ư ���� ����  LOW �� 
#define	IX_KEY_VAULE_DOWN_LOW			1					// Ű��ư ����
#define	IX_KEY_VAULE_PRESS_LOW			2					// Ű��Ʈ DOWN->UP �� ��, ��Ű�� �ڵ尪���� CODE�� ��ȯ�ϴ�. 

#define	IX_MOUSE_VAULE_UP_LOW			0					//  ���콺 ������ ��ư�� ����
#define	IX_MOUSE_VAULE_DOWN_LOW			1					//  ���콺 ������ ��ư�� ���� ����

//---------------------------------------------------------------------------------
// ��Ʈ ����ũ 
//---------------------------------------------------------------------------------
#define IX_MOUSE_BTN_LEFT				0x00000001			// ���콺 ���� ��ư
#define IX_MOUSE_BTN_RIGHT				0x00000002			// ���콺 ������ ��ư
#define IX_MOUSE_BTN_MIDDLE				0x00000004			// ���콺 ��ũ�� ��ư

#define IX_MOUSE_FLAG_BTN_DOWN			0x00000001			// ���콺 ��ư ����
#define IX_MOUSE_FLAG_BTN_UP			0x00000002			// ���콺 ��ư ������
#define IX_MOUSE_FLAG_MOVE				0x00000004			// ���콺 ������
#define IX_MOUSE_FLAG_DOUBLE_CLICK		0x00000008			// ���콺 ������


#define	IX_KEY_VAULE_UP					0x00000001			// Ű��ư ���� ����   , state�� make�� ���ȴ�. 
#define	IX_KEY_VAULE_DOWN				0x00000002			// Ű��ư ����
#define	IX_KEY_VAULE_PRESS				0x00000004			// Ű��Ʈ DOWN->UP �� ��, ��Ű�� �ڵ尪���� CODE�� ��ȯ�ϴ�. 
//#define	IX_KEY_VAULE_REPEAT			0x00000008					// Ű��ư ���� ����  (���� ���)

#define IX_KEYBOARD_BTN_LEFT_SHIFT		0x80000000			// Ű���� ���� SHIFT
#define IX_KEYBOARD_BTN_RIGHT_SHIFT		0x40000000			// Ű���� ������ SHIFT

#define IX_KEYBOARD_BTN_LEFT_ALT		0x20000000			// Ű���� ���� ALT
#define IX_KEYBOARD_BTN_RIGHT_ALT		0x10000000			// Ű���� ������ ALT

#define IX_KEYBOARD_BTN_LEFT_CTRL		0x08000000			// Ű���� ���� CTRL
#define IX_KEYBOARD_BTN_RIGHT_CTRL		0x04000000			// Ű���� ������ CTRL

#define IX_KEYBOARD_CAPSLOCK			0x02000000			// Ű���� Caps Lock ��ư�� ����     // imesu 090215
#define IX_KEYBOARD_NUMLOCK				0x01000000			// Ű���� Num Lock ��ư�� ����     // imesu 090215

#define IX_KEYBOARD_UP					0x00800000			// Ű���� ����  	����Ű
#define IX_KEYBOARD_DOWN				0x00400000			// Ű���� �Ʒ��� 	����Ű
#define IX_KEYBOARD_LEFT				0x00200000			// Ű���� ���� 		����Ű ��ư�� ����     // imesu 090215
#define IX_KEYBOARD_RIGHT				0x00100000			// Ű���� ������ 	����Ű ��ư�� ����     // imesu 090215



#define IX_SCREEN_X_DEFAULT				640					// ��ũ�� X ������
#define IX_SCREEN_Y_DEFAULT				480					// ��ũ�� Y ������

#define IX_CAL_TOUCH_INFO_MAX			4					// ��ǥ �Է� �ִ� ���� ����

#if 0//wj@@test-touch
#define IX_CAL_TOUCH_INFO_OFSSET		1				// TOUCH SCREEN ������ 
#else
#define IX_CAL_TOUCH_INFO_OFSSET		0x00010000          //(1<< 16)// TOUCH SCREEN ������ 
#endif



#define IX_MOUSE_DOUBLE_CLICK_TIME_INTAVAL	   500			//500msec = 0.5 �� 	
//---------------------------------------------------------------------------------
// ����� ���� �̺�Ʈ �޽���
//---------------------------------------------------------------------------------
#define IE_MOUSE_DOWN					1
#define IE_MOUSE_UP 					2
#define IE_MOUSE_MOVE   				4
#define IE_KEY_PRESS 					5
#define IE_KEY_DOWN 					6
#define IE_KEY_UP 						7

typedef struct input_event ix_event_t;					// kernel �� �ִ� �̺�Ʈ ����ü /include/linux/input.h

typedef struct
{
	int     fd;					// ��ũ����
	char 	name[128];			// ��ġ �̸�
	int 	input_number;		// �Է���ġ ��ȣ
	int 	event_number;		// event ��ġ ��ȣ

	int 	dev_major;			// ��ġ �ֹ�ȣ
	int 	dev_minor;			// ��ġ �ι�ȣ

	int		is_touch;			// ��ġ ����̽� �̸� 1 0 �̸� �ƴ� 

} ix_mng_t;

typedef  unsigned long ix_btn_state_t;	// ��ư�� ���� �ڷ���

typedef struct {
	int  tx;					// touch screen x��ǥ��
	int  ty;					// touch screen y��ǥ��

	int  sx;					// lcd screen x��ǥ��
	int  sy;					// lcd screen y��ǥ��
}ix_cal_touch_info_t;

typedef struct {
	int  xa;					// ����
	int  xb;					//

	int  ya;					// ����
	int  yb;					//
}ix_adjust_touch_info_t;


typedef struct {
	int				x;			//x ��ǥ
	int				y;			//y ��ǥ
	ix_btn_state_t  state;		//����

}ie_mouse_t;

typedef struct {
	int				key;		//�Է� ǥ��
	ix_btn_state_t  state;		//����
}ie_key_t;

typedef struct {
	int		type;				// �Է°� ����
	union{
		ie_mouse_t		mouse;	// ���콺 �Է�
		ie_key_t		key;	// keyboard �Է�
	}data;
}ie_event_t;


typedef      int (*ix_event_func_t) (ie_event_t *event_data);

#ifdef __cplusplus
extern "C"
{
#endif


	void 		ix_print_error				( char *msg );								// ���� �޽��� ���
	void		ix_debug_disp_event_data	( ix_event_t *event_data );					// ������ �̺�Ʈ ������ ���� ǥ���Ѵ�.
	void		ix_debug_disp_ix_device_info( void );									// ix_scan_device ���� ���� �Է� ��ġ�� ���� ���� ȭ�鿡 ǥ���Ѵ�. 


	int 		ix_init						( int mode );
	int			ix_auto_scan_open			( char *path );								// 1)�Է� ��ġ�� �ڵ� �˻�
																						// 2) mknod
																						// 3) ����̽� ���� ����..
																						// 4) ex �Լ� ���
																						
	int 		ix_get_touch_fd				( char *path );								// ��ġ ��ũ�� ���� ��ũ���͸� ���Ѵ�. 

	int 		ix_add_event				( ix_event_func_t func );					//�̺�Ʈ �����͸� ó���� �Լ��� ����Ѵ�.
	int 		ix_scan_device				( void );									// �Է� ��ġ ������ �˻��Ѵ�.

	int 		ix_read_event_data_low		( int fd,  ix_event_t *event_data );		//�̺�Ʈ ������ �����͸� �о� ���°�
	int 		ix_read_keyboard_low		( int fd,  int *key, int *key_state );		//Ű���� ���� �о� �´�.
	int 		ix_read_key_press_low		( int fd,  int *key );						//Ű���尡 �������� ��
	int 		ix_read_touch_low			( int fd,  int *x, int *y, int *press );	// touch ���� �о� �´�.
	int 		ix_read_mouse_low			( int fd,  int *x, int *y, int *btn);		//���콺 �Է°��� ��� �´�.
	int 		ix_set_screen_size			( int x ,  int y);

	int			ix_write_event_low			( int fd, ix_event_t *event_data );			//�̺�Ʈ ��ġ�� ����ڰ� ���� �̺�Ʈ ������ �־ �̺�Ʈ�� �߻���Ų��.
	int			ix_write_mouse_button_action( int mouse_button, int action );			//���콺 Ư����ư�� Up down ���� �̺�Ʈ�� �߻���Ų��.
	int			ix_write_mouse_rel_move		( int rel_x, int rel_y );					//���콺 ���� ��ġ ���� �̺�Ʈ�� �߻���Ų��.
	int			ix_write_mouse_abs_move		( int abs_x, int abs_y );					//���콺 ���� ��ġ ���� �̺�Ʈ�� �߻���Ų��.
	int			ix_write_keyboard_key_action( int keyboard_key, int action);			//Ű���忡 Ű �̺�Ʈ�� �߻���Ų��.
	int			ix_write_key_press		( int askii );								//Ű���忡 key press ���� �ִ´�. 

	int			ix_get_mouse_position		( int *x,  int *y );						// ���콺 ���� ��ġ�� ��´�.
	int			ix_set_mouse_double_click_intaval( int msec );							// ���콺 ���� Ŭ�� ���� ���� 
	int			ix_get_key_value			( int *key );								// keyboard �Է°��� ��´�.


	int			ix_adjust_touch				( void );									// touch screen�� �����Ѵ�.
	//int 		ix_get_touch_position		( int *x, int *y );							// ��ġ ��ũ�� ������ ��ġ���� ��� �´�. 
    int         ix_get_touch                ( int x_tch, int y_tch, int *x_scr_ptr, int *y_scr_ptr );
    int         ix_get_touch_info           ( ix_adjust_touch_info_t *info );
    int         ix_set_touch                ( ix_adjust_touch_info_t *info );
    int         ix_print_touch              ( void );
    int         ix_adjust_touch_init        ( void );

	int			ix_loading_ts_cailbration_info( void );									//��ġ ��ũ�� Į���� ���̼� ������ �ε��Ѵ�. 

	int			ix_write_ts_cailbration_info( char *section, char *keyname ,int value );	// ��ġ��ũ�� �Է� ���� ����. 
#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
// ���� ���� ����
//------------------------------------------------------------------------------






#ifndef _IX_VAR_

#endif

#endif  // _IX_HEADER_

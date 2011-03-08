//------------------------------------------------------------------------------
// �� �� �� : serial.h
// ������Ʈ : ezboot
// ��    �� : ezboot���� ����ϴ� serial�� ���õ� ����
// �� �� �� : ����â
// �� �� �� : 2001�� 11�� 3��
// �� �� �� : 
// ��    �� : �� ��� ȭ���� ��κ��� ������ 
//            blob�� serial.h���� ���� �Դ�. 
//            ���� ���� �Դ�. 
//------------------------------------------------------------------------------

#ifndef _SERIAL_HEADER_
#define _SERIAL_HEADER_

// PXA250�� ���� Ŭ���� �̿��Ҷ��� �� ��������Ʈ�� �ش��ϴ� �� 

typedef enum 
{ 
	BAUD_4800    = 192 ,
	BAUD_9600    =  96 ,
	BAUD_19200   =  48 ,
	BAUD_38400   =  24 ,
	BAUD_57600   =  16 ,
	BAUD_115200  =   8 ,
	BAUD_230400  =   4 ,
	BAUD_307200  =   3 ,
	BAUD_460800  =   2 ,
	BAUD_921600  =   1
} eBauds;
    
extern void SerialInit( eBauds baudrate);		// ���� �ø��� �ʱ�ȭ 
extern void SerialOutChar( const char c  );    	// �ø��� �� ���ڸ� ����Ѵ�. 
extern int  SerialOutStr( char *str, int size );	// �ø��� ������ ������ ����Ѵ�. 
extern void SerialOutChar_CheckCR( const char c  );    // �ø��� �� ���ڸ� ����Ѵ�. 
extern int  SerialOutStr_CheckCR( char *str, int size ); // �ø��� ������ ������ ����Ѵ�. 

extern int SerialIsReadyChar( void );
extern char SerialIsGetChar( void );
extern char SerialIsClearError( void );
extern int SerialIsGetError( void );

#endif //_SERIAL_HEADER_

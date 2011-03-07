//------------------------------------------------------------------------------
// ȭ�ϸ� : printf.c
// ��  �� : ezBoot���� ���Ǵ� printf �Լ� ��ƾ�̴�. 
// 
// �ۼ��� : ����â (��)���̴��Ƽ frog@falinux.com
// �ۼ��� : 2001�� 11�� 4��
// ���۱� : (��)���̴��Ƽ 
// ��  �� : 
//------------------------------------------------------------------------------
#include <pxa255.h>
#include <time.h>
#include <gpio.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
// 
// ���� ���� 
// 
//------------------------------------------------------------------------------
#define HIS_MAX               8

char HisBuff[HIS_MAX][32] = { {0,}, };
int  HisCount               = 0;
int  HisIndex               = 0;

//------------------------------------------------------------------------------
// ���� : �ѹ��ڸ� ����Ѵ�. 
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : ĳ���� ������ ��ȭ�� ó���Ѵ�. 
//------------------------------------------------------------------------------
int putc(char c)
{
	SerialOutChar_CheckCR( c );
	return 1;
}
//------------------------------------------------------------------------------
// ���� : �ѹ��ڸ� ����Ѵ�. 
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : ���� 
//------------------------------------------------------------------------------
int putx(char c)
{
	SerialOutChar( c );
	return 1;
}
//------------------------------------------------------------------------------
// ���� : ������ ���ڿ��� ����Ѵ�. 
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : ���� 
//------------------------------------------------------------------------------
int printf(const char *fmt, ...)
{
	char buffer[1024];
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vsprintf(buffer, fmt, ap);
	va_end(ap);

        SerialOutStr_CheckCR(buffer, len);

	return len;
}

//------------------------------------------------------------------------------
// ���� : �ѹ����� �Է��� ��´�. 
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : ���� 
//------------------------------------------------------------------------------
int getc(void)
{
	int lp = 0;

	// �ѹ��� ��� 
	while( !SerialIsReadyChar() ) 
    {
                lp++; 
    }
	return SerialIsGetChar() & 0xFF;
}
//------------------------------------------------------------------------------
// ���� : ���ڿ� �Է��� �޴´�. ( CR�� �Էµɶ����� )
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : ���� 
//------------------------------------------------------------------------------
int gets(char *s)
{
     int cnt = 0;
     char  c;

     while((c = getc()) != CR)
     {
          if(c != BS) 
          {  
               cnt++;
               *s++ = c;
               printf("%c",c );
          }
          else 
          {
               if(cnt > 0) 
               { cnt--; *s-- = ' '; 
                 printf("\b \b");  
               } 
               
          }
     }
     *s = 0;
   
     return(cnt);
}

//------------------------------------------------------------------------------
// ���� : �����丮�� �߰� �Ѵ�. 
// �Ű� : s : ������ ���� �ּ� 
// ��ȯ : ���� ���ۿ� ����� �����丮 ��
// ���� : 
//------------------------------------------------------------------------------
int his_append(char *s)
{
    int loop;

    // �̹� ���� ���ڿ��� ������ �����Ѵ�. 
    for( loop = 0; loop < HIS_MAX; loop++ )
    {
      if( strcmp( s, HisBuff[loop] ) == 0 ) 
      {
          HisIndex = 0;
          if( HisCount ) HisIndex = HisCount-1;
          return HisCount;
      } 
    } 

    // ���ο� ������ �����丮 ���ۿ� �߰� �Ѵ�. 
    if( HisCount < HIS_MAX )
    {
        strcpy( HisBuff[HisCount], s );
        HisCount++;
    }
    else
    {   
        for( loop = 1; loop < HIS_MAX ; loop++ )
        {
            strcpy( HisBuff[loop-1], HisBuff[loop] );
        }
        strcpy( HisBuff[HIS_MAX-1], s );
        HisIndex = HisCount-1;
    }

    HisIndex = 0; 
    if( HisCount ) HisIndex = HisCount-1;

    return HisCount;
}
//------------------------------------------------------------------------------
// ���� : �����丮�� ���� ���ۿ� �����Ѵ�. 
// �Ű� : s : ������ ���� 
//        index : ������ ����  
// ��ȯ : ���� 
// ���� : 
//------------------------------------------------------------------------------
int his_set(char *s, int index )
{
    int loop;
    int len;

    len = strlen( s );

    // ���� ���� ���ڿ��� �����. 
    for( loop = 0; loop < len; loop++ )
    {
      printf("\b \b");
    }
     
    // ���� ������ �����Ѵ�. 
    strcpy( s, HisBuff[index] ); 
    printf( "%s", s );

    return index;
}

//------------------------------------------------------------------------------
// ���� : ���ڿ� �Է��� �޴´�. ( CR�� �Էµɶ����� )
// �Ű� : ���� 
// ��ȯ : ���� 
// ���� : �����丮 ����� �߰� �Ͽ���. 
//------------------------------------------------------------------------------
int gets_his(char *s)
{
     int cnt = 0;
     char  *fp;
     char  c;

     fp = s;
     
     while( 1 )
     {
          c = getc();

          if( c == CR )
          {
              *s = 0;
              if( strlen( fp ) ) his_append( fp );
              break;
          } 

          switch( c )
          {
          case 0x1a  : // ^Z
                       if( HisIndex >= 0 )
                       { 
                           his_set( fp, HisIndex );
                           if( HisIndex ) HisIndex--;
                           if( HisIndex >= HisCount ) HisIndex = HisCount -1;
                           cnt = strlen( fp );
                           s = fp + cnt; 
                       } 
                       break;
          case 0x18  : // ^X
                       if( HisIndex < HisCount )
                       { 
                           his_set( fp, HisIndex );
                           HisIndex++;
                           cnt = strlen( fp );
                           s = fp + cnt; 
                       }  
                       break;
      
          case BS    : if( cnt > 0 )
                       { 
                         cnt--; *s-- = ' '; 
                         printf("\b \b");  
                       } 
                       break;
          default    : cnt++;
                       *s++ = c;
                       printf("%c",c );
                       break;
          }
     }
   
     return(cnt);

}

//------------------------------------------------------------------------------
// ���� : �ѹ����� �Է��� ��´�. 
// �Ű� : ��Ű, msec
// ��ȯ : ���� -1  ���� 0
// ���� : �ð��� ������ �ִ�. 
//------------------------------------------------------------------------------
#define halfsec ((TICKS_PER_SECOND/1000)*500)
int     getc_timed( char cmpKey, int mTimeOver )
{
	unsigned long ticks, end;
	int     toggle = 0;
	char    key;
	
	if ( mTimeOver == 0 ) return -1;
	
	// ƽ�� ����Ѵ�.
	ticks   = (TICKS_PER_SECOND/1000) * mTimeOver;
	end     = OSCR + ticks;
        
        OSSR  = OSSR_M0 | OSSR_M1;      // Ÿ�̸�0,1 �����÷ο��Ʈ Ŭ����
        OSMR0 = end;                    // Ÿ�̸�0 �����÷ο찡 �߻��� ƽ
        OSMR1 = OSCR + halfsec;
        // Ÿ�̸�0 ���ͷ�Ʈ Ȱ��ȭ
        OIER = OSSR_M0 | OSSR_M1;

        // Ÿ�̸�0 �����÷ο��Ʈ�� ������ ���� ���
        while( 0 == (OSSR&OSSR_M0) )
        {
                if ( SerialIsReadyChar() )
                {
                        key = SerialIsGetChar()&0xFF;
                        if ( cmpKey     == key ) return 0;
                        if ( (char)0x0d == key ) return -1;
                }
                
                // 0.5 �ʸ��� �����δ�.
                if ( OSSR&OSSR_M1 )
                {
                        OSSR  = OSSR_M1;      // Ÿ�̸�1 �����÷ο��Ʈ Ŭ����
                        OSMR1 = OSCR + halfsec;
                        
                        toggle ++;
                        printf( "." );
                }        
        }
        
        // ��� Ÿ�̸� ���ͷ�Ʈ ��Ȱ��ȭ
        OIER = 0;

        return -1;
}







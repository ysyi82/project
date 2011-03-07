//------------------------------------------------------------------------------
// �� �� �� : gpio.h
// ������Ʈ : ezboot
// ��    �� : ezboot���� ����ϴ� gpio�� ���õ� ��� 
// �� �� �� : ����â
// �� �� �� : 2001�� 11�� 3��
// �� �� �� : 
// ��    �� : 
//------------------------------------------------------------------------------

#ifndef _GPIO_HEADER_
#define _GPIO_HEADER_

#define	LED_0			GPIO_bit(2)
#define	LED_1			GPIO_bit(3)
#define	LED_2			GPIO_bit(4)
#define	LED_3			GPIO_bit(5)	// 

#define	LED_OFF			0
#define	LED_ON			1

void  set_GPIO_mode(int gpio_mode);
void  GPIOInit(void); 				// GPIO ���� �ʱ�ȭ
void  GPIO_SetLED( int LedIndex, int value ); 	// GPIO �� ����� LED�� �����Ѵ�.

#endif //_GPIO_HEADER_


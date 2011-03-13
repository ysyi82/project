#include <navilnux.h>

void swiHandler(unsigned int syscallnum)
{
	printf("system call %d\n", syscallnum);
}

void irqHandler(void)
{
	if( (ICIP&(1<<27)) != 0 )
	{
		OSSR = OSSR_M1;
		OSMR1 = OSCR + 3686400;
		printf("Timer Interrupt!!!\n");
	} 
}

void os_timer_init(void)
{
	ICCR = 0x01;

	ICMR |= (1 << 27);      
	ICLR &= ~(1 << 27);    

	OSCR = 0;
	OSMR1 = OSCR + 3686400; 

	OSSR = OSSR_M1;
}

void os_timer_start(void)
{
	OIER |= (1<<1);
	OSSR = OSSR_M1;
}

void irq_enable(void)
{
	__asm__("msr    cpsr_c,#0x40|0x13");
}

void irq_disable(void)
{
	__asm__("msr    cpsr_c,#0xc0|0x13");
}

int main(void)
{
	int a = 1;
	int b = 2;
	int c = 0;

	c = a + b;
	os_timer_init();
	os_timer_start();

	irq_enable();

	while(1)
	{
		printf("kernel stack a(%p), b(%p), c(%p)\n", &a, &b, &c);
		__asm__("swi 77");
		msleep(1000);
	}
	return 0;

#if 0
	int  i = 0;
	while( 1 )
	{
		GPIO_SetLED( i, LED_OFF );
		__asm("swi 77");
		msleep( 1000 );
		GPIO_SetLED( i, LED_ON );
		i++;
		if( i >= 4 ) i = 0;

	}
	return 0;
#endif
}
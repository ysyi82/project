#if 1
#include <navilnux.h>

extern Navil_task_mng taskmng;

void user_task_1(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while(1){
		printf("TASK1 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		msleep(1000);
	}
}

void user_task_2(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while(1)
	{
		printf("TASK2 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		//c = mysyscall4(4,5,6,7);
		//printf("Syscall4 return value is %d\n", c);
		msleep(1000);
	}
}

void user_task_3(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while(1)
	{
		printf("TASK3 - a:%p\tb:%p\tc:%p\n", &a, &b, &c);
		c = mysyscall(1,2,3);
		printf("Syscall return value is %d\n", c);
		msleep(1000);
	}
}

void navilnux_user(void)
{
	taskmng.create(user_task_1);
	taskmng.create(user_task_2);
	taskmng.create(user_task_3);
}

#else
#include <navilnux.h>

extern Navil_task_mng taskmng;

void user_task_1(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while( 1 )
	{
		printf("TASK1 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
	}
}

void user_task_2(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while( 1 )
	{
		printf("TASK2 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
	}
}

void user_task_3(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	while( 1 )
	{
		printf("TASK3 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
		c = mysyscall(1, 2, 3);
		printf("Syscall return value is %d\n", c);
		//msleep(1000);
	}
}

void navilnux_user(void)
{
	task_create(user_task_1);
	task_create(user_task_2);
	task_create(user_task_3);
}
#endif

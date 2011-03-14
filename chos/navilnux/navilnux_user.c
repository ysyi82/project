#include <navilnux.h>

extern Navil_task_mng taskmng;

void user_task_1(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	printf("TASK1 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
}

void user_task_2(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	printf("TASK1 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
}

void user_task_3(void)
{
	int a, b, c;

	a = 1;
	b = 2;
	c = a + b;

	printf("TASK1 -a:%p\t b:%p\t c:%p\n", &a, &b, &c);
}

void navilnux_user(void)
{
	task_create(user_task_1);
	task_create(user_task_2);
	task_create(user_task_3);
}
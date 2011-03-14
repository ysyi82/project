#ifndef _NAVIL_TASK
#define _NAVIL_TASK

#define MAXTASKNUM 40
#define CONTEXTNUM 13

typedef struct _navil_free_task
{
	unsigned int context_spsr;
	unsigned int context[CONTEXTNUM];
	unsigned int context_sp;
	unsigned int context_lr;
	unsigned int context_pc;
} Navil_free_task;

typedef struct _navil_free_mng
{
	Navil_free_task free_task_pool[MAXTASKNUM];

	int max_task_id;

	void (*init)(void);
	int (*create)(void(*startFunc)(void));
} Navil_task_mng;

void task_init(void);
int task_create(void (*startFunc)(void));

#endif //_NAVIL_TASK

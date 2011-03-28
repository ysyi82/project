#ifndef _NAVIL_MSG_
#define _NAVIL_MSG_

#define MAXMSG 255

#define ITCSTART 0
#define ITCEND 99
#define SEMSTART 100
#define SEMEND	199

typedef struct _navil_free_msg
{
	int data;
	int flag;
} Navil_free_msg;


typedef struct _navil_msg_mng
{
	Navil_free_msg free_msg_pool[MAXMSG];

	void (*init)(void);

	int  (*itc_send)(int, int);
	int  (*itc_get)(int, int*);

	int (*sem_init)(int, int);
	int (*sem_p)(int);
	int(*sem_v)(int);

} Navil_msg_mng;

void msg_init(void);
int  msg_itc_send(int, int);
int  msg_itc_get(int, int*);

int msg_sem_init(int, int);
int msg_sem_p(int);
int msg_sem_v(int);

#endif

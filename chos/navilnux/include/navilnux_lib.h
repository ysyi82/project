#ifndef _NAVIL_LIB
#define _NAVIL_LIB

extern int mysyscall(int, int, int);
extern int mysyscall4(int, int, int, int);

extern int itc_sned(int, int);
extern int itc_get(int, int*);
extern int sem_p(int);
extern int sem_v(int);

extern void call_scheduler(void);

int navilnux_itc_send(int, int);
int navilnux_itc_get(int);
int navilnux_sem_p(int);
int navilnux_sem_v(int);


#endif

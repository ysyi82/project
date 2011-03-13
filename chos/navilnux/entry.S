/*========================================================================================================================

  화일명 : entry.S
  설  명 : 램 상에 존재하는 ezBoot의 메인 C 함수의 main을 호출하기 위한 루틴이다.
  
  작성자 : (주)제이닷디앤티  유영창
  작성일 : 2001년 10월 8일
  
  비  고 :  
==========================================================================================================================*/
.globl _ram_entry
_ram_entry:
	b	kernel_init
	b	_ram_entry // main 함수의 호출이 끝났다면 재 호출한다. 
	b	navilnux_swiHandler
	b	navilnux_irqHandler

#define svc_stack 0xa0300000
#define irq_stack 0xa0380000
#define sys_stack 0xa4000000


.global kernel_init
kernel_init:
	msr		cpsr_c,#0xc0|0x13	//SVC mode
	ldr		r0,=svc_stack
	sub		sp,r0,#4

    msr		cpsr_c,#0xc0|0x12    //IRQ mode
    ldr     r0,=irq_stack
    sub     sp,r0,#4

	msr		cpsr_c,#0xc0|0x13	//SYSTEM mode
	ldr		r0,=sys_stack
	sub		sp,r0,#4

    msr     cpsr_c,#0xc0|0x13

    bl      main
    b       _ram_entry

.global navilnux_swiHandler
navilnux_swiHandler:
    stmfd   sp!,{r0-r12,r14}
    mrs     r1,spsr
    stmfd   sp!,{r1}
    ldr     r10,[lr,#-4]
    bic     r10,r10,#0xff000000
    mov     r0,r10
    bl      swiHandler
    ldmfd   sp!,{r1}
    msr     spsr_cxsf,r1
    ldmfd   sp!,{r0-r12,pc}^

.global navilnux_irqHandler
<<<<<<< HEAD
<<<<<<< HEAD
navilnux_irqHandler:
	sub lr,lr,#4
	stmfd sp!,{lr}
	stmfd sp!,{r0-r14}^
	mrs r1,spsr
	stmfd sp!,{r1}
	bl irqHandler
	ldmfd sp!,{r1}
	msr spsr_cxsf,r1
	ldmfd sp!,{r0-r14}^
	ldmfd sp!,{pc}^
=======
=======
>>>>>>> 68609ce8f05d92d192eaf8057acdea68d69f2f24
navilnux_irqHandler:    
    sub     lr,lr,#4
    stmfd   sp!,{lr}
    stmfd   sp!,{r0-r14}^
    mrs     r1,spsr
    stmfd   sp!,{r1}
    bl      irqHandler
    ldmfd   sp!,{r1}
    msr     spsr_cxsf,r1
    ldmfd   sp!,{r0-r14}^
    ldmfd   sp!,{pc}^
<<<<<<< HEAD
>>>>>>> origin
=======
>>>>>>> 68609ce8f05d92d192eaf8057acdea68d69f2f24

/*========================================================================================================================

  화일명 : entry.S
  설  명 : 램 상에 존재하는 ezBoot의 메인 C 함수의 main을 호출하기 위한 루틴이다.
  
  작성자 : (주)제이닷디앤티  유영창
  작성일 : 2001년 10월 8일
  
  비  고 :  
==========================================================================================================================*/
.globl _ram_entry
_ram_entry:
	bl	main	   // C 함수의 main함수를 호출한다. 	
	b	_ram_entry // main 함수의 호출이 끝났다면 재 호출한다. 
	b	navilnux_swiHandler
	b	navilnux_irqHandler

.global navilnux_swiHandler
navilnux_swiHandler:
	stmfd	sp!,{r0-r12, r14}
	mrs	r1, spsr
	stmfd	sp!,{r1}
	ldr	r10, [lr, #-4]
	bic	r10, r10, #0xff000000
	mov	r0,	r10
	bl	swiHandler
	ldmfd	sp!,{r1}
	msr	spsr_cxsf, r1
	ldmfd	sp!,{r0-r12, pc}^

.global navilnux_irqHandler
navilnux_irqHandler:
	bl	irqHandler
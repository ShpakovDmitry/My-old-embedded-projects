/*-------------------------------------------------------------------*/
.macro Delayus us
	.if (\us*3-5) > 0xffff
	.warning "Слишком длинная задержка времени"
	.endif
	.if (\us*3-5) < 0
	.warning "Слишком короткая задержка времени"
	.endif
	push r25		;2 ticks
	push r24		;2 ticks
	ldi r25, hi8(\us*3-5)	;1 tick
	ldi r24, lo8(\us*3-5)	;1 tick
	call Delay		;10+4*(r25:r24) ticks
	pop r24			;2 ticks
	pop r25			;2 ticks
.endm
/*-------------------------------------------------------------------*/

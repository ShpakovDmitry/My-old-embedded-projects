.include "macros.s"
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;initialize registers
InitRegisters:
	;Stack
	ldi r16, hi8(RAMEND)
	out SPH, r16
	ldi r16, lo8(RAMEND)
	out SPL, r16		;set stack


	;SPI - master, mode 0
	sbi DDRB, SS	;SS as output
	sbi DDRB, SCK	;SCK as output
	sbi DDRB, MOSI	;MOSI as output
	sbi SPCR, MSTR	;SPI in master mode
	sbi SPCR, SPE	;enable SPI
	sbi SPSR, SPI2X	;SPI clock = f_osc/2

	;Timer/Counter2
	in r16, ASSR
	ori r16, (1 << AS2)
	out ASSR, r16		;asynchronous timer/counter

	in r16, TCCR2
	ori r16, (1 << CS22)
	;ori r16, (1 << CS20)
	out TCCR2, r16		;set prescaler to 1/128


	in r16, TIMSK
	ori r16, (1 << TOIE2)
	out TIMSK, r16		;overflow interrupt enable
	
	
	
	
	
	;sei
	cli
	ret
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;Infinite loop
InfiniteLoop:
	rjmp InfiniteLoop
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;Delay function	parameters are in r25:r24
Delay:
	sbiw r24, 0x01	;2 ticks
	brne Delay	;1 tick if Z = 1, else 2 ticks
	nop		;1 tick
	nop		;1 tick
	nop		;1 tick
	ret		;4 ticks

/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;Send to register throught SPI, bytes are located in SRAM
;lower address is LSB
SendToRegister:
	push r26	;
	push r27	;save X register
	push r16	;save r16 register
	push r17	;save r17 register
	cbi PORTB, SS
	
	ldi r17, NumOf595N
	ldi r26, lo8(DataToRegisterAddress + NumOf595N)
	ldi r27, hi8(DataToRegisterAddress + NumOf595N)
SendToRegisterMainLoop:
	cpi r17, 0x00
	breq QuitSendToRegister
	ld r16, -X
	out SPDR, r16
SendToRegisterWait_Transmit:
	sbis SPSR, SPIF
	rjmp SendToRegisterWait_Transmit

	dec r17
	rjmp SendToRegisterMainLoop

QuitSendToRegister:
	sbi PORTB, SS
	pop r17		;restore r17 register
	pop r16		;restore r16 register
	pop r27		;
	pop r26		;restore X register
	ret
/*-------------------------------------------------------------------*/






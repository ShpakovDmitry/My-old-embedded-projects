.include "macros.s"
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;initialize registers
InitRegisters:
;-------Initialize Stack
	ldi r16, hi8(RAMEND)
	out SPH, r16
	ldi r16, lo8(RAMEND)
	out SPL, r16		;set stack




;-------Configure PORTS
	sbi DDRD, 2	;RTS exit
	cbi DDRD, 3	;CTR input
	sbi PORTD,2	;RTS high


;-------Configure Timer/Counter2
	in r16, ASSR
	ori r16, (1 << AS2)
	out ASSR, r16		;asynchronous timer/counter

	in r16, TCCR2
	ori r16, (1 << CS22)
	ori r16, (0 << CS21)
	ori r16, (1 << CS20)
	out TCCR2, r16		;set prescaler to 1/128

	in r16, TIMSK
	ori r16, (1 << TOIE2)
	out TIMSK, r16		;overflow interrupt enable
	


;-------Init UART	BaudRate = 9600 F_CPU = 12 MHz
	ldi r16, 0x4d
	ldi r17, 0x00
	out UBRRH, r17
	out UBRRL, r16		;load 0x004d to UBRR

	ldi r16, (1 << RXEN) | (1 << TXEN)
	out UCSRB, r16		;enable UART receiver and transmitter

	ldi r16, (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0)
	out UCSRC, r16		;8-bit mode, no parity, one stop bit


	sei
	;cli
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
;Send throught UART one byte, byte is located in r16
UARTSendByte:
	sbis UCSRA, UDRE
	rjmp UARTSendByte	;wait until last transmit ends
	
	out UDR, r16		;transmit data
	ret
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;Receive one byte throught UART, byte will be located in r16
UARTReceiveByte:
	sbis UCSRA, RXC
	rjmp UARTReceiveByte	;wait until byte is received

	in r16, UDR

	ret
/*-------------------------------------------------------------------*/













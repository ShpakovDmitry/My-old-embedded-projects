;программа для работы сдвигового
;регистра 74HC595N используя SPI
;
; RCLK <-> SS
;SRCLK <-> SCK
;  SER <-> MOSI

NumOf595N = 1
DataToRegisterAddress = 0x000

.include "ATMega16Def.s"


.org 0x00
;interrupt vectors initialisation
	jmp main	;External pin, power-on reset,
			;	brown-out reset, watchdog reset,
			;	JTAG AVR reset
	jmp main	;External interrupt request 0
	jmp main	;External interrupt request 1
	jmp main	;Timer/Counter2 compare match
	jmp T2_OVF	;Timer/Counter2 overflow
	jmp main	;Timer/Counter1 capture event
	jmp main	;Timer/Counter1 compare match A
	jmp main	;Timer/Counter1 compare match B
	jmp main	;Timer/Counter1 overflow
	jmp main	;Timer/Counter0 overflow
	jmp main	;Serial transfer complete
	jmp main	;USART, Rx complete
	jmp main	;USART data register empty
	jmp main	;USART, Tx complete
	jmp main	;ADC conversion complete
	jmp main	;EEPROM ready
	jmp main	;Analog comparator
	jmp main	;Two-wire serial interface
	jmp main	;External interrupt request 2
	jmp main	;Timer/Counter0 compare match
	jmp main	;Store programm memory ready


.include "functions.s"	



main:
	call InitRegisters
	

	ldi r26, lo8(DataToRegisterAddress)
	ldi r27, hi8(DataToRegisterAddress)
	
	ldi r16, 0b00000000
	st X, r16
	call SendToRegister

	call InfiniteLoop


T2_OVF:
	reti





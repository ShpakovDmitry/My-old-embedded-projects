.include "macros.s"
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
;initialize registers
InitRegisters:
	ldi r16, hi8(RAMEND)
	out SPH, r16
	ldi r16, lo8(RAMEND)
	out SPL, r16		;set stack

	sbi DDRA, 0	;PA0 as exit
	sbi PORTA,0	;PA0 high

	sbi DDRD, 4	;PD4->E as exit
	sbi DDRD, 5	;PD5->R/W as exit
	sbi DDRD, 6	;PD6->RS as exit
	sbi DDRD, 7	;PD7->RST as exit

	sbi PORTD, 7

	ldi r16, 0xff	;
	out DDRC, r16	;PORTC as exit

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
;Externally reset ST7920
ResetDisplay:
	sbi PORTD, 7
	Delayus 20
	cbi PORTD, 7
	Delayus 1000
	sbi PORTD, 7
	Delayus 1000
	ret
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
SendCommandToST7920:	;command is located in r16
	cbi PORTD, 6	;set low on RS pin
	cbi PORTD, 5	;set low on R/W pin
	Delayus 2	;delay 2 us
	sbi PORTD, 4	;set high on E pin
	out PORTC, r16	;set data
	Delayus 2	;delay 1 us
	cbi PORTD, 4	;set low on E pin
	Delayus 50	;delay 50 us
	ret
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
SendDataToST7920:	;data byte is located in r16
	sbi PORTD, 6	;set high on RS pin
	cbi PORTD, 5	;set low on R/W pin
	Delayus 2	;delay 1 us
	sbi PORTD, 4	;set high on E pin
	out PORTC, r16	;set data
	Delayus 2	;delay 1 us
	cbi PORTD, 4	;set low on E pin
	Delayus 50	;delay 50 us
	ret
/*-------------------------------------------------------------------*/



/*-------------------------------------------------------------------*/
InitST7920InTextMode:
	Delayus 20000		;
	Delayus 20000		;wait until power will rich Vdd
	call ResetDisplay	;reset display
	ldi r16, 0b00110000	;8 bit mode with basic command set
	call SendCommandToST7920;
	Delayus 120		;delay 120 us
	ldi r16, 0b00110000	;8 bit mode with basic command set
	call SendCommandToST7920;
	Delayus 50		;delay 50 us
	ldi r16, 0b00001100	;display on, no cursor
	call SendCommandToST7920;
	Delayus 120		;delay 120 us
	ldi r16, 0b00000001	;clear display screen
	call SendCommandToST7920;
	Delayus 20000		;delay 20 ms
	ldi r16, 0b00000110	;cursor moving right
	call SendCommandToST7920;
	Delayus 20000		;delay 20 ms
	ret
/*-------------------------------------------------------------------*/













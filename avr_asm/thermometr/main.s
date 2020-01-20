.equ PB0, 0
.equ PB1, 1
.equ PB2, 2
.equ PB3, 3
.equ PB4, 4
.equ PB5, 5

.equ SRCLK, PB2	;output
.equ RCLK , PB1	;output
.equ SER  , PB0	;output

.equ ADCSRA, 0x06
.equ ADMUX, 0x07
.equ DDRB , 0x17
.equ PORTB, 0x18
.equ TCCR0A, 0x2f
.equ TCCR0B, 0x33
.equ OCR0A, 0x36
.equ TIMSK0, 0x39
.equ SPL, 0x3d

.equ RAMEND, 0x9f

.equ D1, 0x01
.equ D2, 0x02
.equ D3, 0x04
.equ D4, 0x08

.equ OFF, 0x00
.equ A, 0x01	;0
.equ B, 0x02	;1
.equ C, 0x04	;2
.equ D, 0x08	;3
.equ E, 0x10	;4
.equ F, 0x20	;5
.equ G, 0x40	;6
.equ DP,0x80	;7

.equ  ZERO, A|B|C|D|E|F
.equ   ONE, B|C
.equ   TWO, A|B|D|E|G
.equ THREE, A|B|C|D|G
.equ  FOUR, B|C|F|G
.equ  FIVE, A|C|D|F|G
.equ   SIX, A|C|D|E|F|G
.equ SEVEN, A|B|C
.equ EIGHT, A|B|C|D|E|F|G
.equ  NINE, A|B|C|D|F|G
.equ MINUS, G
.equ   DEG, A|B|F|G
.equ EMPTY, OFF

.text
.org 0x00
;interrupt vector table
	rjmp RESET	;RESET
	rjmp RESET	;INT0
	rjmp RESET	;PCINT0
	rjmp RESET	;TIM0_OVF
	rjmp RESET	;EE_RDY
	rjmp RESET	;ANA_COMP
	rjmp TIM0_COMPA	;TIM0_COMPA
	rjmp RESET	;TIM0_COMPB
	rjmp RESET	;WDT
	rjmp RESET	;ADC

RESET:
	;Set stack, possibly not needed to init
	ldi r16, lo8(RAMEND)
	out SPL, r16		;set stack

	;init PORTB
	ldi r16, (1 << SRCLK) | (1 << RCLK) | (1 << SER)
	out DDRB, r16

	;init Timer0
	ldi r16, 0x02
	out TCCR0A, r16
	ldi r16, 0x04	;set prescaler to 1/256
	out TCCR0B, r16
	ldi r16, 163
	out OCR0A, r16	;compare interrupt freq = 60*4
	ldi r16, 0x04	;enable T0 COMPA interrupt
	out TIMSK0, r16

	;init ADC
	ldi r16, 0x03	;use ADC3, Vref=VCC, data right adjusted
	out ADMUX, r16
	;ldi r16,

	ldi r16, EMPTY
	ldi r17, TWO
	ldi r18, ONE|DP
	ldi r19, NINE
	sei
loop:
ADC_loop:
	;TODO
	rjmp ADC_loop
	rjmp loop

TIM0_COMPA:
Dig_1:
	cpi r20, D1
	brne Dig_2
	mov r0, r16
	mov r1, r20
	ldi r20, D2
	rjmp T0_COMPA_exit
Dig_2:
	cpi r20, D2
	brne Dig_3
	mov r0, r17
	mov r1, r20
	ldi r20, D3
	rjmp T0_COMPA_exit
Dig_3:
	cpi r20, D3
	brne Dig_4
	mov r0, r18
	mov r1, r20
	ldi r20, D4
	rjmp T0_COMPA_exit
Dig_4:
	mov r0, r19
	mov r1, r20
	ldi r20, D1

T0_COMPA_exit:
	rcall Send_Data
	reti



;FUNCTIONS
;
;send data to SN74HC595N
;data located in r0:r1
Send_Data:
	push r16
	cbi PORTB, RCLK		;activate shift register
	ldi r16, 16
SD_loop:
	lsl r0	;left shift C = b7, b0 = 0
	rol r1	;left shift b0 = C, C = b7
	cbi PORTB, SRCLK
	brcs SD_set_ser_high
SD_set_ser_low:
	cbi PORTB, SER
	rjmp SD_make_clock
SD_set_ser_high:
	sbi PORTB, SER
SD_make_clock:
	sbi PORTB, SRCLK
SD_loop_exit_condition:	
	dec r16
	brne SD_loop
SD_exit:
	sbi PORTB, RCLK	;by pos. edge data is in output
	pop r16
	ret

;тестовая программа для ЖК дисплея ST7920
;контроллер ATMega16 F_CPU = 12 MHz (внешний резонатор)
;распиновка ST7920:
; 1 - GND		земля
; 2 - VCC		питание +5В
; 3 - V0		настройка контрастности
; 4 - RS		режим передачи данных: 1 - данные, 0 - команда
; 5 - R/W		запись или чтение: 1 - чтение, 0 - запись
; 6 - E			строб
; 7:14 - DB0:7		шина данных
; 15 - PSB		протокол передачи: 1 - 8/4 бит шина, 0 - SPI
; 16 - NC		не подсоединён
; 17 - RST		сброс
; 18 - VOUT		выход удвоителя напряжения
; 19 - BLA		анод подсветки
; 20 - BLK		катод подсветки
;
;подключение контактов на схеме
;----ST7920--------ATMega16----
;    RS(4)	    PD6(20)
;   R/W(5)	    PD5(19)
;     E(6)	    PD4(18)
; DB0:7(7:14)     PC0:7(22:29)
;   RST(17)	    PD7(21)

.include "ATMega16Def.s"


.org 0x00
;interrupt vectors initialisation
	jmp main	;External pin, power-on reset,
			;	brown-out reset, watchdog reset,
			;	JTAG AVR reset
	jmp main	;External interrupt request 0
	jmp main	;External interrupt request 1
	jmp main	;Timer/Counter2 compare match
	jmp main	;Timer/Counter2 overflow
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
	call InitST7920InTextMode

	ldi r17, 64
loop:
	mov r16, r17
	call SendDataToST7920
	dec r17
	brne loop

	call InfiniteLoop














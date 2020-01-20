	.file	"sensor.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	uart_init
	.type	uart_init, @function
uart_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(-122)
	out 0x20,r24
	ldi r24,lo8(24)
	out 0xa,r24
	ldi r24,lo8(2)
	out 0xb,r24
	out 0x20,__zero_reg__
	out 0x9,__zero_reg__
	ret
	.size	uart_init, .-uart_init
.global	uart_putc
	.type	uart_putc, @function
uart_putc:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L4:
	sbis 0xb,5
	rjmp .L4
	out 0xc,r24
	ret
	.size	uart_putc, .-uart_putc
.global	uart_getc
	.type	uart_getc, @function
uart_getc:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L8:
	sbis 0xb,7
	rjmp .L8
	in r24,0xc
	ret
	.size	uart_getc, .-uart_getc
.global	uart_puts
	.type	uart_puts, @function
uart_puts:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r28,r24
	movw r16,r24
	subi r17,-4
.L10:
	ld r24,Y+
	tst r24
	breq .L9
	call uart_putc
	cp r28,r16
	cpc r29,r17
	brne .L10
.L9:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	uart_puts, .-uart_puts
	.section	.rodata
.LC0:
	.byte	48
	.byte	49
	.byte	50
	.byte	51
	.byte	52
	.byte	53
	.byte	54
	.byte	55
	.byte	56
	.byte	57
	.byte	97
	.byte	98
	.byte	99
	.byte	100
	.byte	101
	.byte	102
	.text
.global	debug
	.type	debug, @function
debug:
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,16
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 16 */
/* stack size = 19 */
.L__stack_usage = 19
	mov r17,r24
	ldi r24,lo8(16)
	ldi r30,lo8(.LC0)
	ldi r31,hi8(.LC0)
	movw r26,r28
	adiw r26,1
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	mov r24,r17
	swap r24
	andi r24,lo8(15)
	ldi r30,lo8(1)
	ldi r31,0
	add r30,r28
	adc r31,r29
	add r30,r24
	adc r31,__zero_reg__
	ld r24,Z
	call uart_putc
	andi r17,lo8(15)
	ldi r30,lo8(1)
	ldi r31,0
	add r30,r28
	adc r31,r29
	add r30,r17
	adc r31,__zero_reg__
	ld r24,Z
	call uart_putc
	ldi r24,lo8(32)
/* epilogue start */
	adiw r28,16
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	jmp uart_putc
	.size	debug, .-debug
.global	spi_init
	.type	spi_init, @function
spi_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x17
	ori r24,lo8(-80)
	out 0x17,r24
	ldi r24,lo8(1)
	out 0xe,r24
	ldi r24,lo8(80)
	out 0xd,r24
	ret
	.size	spi_init, .-spi_init
.global	spi_speedup
	.type	spi_speedup, @function
spi_speedup:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(1)
	out 0xe,r24
	ldi r24,lo8(81)
	out 0xd,r24
	ret
	.size	spi_speedup, .-spi_speedup
.global	spi_slowdown
	.type	spi_slowdown, @function
spi_slowdown:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(1)
	out 0xe,r24
	ldi r24,lo8(82)
	out 0xd,r24
	ret
	.size	spi_slowdown, .-spi_slowdown
.global	spi_transaction
	.type	spi_transaction, @function
spi_transaction:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	out 0xf,r24
.L20:
	sbis 0xe,7
	rjmp .L20
	in r24,0xf
	ret
	.size	spi_transaction, .-spi_transaction
.global	receive_data_nRF24L01
	.type	receive_data_nRF24L01, @function
receive_data_nRF24L01:
	push r14
	push r15
	push r17
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 6 */
.L__stack_usage = 6
	mov r17,r24
	in r24,0x18
	cbi 0x18,3
	ldi r24,lo8(97)
	std Y+1,r25
	call spi_transaction
	mov r14,r17
	ldd r25,Y+1
	mov r15,r25
	ldi r17,lo8(32)
.L23:
	ldi r24,lo8(-1)
	call spi_transaction
	movw r30,r14
	st Z+,r24
	movw r14,r30
	subi r17,lo8(-(-1))
	brne .L23
	sbi 0x18,3
	ldi r31,lo8(5)
	1: dec r31
	brne 1b
	nop
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r15
	pop r14
	ret
	.size	receive_data_nRF24L01, .-receive_data_nRF24L01
.global	transmit_data_nRF24L01
	.type	transmit_data_nRF24L01, @function
transmit_data_nRF24L01:
	push r14
	push r15
	push r17
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 6 */
.L__stack_usage = 6
	mov r17,r24
	in r24,0x18
	cbi 0x18,3
	ldi r24,lo8(-96)
	std Y+1,r25
	call spi_transaction
	mov r14,r17
	ldd r25,Y+1
	mov r15,r25
	ldi r17,lo8(32)
.L26:
	movw r30,r14
	ld r24,Z+
	movw r14,r30
	call spi_transaction
	subi r17,lo8(-(-1))
	brne .L26
	sbi 0x18,3
	ldi r31,lo8(5)
	1: dec r31
	brne 1b
	nop
	sbi 0x18,0
	ldi r24,lo8(53)
	1: dec r24
	brne 1b
	nop
	cbi 0x18,0
	ldi r30,lo8(599)
	ldi r31,hi8(599)
	1: sbiw r30,1
	brne 1b
	rjmp .
	nop
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r15
	pop r14
	ret
	.size	transmit_data_nRF24L01, .-transmit_data_nRF24L01
.global	read_register_nRF24L01
	.type	read_register_nRF24L01, @function
read_register_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r25,0x18
	cbi 0x18,3
	call spi_transaction
	ldi r24,lo8(-1)
	call spi_transaction
	sbi 0x18,3
	ldi r25,lo8(5)
	1: dec r25
	brne 1b
	nop
	ret
	.size	read_register_nRF24L01, .-read_register_nRF24L01
.global	write_register_nRF24L01
	.type	write_register_nRF24L01, @function
write_register_nRF24L01:
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 3 */
.L__stack_usage = 3
	in r25,0x18
	cbi 0x18,3
	ori r24,lo8(32)
	std Y+1,r22
	call spi_transaction
	ldd r22,Y+1
	mov r24,r22
	call spi_transaction
	sbi 0x18,3
	ldi r24,lo8(5)
	1: dec r24
	brne 1b
	nop
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	write_register_nRF24L01, .-write_register_nRF24L01
.global	pwr_up_nRF24L01
	.type	pwr_up_nRF24L01, @function
pwr_up_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,0
	ldi r25,0
	call read_register_nRF24L01
	mov r22,r24
	ori r22,lo8(2)
	ldi r24,0
	ldi r25,0
	call write_register_nRF24L01
	ldi r24,lo8(5999)
	ldi r25,hi8(5999)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	pwr_up_nRF24L01, .-pwr_up_nRF24L01
.global	pwr_down_nRF24L01
	.type	pwr_down_nRF24L01, @function
pwr_down_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,0
	ldi r25,0
	call read_register_nRF24L01
	mov r22,r24
	andi r22,lo8(-3)
	ldi r24,0
	ldi r25,0
	jmp write_register_nRF24L01
	.size	pwr_down_nRF24L01, .-pwr_down_nRF24L01
.global	enter_receiver_mode_nRF24L01
	.type	enter_receiver_mode_nRF24L01, @function
enter_receiver_mode_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cbi 0x18,0
	ldi r24,0
	ldi r25,0
	call read_register_nRF24L01
	mov r22,r24
	ori r22,lo8(1)
	ldi r24,0
	ldi r25,0
	call write_register_nRF24L01
	sbi 0x18,0
	ldi r24,lo8(599)
	ldi r25,hi8(599)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	enter_receiver_mode_nRF24L01, .-enter_receiver_mode_nRF24L01
.global	enter_transmitter_mode_nRF24L01
	.type	enter_transmitter_mode_nRF24L01, @function
enter_transmitter_mode_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cbi 0x18,0
	ldi r24,0
	ldi r25,0
	call read_register_nRF24L01
	mov r22,r24
	andi r22,lo8(-2)
	ldi r24,0
	ldi r25,0
	jmp write_register_nRF24L01
	.size	enter_transmitter_mode_nRF24L01, .-enter_transmitter_mode_nRF24L01
.global	init_nRF24L01
	.type	init_nRF24L01, @function
init_nRF24L01:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x17
	ori r24,lo8(9)
	out 0x17,r24
	sbi 0x18,3
	cbi 0x18,0
	ldi r22,lo8(32)
	ldi r24,lo8(17)
	ldi r25,0
	call write_register_nRF24L01
	call pwr_up_nRF24L01
	call enter_receiver_mode_nRF24L01
	ldi r24,lo8(7)
	ldi r25,0
	call read_register_nRF24L01
	sbrs r24,4
	rjmp .L34
	mov r22,r24
	ori r22,lo8(16)
	rjmp .L37
.L34:
	sbrs r24,5
	rjmp .L35
	mov r22,r24
	ori r22,lo8(32)
	rjmp .L37
.L35:
	sbrs r24,6
	rjmp .L33
	mov r22,r24
	ori r22,lo8(64)
.L37:
	ldi r24,lo8(7)
	ldi r25,0
	call write_register_nRF24L01
	ldi r22,0
	ldi r24,lo8(-31)
	ldi r25,0
	call write_register_nRF24L01
	ldi r22,0
	ldi r24,lo8(-30)
	ldi r25,0
	jmp write_register_nRF24L01
.L33:
	ret
	.size	init_nRF24L01, .-init_nRF24L01
.global	write_to_eeprom
	.type	write_to_eeprom, @function
write_to_eeprom:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L40:
	sbic 0x1c,1
	rjmp .L40
	out 0x1f,__zero_reg__
	out 0x1e,r24
	out 0x1d,r22
	sbi 0x1c,2
	sbi 0x1c,1
	ret
	.size	write_to_eeprom, .-write_to_eeprom
.global	read_from_eeprom
	.type	read_from_eeprom, @function
read_from_eeprom:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L43:
	sbic 0x1c,1
	rjmp .L43
	out 0x1f,__zero_reg__
	out 0x1e,r24
	sbi 0x1c,0
	in r24,0x1d
	ret
	.size	read_from_eeprom, .-read_from_eeprom
.global	init_LED_indicator
	.type	init_LED_indicator, @function
init_LED_indicator:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x11
	ori r24,lo8(-16)
	out 0x11,r24
	in r24,0x1a
	ldi r24,lo8(-1)
	out 0x1a,r24
	ret
	.size	init_LED_indicator, .-init_LED_indicator
.global	LED_indicator
	.type	LED_indicator, @function
LED_indicator:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r25,i.1787
	cpse r25,__zero_reg__
	rjmp .L46
	in r25,0x12
	andi r25,lo8(15)
	out 0x12,r25
	ldi r22,lo8(100)
	call __udivmodqi4
	mov r24,r25
	ldi r22,lo8(10)
	call __udivmodqi4
	mov r30,r24
	ldi r31,0
	lsl r30
	rol r31
	subi r30,lo8(-(digits))
	sbci r31,hi8(-(digits))
	ld r24,Z
	out 0x1b,r24
	sbi 0x12,7
	rjmp .L47
.L46:
	cpi r25,lo8(1)
	brne .L48
	in r25,0x12
	andi r25,lo8(15)
	out 0x12,r25
	ldi r22,lo8(100)
	call __udivmodqi4
	mov r24,r25
	ldi r22,lo8(10)
	call __udivmodqi4
	mov r30,r25
	ldi r31,0
	lsl r30
	rol r31
	subi r30,lo8(-(digits))
	sbci r31,hi8(-(digits))
	ld r24,Z
	out 0x1b,r24
	sbi 0x1b,5
	sbi 0x12,6
	rjmp .L47
.L48:
	cpi r25,lo8(2)
	brne .L49
	in r25,0x12
	andi r25,lo8(15)
	out 0x12,r25
	mov r24,r22
	ldi r22,lo8(100)
	call __udivmodqi4
	mov r24,r25
	ldi r22,lo8(10)
	call __udivmodqi4
	mov r30,r24
	ldi r31,0
	lsl r30
	rol r31
	subi r30,lo8(-(digits))
	sbci r31,hi8(-(digits))
	ld r24,Z
	out 0x1b,r24
	sbi 0x12,5
	rjmp .L47
.L49:
	cpi r25,lo8(3)
	brne .L47
	in r25,0x12
	andi r25,lo8(15)
	out 0x12,r25
	mov r24,r22
	ldi r22,lo8(100)
	call __udivmodqi4
	mov r24,r25
	ldi r22,lo8(10)
	call __udivmodqi4
	mov r30,r25
	ldi r31,0
	lsl r30
	rol r31
	subi r30,lo8(-(digits))
	sbci r31,hi8(-(digits))
	ld r24,Z
	out 0x1b,r24
	sbi 0x12,4
.L47:
	lds r24,i.1787
	subi r24,lo8(-(1))
	cpi r24,lo8(4)
	brsh .L50
	sts i.1787,r24
	ret
.L50:
	sts i.1787,__zero_reg__
	ret
	.size	LED_indicator, .-LED_indicator
.global	init_interrupt_INT0
	.type	init_interrupt_INT0, @function
init_interrupt_INT0:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x35
	ori r24,lo8(2)
	out 0x35,r24
	in r24,0x3b
	ori r24,lo8(64)
	out 0x3b,r24
	ret
	.size	init_interrupt_INT0, .-init_interrupt_INT0
.global	init_interrupt_INT1
	.type	init_interrupt_INT1, @function
init_interrupt_INT1:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x35
	ori r24,lo8(8)
	out 0x35,r24
	in r24,0x3b
	ori r24,lo8(-128)
	out 0x3b,r24
	ret
	.size	init_interrupt_INT1, .-init_interrupt_INT1
.global	init_interrupt_INT2
	.type	init_interrupt_INT2, @function
init_interrupt_INT2:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x34
	ori r24,lo8(64)
	out 0x34,r24
	in r24,0x3b
	ori r24,lo8(32)
	out 0x3b,r24
	ret
	.size	init_interrupt_INT2, .-init_interrupt_INT2
.global	init_interrupt_TIMER0
	.type	init_interrupt_TIMER0, @function
init_interrupt_TIMER0:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	out 0x33,__zero_reg__
	ldi r24,lo8(101)
	out 0x3c,r24
	in r24,0x33
	ori r24,lo8(10)
	out 0x33,r24
	in r24,0x39
	ori r24,lo8(2)
	out 0x39,r24
	ret
	.size	init_interrupt_TIMER0, .-init_interrupt_TIMER0
.global	__vector_18
	.type	__vector_18, @function
__vector_18:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
	push r25
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L57
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L57:
	in r24,0x3b
	ldi r25,lo8(-64)
	eor r24,r25
	out 0x3b,r24
	lds r24,dt
	ldi r25,lo8(1)
	eor r24,r25
	sts dt,r24
	lds r24,game_not_stopped
	eor r24,r25
	sts game_not_stopped,r24
	sts read_next_byte,__zero_reg__
/* epilogue start */
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_18, .-__vector_18
.global	init_start_game
	.type	init_start_game, @function
init_start_game:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r24,0
	call read_from_eeprom
	mov r29,r24
	ldi r24,lo8(1)
	call read_from_eeprom
	mov r28,r24
	ldi r24,lo8(2)
	call read_from_eeprom
	mov r16,r24
	ldi r24,lo8(3)
	call read_from_eeprom
	mov r17,r24
	ldi r24,lo8(4)
	call read_from_eeprom
	sts seed,r24
	cpi r29,lo8(-1)
	breq .L59
	cpi r28,lo8(-1)
	brne .L60
.L59:
	ldi r22,lo8(10)
	ldi r24,0
	call write_to_eeprom
	ldi r22,lo8(10)
	ldi r24,lo8(1)
	call write_to_eeprom
	ldi r24,lo8(10)
	sts max_count1,r24
	sts max_count2,r24
	rjmp .L58
.L60:
	sts max_count1,r29
	sts max_count2,r28
	sts best_goal_min,r16
	sts best_goal_sec,r17
.L58:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	init_start_game, .-init_start_game
.global	random
	.type	random, @function
random:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,0
	ret
	.size	random, .-random
.global	init_flash
	.type	init_flash, @function
init_flash:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x17,4
	sbi 0x18,4
	ret
	.size	init_flash, .-init_flash
.global	play_sound
	.type	play_sound, @function
play_sound:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(1)
	sts sound_name,r24
	sts read_next_byte,r24
	ret
	.size	play_sound, .-play_sound
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	lds r24,count1
	subi r24,lo8(-(1))
	sts count1,r24
	lds r25,total_sec
	lds r24,old_goal_sec
	cp r24,r25
	brsh .L69
	mov r18,r25
	sub r18,r24
	mov r24,r18
	rjmp .L76
.L69:
	sub r24,r25
.L76:
	sts goal_sec,r24
	lds r24,total_min
	lds r18,old_goal_min
	cp r18,r24
	brsh .L71
	mov r19,r24
	sub r19,r18
	mov r18,r19
	rjmp .L77
.L71:
	sub r18,r24
.L77:
	sts goal_min,r18
	sts old_goal_sec,r25
	sts old_goal_min,r24
	lds r22,goal_min
	lds r24,best_goal_min
	cp r24,r22
	brlo .L73
	lds r24,goal_sec
	lds r25,best_goal_sec
	cp r24,r25
	brsh .L73
	sts best_goal_min,r22
	sts best_goal_sec,r24
	ldi r24,lo8(2)
	call write_to_eeprom
	lds r22,best_goal_sec
	ldi r24,lo8(3)
	call write_to_eeprom
.L73:
	call play_sound
	lds r25,count1
	lds r24,max_count1
	cpse r25,r24
	rjmp .L68
	lds r24,game_ended
	cpse r24,__zero_reg__
	rjmp .L68
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L75
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L75:
	in r24,0x3b
	ldi r25,lo8(-64)
	eor r24,r25
	out 0x3b,r24
	lds r24,dt
	ldi r25,lo8(1)
	eor r24,r25
	sts dt,r24
	sts game_ended,r25
	sts game_not_stopped,__zero_reg__
.L68:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_1, .-__vector_1
.global	__vector_2
	.type	__vector_2, @function
__vector_2:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	lds r24,count2
	subi r24,lo8(-(1))
	sts count2,r24
	lds r25,total_sec
	lds r24,old_goal_sec
	cp r24,r25
	brsh .L79
	mov r18,r25
	sub r18,r24
	mov r24,r18
	rjmp .L86
.L79:
	sub r24,r25
.L86:
	sts goal_sec,r24
	lds r24,total_min
	lds r18,old_goal_min
	cp r18,r24
	brsh .L81
	mov r19,r24
	sub r19,r18
	mov r18,r19
	rjmp .L87
.L81:
	sub r18,r24
.L87:
	sts goal_min,r18
	sts old_goal_sec,r25
	sts old_goal_min,r24
	lds r22,goal_min
	lds r24,best_goal_min
	cp r24,r22
	brlo .L83
	lds r24,goal_sec
	lds r25,best_goal_sec
	cp r24,r25
	brsh .L83
	sts best_goal_min,r22
	sts best_goal_sec,r24
	ldi r24,lo8(2)
	call write_to_eeprom
	lds r22,best_goal_sec
	ldi r24,lo8(3)
	call write_to_eeprom
.L83:
	call play_sound
	lds r25,count2
	lds r24,max_count2
	cpse r25,r24
	rjmp .L78
	lds r24,game_ended
	cpse r24,__zero_reg__
	rjmp .L78
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L85
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L85:
	in r24,0x3b
	ldi r25,lo8(-64)
	eor r24,r25
	out 0x3b,r24
	lds r24,dt
	ldi r25,lo8(1)
	eor r24,r25
	sts dt,r24
	sts game_ended,r25
	sts game_not_stopped,__zero_reg__
.L78:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_2, .-__vector_2
.global	get_bytes
	.type	get_bytes, @function
get_bytes:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r30,r20
	movw r26,r18
	st Z,__zero_reg__
	st X,__zero_reg__
	movw r28,r16
	st Y,r22
	ldi r20,6
	1:
	lsr r23
	ror r22
	dec r20
	brne 1b
	mov r19,r24
	lsl r19
	lsl r19
	or r19,r22
	ld r18,X
	or r18,r19
	st X,r18
	lsr r25
	ror r24
	lsr r25
	ror r24
	ld r25,Z
	or r25,r24
	st Z,r25
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	get_bytes, .-get_bytes
.global	decode_data_and_execute
	.type	decode_data_and_execute, @function
decode_data_and_execute:
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 10 */
.L__stack_usage = 10
	movw r14,r24
	movw r30,r24
	ld r17,Z
	cpse r17,__zero_reg__
	rjmp .L90
	lds r24,count1
	std Z+1,r24
	lds r24,count2
	std Z+2,r24
	lds r24,total_min
	std Z+3,r24
	lds r24,total_sec
	std Z+4,r24
	lds r24,old_goal_min
	std Z+5,r24
	lds r24,old_goal_sec
	std Z+6,r24
	rjmp .L130
.L90:
	cpi r17,lo8(1)
	brne .L92
	movw r30,r24
	ldd r22,Z+1
	tst r22
	brne .+2
	rjmp .L106
	ldd r24,Z+2
	tst r24
	brne .+2
	rjmp .L106
	sts max_count1,r22
	sts max_count2,r24
	ldi r24,0
	call write_to_eeprom
	lds r22,max_count2
	ldi r24,lo8(1)
	call write_to_eeprom
	rjmp .L91
.L92:
	cpi r17,lo8(2)
	brne .L95
	movw r30,r24
	ldd r19,Z+3
	ldd r25,Z+5
	ldd r18,Z+4
	ldd r24,Z+6
	cp r25,r19
	brlo .L96
	cp r18,r24
	brsh .+2
	rjmp .L106
	rjmp .L130
.L96:
	movw r30,r14
	ldd r20,Z+1
	sts count1,r20
	ldd r20,Z+2
	sts count2,r20
	sts total_min,r19
	sts total_sec,r18
	sts old_goal_min,r25
	sts old_goal_sec,r24
	rjmp .L130
.L95:
	cpi r17,lo8(3)
	brne .L97
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L98
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L98:
	in r24,0x3b
	andi r24,lo8(63)
	out 0x3b,r24
	sts dt,__zero_reg__
	sts game_not_stopped,__zero_reg__
	sts read_next_byte,__zero_reg__
	rjmp .L130
.L97:
	cpi r17,lo8(4)
	brne .L99
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L100
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L100:
	in r24,0x3b
	ori r24,lo8(-64)
	out 0x3b,r24
	ldi r24,lo8(1)
	sts dt,r24
	sts game_not_stopped,r24
	rjmp .L130
.L99:
	cpi r17,lo8(5)
	brne .L101
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L102
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L102:
	in r24,0x3b
	andi r24,lo8(63)
	out 0x3b,r24
	sts dt,__zero_reg__
	sts game_not_stopped,__zero_reg__
	sts count1,__zero_reg__
	sts count2,__zero_reg__
	sts total_min,__zero_reg__
	sts total_sec,__zero_reg__
	sts old_goal_min,__zero_reg__
	sts old_goal_sec,__zero_reg__
	sts game_ended,__zero_reg__
	rjmp .L130
.L101:
	cpi r17,lo8(-1)
	brne .+2
	rjmp .L116
	cpi r17,lo8(7)
	breq .+2
	rjmp .L103
	lds r24,game_not_stopped
	tst r24
	breq .L104
	in r24,0x3b
	andi r24,lo8(-64)
	brne .L105
	in r24,0x3a
	ori r24,lo8(-64)
	out 0x3a,r24
.L105:
	in r24,0x3b
	andi r24,lo8(63)
	out 0x3b,r24
	sts dt,__zero_reg__
	sts game_not_stopped,__zero_reg__
	sts read_next_byte,__zero_reg__
.L104:
	movw r30,r14
	ldd r24,Z+1
	cpi r24,lo8(16)
	brlo .+2
	rjmp .L106
	in r25,0x18
	lds r25,current.1928
	cp r24,r25
	breq .L107
	sts current.1928,r24
	sts offset.1927+1,__zero_reg__
	sts offset.1927,__zero_reg__
	sts page.1926,__zero_reg__
	sts page.1926+1,r24
	ldi r24,lo8(1)
	sts continue_write.1929,r24
.L107:
	lds r24,continue_write.1929
	tst r24
	brne .+2
	rjmp .L130
	lds r22,offset.1927
	lds r23,offset.1927+1
	movw r16,r28
	subi r16,-1
	sbci r17,-1
	movw r18,r28
	subi r18,-2
	sbci r19,-1
	movw r20,r28
	subi r20,-3
	sbci r21,-1
	ldi r24,0
	ldi r25,0
	call get_bytes
	cbi 0x18,4
	ldi r24,lo8(-124)
	call spi_transaction
	ldd r24,Y+3
	call spi_transaction
	ldd r24,Y+2
	call spi_transaction
	ldd r24,Y+1
	call spi_transaction
	ldi r24,lo8(2)
	sts i.1924,r24
	ldi r23,lo8(5)
	mov r13,r23
.L109:
	lds r24,i.1924
	cpi r24,lo8(32)
	brlo .+2
	rjmp .L114
	lds r25,continue_write.1929
	tst r25
	brne .+2
	rjmp .L114
	movw r30,r14
	add r30,r24
	adc r31,__zero_reg__
	ld r24,Z
	call spi_transaction
	lds r24,offset.1927
	lds r25,offset.1927+1
	adiw r24,1
	sts offset.1927+1,r25
	sts offset.1927,r24
	cpi r24,16
	sbci r25,2
	breq .+2
	rjmp .L110
	sts offset.1927+1,__zero_reg__
	sts offset.1927,__zero_reg__
	sbi 0x18,4
	ldi r18,lo8(53)
	1: dec r18
	brne 1b
	nop
	cbi 0x18,4
	lds r22,offset.1927
	lds r23,offset.1927+1
	movw r16,r28
	subi r16,-1
	sbci r17,-1
	movw r18,r28
	subi r18,-2
	sbci r19,-1
	movw r20,r28
	subi r20,-3
	sbci r21,-1
	lds r24,page.1926
	lds r25,page.1926+1
	call get_bytes
	ldi r24,lo8(-125)
	call spi_transaction
	ldd r24,Y+3
	call spi_transaction
	ldd r24,Y+2
	call spi_transaction
	ldd r24,Y+1
	call spi_transaction
	sbi 0x18,4
	ldi r24,lo8(53)
	1: dec r24
	brne 1b
	nop
	cbi 0x18,4
	ldi r24,lo8(-41)
	call spi_transaction
.L112:
	ldi r24,lo8(-1)
	call spi_transaction
	sbrs r24,7
	rjmp .L112
	sbi 0x18,4
	lds r24,page.1926
	lds r25,page.1926+1
	adiw r24,1
	sts page.1926+1,r25
	sts page.1926,r24
	lds r18,current.1928
	ldi r19,0
	subi r18,-1
	sbci r19,-1
	mov r19,r18
	clr r18
	cp r18,r24
	cpc r19,r25
	brsh .L113
	ldi r24,lo8(-1)
	movw r30,r14
	st Z,r24
	std Z+1,r13
	sts continue_write.1929,__zero_reg__
	rjmp .L110
.L113:
	ldi r31,lo8(53)
	1: dec r31
	brne 1b
	nop
	lds r22,offset.1927
	lds r23,offset.1927+1
	movw r16,r28
	subi r16,-1
	sbci r17,-1
	movw r18,r28
	subi r18,-2
	sbci r19,-1
	movw r20,r28
	subi r20,-3
	sbci r21,-1
	ldi r24,0
	ldi r25,0
	call get_bytes
	cbi 0x18,4
	ldi r24,lo8(-124)
	call spi_transaction
	ldd r24,Y+3
	call spi_transaction
	ldd r24,Y+2
	call spi_transaction
	ldd r24,Y+1
	call spi_transaction
.L110:
	lds r24,i.1924
	subi r24,lo8(-(1))
	sts i.1924,r24
	rjmp .L109
.L114:
	sbi 0x18,4
	movw r30,r14
	ld r24,Z
	cpi r24,lo8(-1)
	breq .L130
	ldi r24,lo8(-1)
	st Z,r24
	ldi r24,lo8(6)
	rjmp .L129
.L106:
	ldi r24,lo8(-1)
	movw r30,r14
	st Z,r24
	ldi r24,lo8(4)
	rjmp .L129
.L103:
	ldi r24,lo8(-1)
	movw r30,r14
	st Z,r24
	ldi r24,lo8(3)
.L129:
	std Z+1,r24
.L130:
	ldi r17,lo8(1)
	rjmp .L91
.L116:
	ldi r17,0
.L91:
	mov r24,r17
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	ret
	.size	decode_data_and_execute, .-decode_data_and_execute
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	call init_start_game
	call uart_init
	call spi_init
	call init_nRF24L01
	call init_flash
	call init_interrupt_INT0
	call init_interrupt_INT1
	call init_interrupt_INT2
	call init_interrupt_TIMER0
	call init_LED_indicator
	in r24,__SREG__
	ori r24,lo8(-128)
	out __SREG__,r24
.L135:
	call enter_receiver_mode_nRF24L01
.L133:
	sbic 0x16,1
	rjmp .L133
	ldi r24,lo8(7)
	ldi r25,0
	call read_register_nRF24L01
	sbrs r24,6
	rjmp .L135
	ldi r24,lo8(data)
	ldi r25,hi8(data)
	call receive_data_nRF24L01
	ldi r24,lo8(data)
	ldi r25,hi8(data)
	call decode_data_and_execute
	tst r24
	breq .L135
	ldi r24,lo8(7)
	ldi r25,0
	call read_register_nRF24L01
	mov r22,r24
	ori r22,lo8(64)
	ldi r24,lo8(7)
	ldi r25,0
	call write_register_nRF24L01
	ldi r22,0
	ldi r24,lo8(-30)
	ldi r25,0
	call write_register_nRF24L01
	call enter_transmitter_mode_nRF24L01
	ldi r29,lo8(100)
.L141:
	ldi r24,lo8(data)
	ldi r25,hi8(data)
	call transmit_data_nRF24L01
	ldi r24,lo8(-25537)
	ldi r25,hi8(-25537)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(7)
	ldi r25,0
	call read_register_nRF24L01
	mov r28,r24
	sbrs r24,4
	rjmp .L138
	ori r28,lo8(16)
	mov r22,r28
	ldi r24,lo8(7)
	ldi r25,0
	call write_register_nRF24L01
	ldi r22,0
	ldi r24,lo8(-31)
	ldi r25,0
	call write_register_nRF24L01
.L138:
	sbrc r28,5
	rjmp .L139
	subi r29,lo8(-(-1))
	brne .L141
.L140:
	ldi r22,0
	ldi r24,lo8(-31)
	ldi r25,0
	call write_register_nRF24L01
	rjmp .L135
.L139:
	mov r22,r28
	ori r22,lo8(32)
	ldi r24,lo8(7)
	ldi r25,0
	call write_register_nRF24L01
	rjmp .L140
	.size	main, .-main
	.text
.global	get_and_put_next_sound_byte
	.type	get_and_put_next_sound_byte, @function
get_and_put_next_sound_byte:
	push r16
	push r17
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 7 */
.L__stack_usage = 7
	lds r24,read_next_byte
	cpi r24,lo8(1)
	breq .+2
	rjmp .L149
	sbis 0x18,3
	rjmp .L149
	lds r25,sound_name
	lds r24,last_sound_name.1949
	cp r25,r24
	breq .L153
	sts last_sound_name.1949,r25
	sts page.1953,__zero_reg__
	sts page.1953+1,r25
	sts offset.1954+1,__zero_reg__
	sts offset.1954,__zero_reg__
	movw r16,r28
	subi r16,-3
	sbci r17,-1
	movw r18,r28
	subi r18,-2
	sbci r19,-1
	movw r20,r28
	subi r20,-1
	sbci r21,-1
	ldi r22,0
	ldi r23,0
	ldi r24,0
	call get_bytes
	cbi 0x18,4
	ldi r24,lo8(3)
	call spi_transaction
	ldd r24,Y+1
	call spi_transaction
	ldd r24,Y+2
	call spi_transaction
	ldd r24,Y+3
	call spi_transaction
	ldi r24,lo8(-1)
	call spi_transaction
	out 0x15,r24
	lds r24,offset.1954
	lds r25,offset.1954+1
	adiw r24,1
	sts offset.1954+1,r25
	sts offset.1954,r24
	rjmp .L158
.L153:
	lds r22,offset.1954
	lds r23,offset.1954+1
	subi r22,-1
	sbci r23,-1
	sts offset.1954+1,r23
	sts offset.1954,r22
	lds r24,page.1953
	lds r25,page.1953+1
	cpi r22,16
	ldi r18,2
	cpc r23,r18
	brsh .L154
	movw r16,r28
	subi r16,-3
	sbci r17,-1
	movw r18,r28
	subi r18,-2
	sbci r19,-1
	movw r20,r28
	subi r20,-1
	sbci r21,-1
	call get_bytes
	cbi 0x18,4
	ldi r24,lo8(3)
	call spi_transaction
	ldd r24,Y+1
	call spi_transaction
	ldd r24,Y+2
	call spi_transaction
	ldd r24,Y+3
	call spi_transaction
	ldi r24,lo8(-1)
	call spi_transaction
	out 0x15,r24
.L158:
	sbi 0x18,4
	rjmp .L149
.L154:
	sts offset.1954+1,__zero_reg__
	sts offset.1954,__zero_reg__
	adiw r24,1
	sts page.1953+1,r25
	sts page.1953,r24
	lds r24,pagecount.1955
	lds r25,pagecount.1955+1
	adiw r24,1
	sts pagecount.1955+1,r25
	sts pagecount.1955,r24
	cpi r24,-1
	cpc r25,__zero_reg__
	breq .L149
	brlo .L149
	sts pagecount.1955+1,__zero_reg__
	sts pagecount.1955,__zero_reg__
	sts read_next_byte,__zero_reg__
.L149:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	get_and_put_next_sound_byte, .-get_and_put_next_sound_byte
.global	__vector_19
	.type	__vector_19, @function
__vector_19:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	lds r24,dt_count
	lds r25,dt_count+1
	adiw r24,1
	sts dt_count+1,r25
	sts dt_count,r24
	cpi r24,44
	sbci r25,86
	brlo .L161
	lds r24,dt
	cpi r24,lo8(1)
	brne .L162
	lds r24,total_sec
	subi r24,lo8(-(1))
	sts total_sec,r24
.L162:
	sts dt_count+1,__zero_reg__
	sts dt_count,__zero_reg__
	lds r24,global_sec
	lds r25,global_sec+1
	movw r18,r24
	subi r18,-1
	sbci r19,-1
	sts global_sec+1,r19
	sts global_sec,r18
	sbiw r24,31
	brlo .L161
	sts global_sec+1,__zero_reg__
	sts global_sec,__zero_reg__
.L161:
	lds r24,total_sec
	cpi r24,lo8(60)
	brne .L164
	lds r24,dt
	cpi r24,lo8(1)
	brne .L164
	lds r24,total_min
	subi r24,lo8(-(1))
	sts total_min,r24
	sts total_sec,__zero_reg__
.L164:
	lds r24,global_sec
	lds r25,global_sec+1
	ldi r22,lo8(19)
	ldi r23,0
	call __udivmodhi4
	sbiw r24,5
	brsh .L165
	lds r22,total_sec
	lds r24,total_min
	rjmp .L167
.L165:
	lds r22,count2
	lds r24,count1
.L167:
	call LED_indicator
	call get_and_put_next_sound_byte
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_19, .-__vector_19
	.local	pagecount.1955
	.comm	pagecount.1955,2,1
	.local	offset.1954
	.comm	offset.1954,2,1
	.local	page.1953
	.comm	page.1953,2,1
	.data
	.type	last_sound_name.1949, @object
	.size	last_sound_name.1949, 1
last_sound_name.1949:
	.byte	-1
	.local	i.1924
	.comm	i.1924,1,1
	.type	continue_write.1929, @object
	.size	continue_write.1929, 1
continue_write.1929:
	.byte	1
	.local	page.1926
	.comm	page.1926,2,1
	.local	offset.1927
	.comm	offset.1927,2,1
	.type	current.1928, @object
	.size	current.1928, 1
current.1928:
	.byte	-1
	.local	i.1787
	.comm	i.1787,1,1
.global	value2
	.section .bss
	.type	value2, @object
	.size	value2, 1
value2:
	.zero	1
.global	value1
	.type	value1, @object
	.size	value1, 1
value1:
	.zero	1
.global	read_next_byte
	.type	read_next_byte, @object
	.size	read_next_byte, 1
read_next_byte:
	.zero	1
	.comm	sound_name,1,1
	.comm	seed,1,1
.global	game_not_stopped
	.data
	.type	game_not_stopped, @object
	.size	game_not_stopped, 1
game_not_stopped:
	.byte	1
.global	game_ended
	.section .bss
	.type	game_ended, @object
	.size	game_ended, 1
game_ended:
	.zero	1
	.comm	global_sec,2,1
	.comm	max_count2,1,1
	.comm	max_count1,1,1
.global	dt
	.data
	.type	dt, @object
	.size	dt, 1
dt:
	.byte	1
.global	old_goal_min
	.section .bss
	.type	old_goal_min, @object
	.size	old_goal_min, 1
old_goal_min:
	.zero	1
.global	old_goal_sec
	.type	old_goal_sec, @object
	.size	old_goal_sec, 1
old_goal_sec:
	.zero	1
.global	best_goal_sec
	.data
	.type	best_goal_sec, @object
	.size	best_goal_sec, 1
best_goal_sec:
	.byte	-1
.global	best_goal_min
	.type	best_goal_min, @object
	.size	best_goal_min, 1
best_goal_min:
	.byte	-1
.global	goal_min
	.section .bss
	.type	goal_min, @object
	.size	goal_min, 1
goal_min:
	.zero	1
.global	goal_sec
	.type	goal_sec, @object
	.size	goal_sec, 1
goal_sec:
	.zero	1
.global	total_sec
	.type	total_sec, @object
	.size	total_sec, 1
total_sec:
	.zero	1
.global	total_min
	.type	total_min, @object
	.size	total_min, 1
total_min:
	.zero	1
.global	dt_count
	.type	dt_count, @object
	.size	dt_count, 2
dt_count:
	.zero	2
.global	count2
	.type	count2, @object
	.size	count2, 1
count2:
	.zero	1
.global	count1
	.type	count1, @object
	.size	count1, 1
count1:
	.zero	1
	.comm	data,32,1
.global	digits
	.section	.rodata
	.type	digits, @object
	.size	digits, 20
digits:
	.word	215
	.word	17
	.word	205
	.word	93
	.word	27
	.word	94
	.word	222
	.word	21
	.word	223
	.word	95
	.ident	"GCC: (GNU) 4.8.2"
.global __do_copy_data
.global __do_clear_bss

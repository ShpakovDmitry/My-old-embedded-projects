;PRELIMINARY SETUP
[BITS 16]
[ORG 0X7C00]

COM1 equ 0x3F8

;BOOTLOADER CODE

;INIT COM PORT
	CALL InitCOMPort
	
	MOV SI, StartString
	CALL PrintString
	
	MOV AX, COM1
	MOV DX, AX
MAINLOOP:
	IN AL, DX
	CALL PrintCharacter
	;INC AL
	;CALL WriteCharacter

	JMP MAINLOOP

;SCREEN FUNCTIONS

PrintCharacter:
	MOV AH, 0X0E
	MOV BH, 0X00
	MOV BL, 0X07
	INT 0X10
	RET

PrintString:

next_character:
	MOV AL, [SI]
	OR AL, AL
	JZ exit_function
	CALL PrintCharacter
	INC SI
	JMP next_character

exit_function:
	RET


;SERIAL PORT FUNCTIONS
InitCOMPort:
	MOV AH, 0X00	;INIT PORT FUNCTION
	MOV AL, 0XE3	;9600 BPS, 8N1
	MOV DX, 0X00	;COM1
	RET
ReadCharacter:
	;MOV AH, 0X02
	;MOV AL, 0X00
	;MOV DX, 0X00
	;INT 0X14
	;RET

WriteCharacter:
	MOV AH, 0X01
	;MOV AL, 0xAA
	MOV DX, 0X00
	INT 0X14
	RET

;Delay function
Delay:			;Delay ~1 sec
	mov bp, 0xffff
	mov si, 0x00ff
delay2:
	dec bp
	nop
	jnz delay2
	dec si
	cmp si,0    
	jnz delay2
	ret

;Data block

StartString db 'Running programm... ', 0

;$ is current line, $$ is first line, db 0 is a 00000000 byte
;So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0

;Fill last two bytes(a word) with the MBR signature 0xAA55
DW 0xAA55




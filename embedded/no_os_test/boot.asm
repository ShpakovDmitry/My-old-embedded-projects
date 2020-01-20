;PRELIMINARY SETUP
[BITS 16]
[ORG 0X7C00]

;BOOTLOADER CODE

	MOV SI, HelloString
	CALL PrintString
	HLT

;SCREEN FUNCTIONS

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

PrintCharacter:
	MOV AH, 0X0E
	MOV BH, 0X00
	MOV BL, 0X07
	INT 0X10
	RET

;Data block

HelloString db 'Hello, World', 0

;$ is current line, $$ is first line, db 0 is a 00000000 byte
;So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0

;Fill last two bytes(a word) with the MBR signature 0xAA55
DW 0xAA55




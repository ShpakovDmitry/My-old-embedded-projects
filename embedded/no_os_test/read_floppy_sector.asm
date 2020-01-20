;PRELIMINARY SETUP
[BITS 16]
[ORG 0X7C00]

;BOOTLOADER CODE

	MOV SI, HelloString
	CALL ReadSector
	CALL PrintString
	HLT

;READ FUNCTION
ReadSector:
	PUSH ES
	PUSH BX
	MOV AH, 0x02 ;Read function
	MOV AL, 0x01 ;Sectors to read
	MOV CH, 0x00 ;Cylinder number
	MOV CL, 0x02 ;Sector number
	MOV DH, 0x00 ;Head number
	MOV DL, 0x00 ;Drive number
	XOR AX, AX		;
	MOV ES, AX		;Segment
	MOV BX, HelloString	;Offset
	INT 0x13
	POP BX
	POP ES
	RET

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


;$ is current line, $$ is first line, db 0 is a 00000000 byte
;So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0

;Fill last two bytes(a word) with the MBR signature 0xAA55
DW 0xAA55

HelloString db 'Hello, World', 0



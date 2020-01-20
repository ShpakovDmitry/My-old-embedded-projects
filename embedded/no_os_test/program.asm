;PRELIMINARY SETUP
[bits 16]
[org 0X7C00]

;BOOTLOADER CODE
	;call GetVideoMode
	;call PrintHexWord
	call GetSVGAInfo
	call InfiniteLoop

;SCREEN FUNCTIONS
PrintCharacter:		;character is located in 'al' register
	mov ah, 0x0E	;teletype function
	mov bh, 0x00	;page number
	;mov bl, 0x07	;light gray color
	mov bl, 0x04	;red color
	int 0x10	;
	ret		;

PrintHexByte:		;Hex number is located in al
	push ax
	and ax, 0x00f0
	shr ax, 0x0004
	mov si, HexData
	add si, ax
	mov al, [si]
	call PrintCharacter
	pop ax
	and ax, 0x000f
	mov si, HexData
	add si, ax
	mov al, [si]
	call PrintCharacter
	ret

PrintHexWord:		;Hex number is located in ax
	push ax
	and ax, 0xff00
	shr ax, 0x0008
	call PrintHexByte
	pop ax
	and ax, 0x00ff
	call PrintHexByte
	ret
	

;OTHER FUNCTIONS
InfiniteLoop:
	jmp InfiniteLoop

GetVideoMode:
	mov ah, 0x0f
	int 0x10
	ret

GetSVGAInfo:
	mov ax, 0x0000
	mov es, ax
	mov di, 0x7e00
	mov ax, 0x4f00
	int 0x10
	push ax
	call PrintHexWord
	pop ax
	mov ax, 0x0000
	mov cx, 0x0100
print:
	mov si, di
	add si, ax
	push ax
	mov al, [si]
	call PrintHexByte
	pop ax
	inc ax
	loop print
	ret

;Data block

HexData db '0123456789abcdef'

;$ is current line, $$ is first line, db 0 is a 00000000 byte
;So, pad the code with 0s until you reach 510 bytes
TIMES 510 - ($ - $$) DB 0

;Fill last two bytes(a word) with the MBR signature 0xAA55
DW 0xAA55




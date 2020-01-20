#define MAXLEN 1024

void uart_init(void);
void uart_putc(unsigned char c);
unsigned char uart_getc(void);

void uart_puts(unsigned char *s);
void debug(unsigned char c);




void uart_init(void)
{	
	// 8 бит данных, 1 стоп бит, без контроля чётности
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	
	// разрешить передачу и приём данных
	UCSRB = (1 << TXEN) | (1 << RXEN);
	
	// удвоенная скорость
	UCSRA = (1 << U2X);
	
	// настройка скорости обмена
		//UBRRH = 0x00;	UBRRL = 0x02;	//500000 бод; F_CPU=12 MHz
		//UBRRH = 0x02;	UBRRL = 0x70;	//2400 бод; F_CPU=12 MHz
	UBRRH = 0x00;	UBRRL = 0x00;	//1500000 бод; F_CPU=12 MHz
	//	........дополнить режимы........
	//------------------------------------------------F_CPU = 12 MHz----------------------------------------------
	//			U2X = 0				|			U2X = 1
	//baud rate	UBRRH		UBRRL	 error%		|	UBRRH		UBRRL	error%
	//     50	0x3a		0x97	  0.0	    v	|	0x75		0x2f	  0.0		v
	//     75	0x27		0x0f	  0.0	    v	|	0x4e		0x1f	  0.0		v
	//    110	0x1a		0xa1	 -0.0027	|	0x35		0x43	 -0.0027
	//    134	0x15		0xdc	 -0.00027	|	0x2b		0xb9	 -0.00027
	//    150	0x13		0x87	  0.0	    v	|	0x27		0x0f	  0.0		v
	//    200	0x0e		0xa5	  0.0	    v	|	0x1d		0x4b	  0.0		v
	//    300	0x09		0xc3	  0.0	    v	|	0x13		0x87	  0.0		v
	//    600	0x04		0xe1	  0.0	    v	|	0x09		0xc3	  0.0		v
	//   1200	0x02		0x70	  0.0	    v	|	0x04		0xe1	  0.0		v
	//   1800	0x01		0xa0	  0.08		|	0x03		0x40	 -0.04
	//   2400	0x01		0x38	  0.16		|	0x02		0x70	  0.0		v
	//   4800	0x00		0x9b	 -0.16		|	0x01		0x38	  0.16
	//   9600	0x00		0x4d	 -0.16		|	0x00		0x9b	 -0.16
	//  19200	0x00		0x26	 -0.16		|	0x00		0x4d	 -0.16
	//  38400	0x00		0x13	  2.4		|	0x00		0x27	  2.4
	//  57600	0x00		0x0c	 -0.16		|	0x00		0x19	 -0.16
	// 230400	0x00		0x02	 -7.84		|	0x00		0x06	  7.52
	// 460800	0x00		0x01	 22.88		|	0x00		0x02	 -7.84
	// 500000	0x00		0x01	 33.33		|	0x00		0x02	  0.0		v
	// 576000	0x00		0x00	-23.2		|	0x00		0x02	 15.2
	// 921600	0x00		0x00	 22.88		|	0x00		0x01	 22.88
	//1000000	----		----	--------	|	0x00		0x01	 33.33
	//1152000	----		----	--------	|	0x00		0x00	-23.2
	//1500000	----		----	--------	|	0x00		0x00	  0.0		v
	//2000000	----		----	--------	|	----		----

}

void uart_putc(unsigned char c)
{
	// ждём окончания передачи предыдущего байта
	while( (UCSRA & (1 << UDRE) ) == 0 )
		;
	// передача данных
	UDR = c;
}

unsigned char uart_getc(void)
{
	// ждём приёма данных
	while( (UCSRA & (1 << RXC) ) == 0 )
		;
	// считываем принятый байт
	return UDR;
}

void uart_puts(unsigned char *s)
{
	unsigned short int i;
	
	for(i = 0; (i < MAXLEN) && ( *s != '\0' ); i++)
	{
		uart_putc(*s);
		*(s++);
	}
}

void debug(unsigned char c)
{
	char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	uart_putc( hex[ (unsigned short int)c/16 ] ); uart_putc( hex[ (unsigned short int)c%16 ] ); uart_putc(' ');
}









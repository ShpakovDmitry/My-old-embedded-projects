void uart_init(void);
void uart_putc(char c);
unsigned char uart_getc(void);


void uart_init(void)
{
	// настройка скорости обмена
	
	//UBRRH = 0x04; UBRRL = 0xE1; // 50 бод
	//UBRRH = 0x02; UBRRL = 0x37; // 110 бод
	//UBRRH = 0x01; UBRRL = 0xf3; // 200 бод
	//UBRRH = 0x00; UBRRL = 0x67; // 600 бод
	//UBRRH = 0x00; UBRRL = 0x02; // 19200 бод
	//UBRRH = 0x00; UBRRL = 0x00; // 57600 бод
	UBRRH = 0x00; UBRRL = 0x02; // 500000 бод; 12 MHz; x2
	
	
	// 8 бит данных, 1 стоп бит, без контроля чётности
	UCSRC = ( 1 << URSEL ) | ( 1 << UCSZ1 ) | ( 1 << UCSZ0 );// | ( 1 << UPM1 ) | ( 1 << UPM0 );
	
	// разрешить приём и передачу данных
	UCSRB = ( 1 << TXEN ) | ( 1 << RXEN ) ;
	
	// удвоенная скорость
	UCSRA = (1 << U2X); // |(1 << UDRE)
}

void uart_putc(char c)
{
	// ждём окончания передачи предыдущего байта
	while( ( UCSRA & ( 1 << UDRE ) ) == 0 )
		;
	
	// передача данных
	UDR = c;
}

unsigned char uart_getc(void)
{
	// ждём приёма байта
	while( ( UCSRA & ( 1 << RXC ) ) == 0 )
		;
	
	// считываем принятый байт
	return UDR;
}

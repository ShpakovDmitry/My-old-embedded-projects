#include <avr/io.h>
#include <util/delay.h>

#include "spi_uart.h"


void main(void)
{
	unsigned char data, i;
	
	DDRB = (1 << PB0) | (1 << PB1) | (0 << PB2);
	PORTB |= (1 << PB0) | (0 << PB1);
	
	spi_init();
	uart_init();
	
	
	PORTB &= ~(1 << PB0);
	
	data = spi_transaction(0x00);
	data = spi_transaction(0xff);
	
	PORTB |= (1 << PB0);

	
	
	while (1)
	{
		
	}
	
}

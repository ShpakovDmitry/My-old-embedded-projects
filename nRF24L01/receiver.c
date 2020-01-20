#include <avr/io.h>
#include <util/delay.h>

#include "spi_uart.h"


void main(void)
{
	unsigned char data, i, data1;
	
	DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6);
	PORTD |= (0 << PD4) | (0 << PD5) | (0 << PD6);
	
	DDRB = (1 << PB0) | (1 << PB1) | (0 << PB2);
	PORTB |= (1 << PB0) | (0 << PB1);
	
	spi_init();
	uart_init();
	
	
	// PWR_UP = 1
	uart_puts("\nPWR_UP = 1\n");
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x00);	// команда чтения из регистра
		data1 = spi_transaction(0xff);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	data1 |= (1 << 1);		// флаг PWR_UP в единицу
	
	
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x20);	// команда записи в регистр
		data = spi_transaction(data1);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x00);	// команда чтения из регистра
		data = spi_transaction(0xff);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	_delay_ms(3); // ждём
	
	
	
	// PRIM_RX = 1; переход в режим приёмника
	uart_puts("\nPRIM_RX = 1\n");
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x00);	// команда чтения из регистра
		data1 = spi_transaction(0xff);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	data1 |= (1 << 0);		// флаг PRIM_RX в единицу
	
	
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x20);	// команда записи в регистр
		data = spi_transaction(data1);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x00);	// команда чтения из регистра
		data = spi_transaction(0xff);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	// RX_PW_P0 = 1
	uart_puts("\nRX_PW_P0 = 1\n");
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x31);	// команда записи в регистр
		data1 = spi_transaction(0x01);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	_delay_us(1);
	
	PORTB &= ~(1 << PB0);
		data = spi_transaction(0x11);	// команда чтения из регистра
		data = spi_transaction(0xff);
	PORTB |= (1 << PB0);
	
	_delay_us(1);
	
	
	
	// слушаем
	//PORTB |= (1 << PB1);
	
	while(1)
	{
		// слушаем
		PORTB |= (1 << PB1);
		_delay_ms(10);
		while( PINB & (1 << PINB2) )
		{
			;
		}
		
		_delay_us(1);
		
		PORTB &= ~(1 << PB1);	// перестаём слушать
		
		// чтение входного буффера
		PORTB &= ~(1 << PB0);
			data = spi_transaction(0x61);
			data = spi_transaction(0xff);
		PORTB |= (1 << PB0);
		
		_delay_us(1);
		
		if(data == 0)
		{
			PORTD &= ~( (1 << PD4) | (1 << PD5) | (1 << PD6) );
		}
		else if(data == 1)
		{
			PORTD |= (1 << PD4);
			PORTD &= ~((1 << PD5) | (1 << PD6) );
		}
		else if(data == 2)
		{
			PORTD |= (1 << PD5);
			PORTD &= ~((1 << PD4) | (1 << PD6) );
		}
		else if(data == 3)
		{
			PORTD |= (1 << PD6);
			PORTD &= ~((1 << PD4) | (1 << PD5) );
		}
		else
		{
			PORTD |= (1 << PD4) | (1 << PD5) | (1 << PD6);
		}
		
		
		
		uart_puts("\nStatus register\n");
		// чтение регистра состояния
		PORTB &= ~(1 << PB0);
		
			data1 = spi_transaction(0xff);	// команда чтения из регистра
			
		PORTB |= (1 << PB0);
		
		_delay_us(1);
		
		if( data1 & (1 << 4) )	// если максимальное количество повторных передач пакетов передано
		{
			uart_puts("\nMaximum retransmissions reached\n");
			data1 |= (1 << 4);	// сброс флага прерывания
			PORTB &= ~(1 << PB0);
			
				data = spi_transaction(0x27);
				data = spi_transaction(data1);
			
			PORTB |= (1 << PB0);
			
			_delay_us(1);
		}
		else if( data1 & (1 << 5) ) // данные переданы
		{
			uart_puts("\nData transmitted\n");
			data1 |= (1 << 5);	// сброс флага прерывания
			PORTB &= ~(1 << PB0);
			
				data = spi_transaction(0x27);
				data = spi_transaction(data1);
			
			PORTB |= (1 << PB0);
			
			_delay_us(1);
		}
		else if( data1 & (1 << 6) ) // данные получены
		{
			uart_puts("\nData received\n");
			data1 |= (1 << 6);	// сброс флага прерывания
			PORTB &= ~(1 << PB0);
			
				data = spi_transaction(0x27);
				data = spi_transaction(data1);
			
			PORTB |= (1 << PB0);
			
			_delay_us(1);
		}
	}
	
	
	
	/*_delay_ms(10000);
	
	
	uart_puts("\nStatus register\n");
	// чтение регистра состояния
	PORTB &= ~(1 << PB0);
	
		data = spi_transaction(0xff);	// команда чтения из регистра
		
	PORTB |= (1 << PB0);
	
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	while (1)
	{
		
	}
	
}

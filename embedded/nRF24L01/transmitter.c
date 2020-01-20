#include <avr/io.h>
#include <util/delay.h>

#include "spi_uart.h"


void main(void)
{
	unsigned char data, i, data1, j = 0;
	
	DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6);
	PORTD |= (0 << PD4) | (0 << PD5) | (0 << PD6);
	
	DDRB = (1 << PB0) | (1 << PB1) | (0 << PB2);
	PORTB |= (1 << PB0) | (0 << PB1);
	
	spi_init();
	uart_init();
	
	
	while(1)
	{
		_delay_ms(5000);
		
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
		
		// PWR_UP = 1 ; конец
		
		
		
		
		
		
		
		
		// запись передаваемых данных; количество = 1
		uart_puts("\nData\n");
		PORTB &= ~(1 << PB0);
		
			data = spi_transaction(0xa0);	// комманда для записи данных в буфер передачи
			
			data = spi_transaction(j);
			
		PORTB |= (1 << PB0);
		
		j++;
		if(j > 3)
		{
			j = 0;
		}
		
		_delay_us(1);
		
		// начинаем передачу
		uart_puts("\nTransmitting\n");
		PORTB |= (1 << PB1);
		
			_delay_us(20);
		
		PORTB &= ~(1 << PB1);
		
		_delay_ms(100);
		
		
		// PWR_UP = 0
		uart_puts("\nPWR_UP = 0\n");
		PORTB &= ~(1 << PB0);
			
			data = spi_transaction(0x00);	// команда чтения из регистра
			data1 = spi_transaction(0xff);
			
		PORTB |= (1 << PB0);
		
		_delay_us(1);
		
		data1 &= ~(1 << 1);		// флаг PWR_UP в нуль
		
		PORTB &= ~(1 << PB0);
		
			data = spi_transaction(0x20);	// команда записи в регистр
			data = spi_transaction(data1);
			
		PORTB |= (1 << PB0);
		
		_delay_ms(100);
		
		uart_puts("\nStatus register\n");
		// чтение регистра состояния
		PORTB &= ~(1 << PB0);
		
			data = spi_transaction(0xff);	// команда чтения из регистра
			
		PORTB |= (1 << PB0);
		
		_delay_us(1);	
		
		while( PINB & (1 << PINB2) )
		{
			;
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
		
		
		uart_puts("\nStatus register\n");
		// чтение регистра состояния
		PORTB &= ~(1 << PB0);
		
			data1 = spi_transaction(0xff);	// команда чтения из регистра
			
		PORTB |= (1 << PB0);
		
		_delay_us(1);
	
	}
	
	
	
	
	
		
	while (1)
	{
		
	}
	
}

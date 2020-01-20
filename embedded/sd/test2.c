#include <avr/io.h>
#include <util/delay.h>


#include "uart.h"
#include "spi.h"

//#include "sd_card.h"
#include "sdinit.h"



void main(void)
{
	char res;
	unsigned char c;
	int i;
	char buff[530];
	spi_init();
	uart_init();
	
	
	//PORTB &= !(1 << 6);
	
	if( (res = sd_init()) == 0 )
	{
		uart_puts("Initialization OK\n");
		
		
		
		/*uart_puts("\nCID\n");
		if( read_CID() )
		{
			uart_puts("\nCID-ERROR\n");
		}
		else
		{
			uart_puts("\nCID-OK\n");
		}
		*/
		
		/*uart_puts("\nCSD\n");
		if( read_CSD() )
		{
			uart_puts("\nCSD-ERROR\n");
		}
		else
		{
			uart_puts("\nCSD-OK\n");
		}*/
		
		/*uart_puts("\nSCR\n");
		if( read_SCR() )
		{
			uart_puts("\nSCR-ERROR\n");
		}
		else
		{
			uart_puts("\nSCR-OK\n");
		}*/
		
		c = 0;
		do
		{
			res = sd_readblock(buff, 0x00000000UL + (unsigned long)c);
			if(res)
			{
				uart_puts("\nRead_ERROR\n");
			}
			else
			{
				uart_puts("\nRead_Ok\n");
			}
			c++;
		}while(c < 1);
	}
	else
	{
		uart_puts("Initialization fail\n");
	}
	
	while(1)
	{
		;
	}
	
	/*PORTB &= !(1 << 6);
	
	if( (res = sd_init()) == 0 )
	{
		uart_putc('O');
		uart_putc('k');
		
		if( (res = sd_readblock(buff, 0x00000200UL) ) == 0 )
		{
			for(i = 0; i < 512; i++)
			{
				uart_putc(buff[i]);
			}
		}
		else
		{
			uart_putc('A' - 1 + res);
		}
		
	}
	else
	{
		uart_putc('N');
		uart_putc('o');
		uart_putc('A' - 1 + res);
	}
	
	while(1)
	{
		;
	}
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*uart_putc('s');
	
	
	if( ( res = sd_init() ) != 0 )
	{
		uart_putc('n');
		uart_putc('A' - 1 + res );
		while(1)
		{
			;
		}
		
	}
	
	res = sd_readblock(buff, 0x00000000);
	if(res != 0)
	{
		uart_putc('E');
		uart_putc('A' - 1 + res);
	}
	else
	{
		for(i = 0; i < 512; i++)
		{
			uart_putc(buff[i]);
		}
	}
	
	res = sd_readblock(buff, 0x00000200);
	if(res != 0)
	{
		uart_putc('E');
		uart_putc('A' - 1 + res);
	}
	else
	{
		for(i = 0; i < 512; i++)
		{
			uart_putc(buff[i]);
		}
	}
	
	while(1)
	{
		;
	}
	
	
	/*while( res = sd_init() )
	{
		uart_putc('n');
		uart_putc('A' - 1 +res);
		_delay_ms(500);
	}
	
	uart_putc('y');
	uart_putc('A' - 1 + card_type);
	
	
	while(1)
	{
		;
	}
	
	/*for(i = 0; i < 512; i++)
	{
		buff[i] = 'A' + i % 30;
	}
	
	for(i = 0; i < 512; i++)
	{
		uart_putc( buff[i] );
	}
	
	/*
	while( (res = sd_init()) )
	{
		uart_putc('n');
		//uart_putc('A' - 1 + res);
		_delay_ms(1000);
	}
	
	uart_putc('y');
	
	for(i = 0; i < 512; i++)
	{
		buff[i] = 'A';
	}
	
	
	
	if( ( res = sd_readblock(buff, 0) ) != 0 )
	{
		uart_putc('A' - 1 + res);
	}
	else
	{
		for(i = 0; i < 512; i++)
		{
			uart_putc( buff[i] );
			_delay_ms(10);
			
		}
	}
	
	while(1)
	{
		;
	}
	
	/*
	while(1)
	{
		uart_putc('y');
		_delay_ms(1000);
	}
	*/
	


	
}

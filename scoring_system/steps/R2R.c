#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>


void main(void)
{
	DDRC = 0xff;
	
	while(1)
	{
		PORTC = 0x00;
		_delay_us(500);
		PORTC = 0x01;
		_delay_us(500);
	}
}

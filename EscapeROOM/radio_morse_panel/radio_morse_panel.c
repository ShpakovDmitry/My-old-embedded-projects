#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CARRIER_ON	(1 << PD5)
#define CARRIER_OFF	(0 << PD5)
#define dot_length 150
#define repeat_max 5
#define RED_LED PD3
#define GREEN_LED PD4

unsigned char carrier = CARRIER_OFF;

void unit()
{
	unsigned char i;
	
	for(i = 0; i < dot_length; i++)
	{
		PORTD ^= ((1 << PD5) & carrier);
		_delay_ms(1);
	}
}

void dot()
{
	carrier = CARRIER_ON;
	unit();
	carrier = CARRIER_OFF;
}

void dash()
{
	carrier = CARRIER_ON;
	unit(); unit(); unit();
	carrier = CARRIER_OFF;
}

void short_space()
{
	unit();
}

void letter_space()
{
	unit(); unit(); unit();
}

void word_space()
{
	unit(); unit(); unit(); unit(); unit(); unit(); unit();
}

void zero()
{
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dash();
}


void one()
{
	dot();  short_space();
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dash();
}


void two()
{
	dot(); short_space();
	dot(); short_space();
	dash(); short_space();
	dash(); short_space();
	dash();
}


void three()
{
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dash(); short_space();
	dash();
}


void four()
{
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dash();
}


void five()
{
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot();
}

void six()
{
	dash(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot();
}

void seven()
{
	dash(); short_space();
	dash(); short_space();
	dot(); short_space();
	dot(); short_space();
	dot();
}

void eight()
{
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dot(); short_space();
	dot();
}

void nine()
{
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dash(); short_space();
	dot();
}


void phrase()
{
	four(); letter_space();
	seven(); letter_space();
	two();
	word_space();
}

unsigned char play_flag = 0;
unsigned char button = 0;

unsigned char button1 = ~(1 << PB6);
unsigned char button2 = ~(1 << PB1);
unsigned char button3 = ~(1 << PB4);

//unsigned char button1 = ~(1 << PB5);
//unsigned char button2 = ~(1 << PB3);
//unsigned char button3 = ~(1 << PB1);

ISR(PCINT_vect)
{
	static unsigned char i = 0;
	unsigned char data = PINB;
	
	// delay while noise finishes and clear interrupt flag
	// to do not cause extra interrupt
	_delay_ms(500);
	EIFR |= 1 << PCIF;
	
	if(data == 0xff)
	{
		return;
	}
	if(!(data & 0x01) || !(data & 0x02) || !(data & 0x04) || !(data & 0x08) || \
	   !(data & 0x10) || !(data & 0x20) || !(data & 0x40) || !(data & 0x80))
	{
			if(i == 0)
			{
				PORTD &= ~( (1 << RED_LED) | (1 << GREEN_LED) );
				if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 1)
			{
				if(data == button2)
				{
					i = 2;
				}
				else if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 2)
			{
				if(data == button3)
				{
					play_flag = 1;
					i = 0;
					PORTD |= (1 << GREEN_LED);
				}
				else if(data == button1)
				{
					i = 1;
				}
				else
				{
					i = 0;
					PORTD |= (1 << RED_LED);
					return;
				}
			}
			
	}
	else
	{
		i = 0;
	}
	
	if(play_flag)
	{
		play_flag = repeat_max;
		for(;play_flag > 0; play_flag--)
		{
			phrase();
		}
	}
}


void main()
{
	PORTD &= ~( (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED) );
	DDRD |= (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED);
	PORTB = 0xff;
	DDRB = 0x00;
	unsigned char k;
	
	PCMSK = (1 << PCINT7) | (1 << PCINT6) | (1 << PCINT5) | (1 << PCINT4) | \
		 (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1) | (1 << PCINT0);
	GIMSK |= (1 << PCIE);
	SREG |= (1 << 7);
	
	while(1)
	{
		;
	}
}








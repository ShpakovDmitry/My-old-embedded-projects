#include <avr/io.h>
#include <util/delay.h>

//#define BLACK1_1 PB0
//#define BLACK1_2 PB1
//
//#define BLACK2_1 PB2
//#define BLACK2_2 PB3
//
//#define RED1_1 PB4
//#define RED1_2 PB5
//
//#define RED2_1 PB6
//#define RED2_2 PB7

#define BLACK1_1 PB1
#define BLACK1_2 PB5

#define BLACK2_1 PB0
#define BLACK2_2 PB4

#define RED1_1 PB2
#define RED1_2 PB6

#define RED2_1 PB3
#define RED2_2 PB7

#define Loudspeaker	PD5
#define Stroboscope	PD2
#define Smoke_machine	PD3
#define UV_lamp		PD1

#define Emergency_button PA1

#define BLACK1 0
#define BLACK2 1
#define RED1 2
#define RED2 3

#define DELAY 10
#define SWITCH_DELAY 1000
#define STATUS_COUNT_DELAY 10
#define MAX_STATUS_COUNT 10

int main(void)
{
	// set devices off
	PORTD &= ~((1 << UV_lamp) | (1 << Loudspeaker) | \
		(1 << Stroboscope) | (1 << Smoke_machine));
	// configure PORTD
	DDRD |= (1 << UV_lamp) | (1 << Loudspeaker) | \
		(1 << Stroboscope) | (1 << Smoke_machine);
	
	// configure PORTB
	PORTB = 0x00; DDRB = 0x00;

	// configure PORTA
	DDRA &= ~(1 << Emergency_button);
	PORTA |= (1 << Emergency_button); // switch-on pull-up resistors

	unsigned char panel_status = 0x00;
	unsigned int status_count = 0;

	while(1)
	{
		
		DDRB = 1 << BLACK1_1;
		PORTB = 1 << BLACK1_1;
		_delay_ms(DELAY);
		if(PINB & (1 << BLACK1_2))
		{
			panel_status |= (1 << BLACK1);
		}
		else
		{
			panel_status &= ~(1 << BLACK1);
		}


		PORTB = 0x00;
		_delay_ms(DELAY);


		DDRB = 1 << BLACK2_1;
		PORTB = 1 << BLACK2_1;
		_delay_ms(DELAY);
		if(PINB & (1 << BLACK2_2))
		{
			panel_status |= (1 << BLACK2);
		}
		else
		{
			panel_status &= ~(1 << BLACK2);
		}


		PORTB = 0x00;
		_delay_ms(DELAY);


		DDRB = 1 << RED1_1;
		PORTB = 1 << RED1_1;
		_delay_ms(DELAY);
		if(PINB & (1 << RED1_2))
		{
			panel_status |= (1 << RED1);
		}
		else
		{
			panel_status &= ~(1 << RED1);
		}


		PORTB = 0x00;
		_delay_ms(DELAY);


		DDRB = 1 << RED2_1;
		PORTB = 1 << RED2_1;
		_delay_ms(DELAY);
		if(PINB & (1 << RED2_2))
		{
			panel_status |= (1 << RED2);
		}
		else
		{
			panel_status &= ~(1 << RED2);
		}


		PORTB = 0x00;
		_delay_ms(DELAY);

		if( !(PINA & (1 << Emergency_button))  )
		{
			panel_status = (1 << BLACK1) | (1 << BLACK2) | (1 << RED1) | (1 << RED2);
		}


		if(panel_status == ( (1 << BLACK1) | (1 << BLACK2) | (1 << RED1) | (1 << RED2) ))
		{
			
			if(status_count++ > MAX_STATUS_COUNT)
			{
				PORTD |= (1 << Loudspeaker);
				_delay_ms(SWITCH_DELAY);
				PORTD |= (1 << UV_lamp);
				//_delay_ms(SWITCH_DELAY);
				//PORTD |= (1 << Stroboscope);
				//_delay_ms(SWITCH_DELAY);
				//PORTD |= (1 << Smoke_machine);
				
				while(1)
				{
					;// infinite loop use hardware RESET
				}
			}
			//_delay_ms(STATUS_COUNT_DELAY);
		}
		else
		{
			status_count = 0;
		}

	}

	return 0;
}

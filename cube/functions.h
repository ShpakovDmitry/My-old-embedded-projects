// pinout definitions for SH74HC595N
#define SRCLK	PB4	// clock out
#define SER	PB3	// serial out
#define RCLK	PB1	// storage register clock out
// pinout definitions for SH74HC165N
#define CLK	PB4	// clock out
#define Q_H	PB2	// serial in
#define SH_LD	PB0	// latch out

#define SPEAKER PD6	// speaker out

#define dt 10

enum sounds { INVALID, CORRECT, BUTTON_PUSH };

void init_registers(void);
void send_to_register(unsigned int data);
unsigned char get_from_register(void);
void make_sound_on_speaker(unsigned int duration_us, unsigned int freqency);
void play_sound(unsigned char sound_type);


void init_registers(void)
{
	//Настраиваем нужные пины на выходы
	DDRB |= (1 << SRCLK) | (1 << SER) | (1 << RCLK);	// out pins for SN74HC595N
	DDRB |= (1 << CLK) | (1 << SH_LD);			// out pins for SN74HC165N

	DDRD |= (1 << SPEAKER);					// out for speaker
	
	//Настраиваем нужные пины на входы
	DDRB &= ~(1 << Q_H);					// in pins for SN74HC165N
	
	
	// установка начальных состояний портов
	PORTB |= (1 << RCLK);
	PORTB |= (1 << SH_LD);
	
	PORTB &= ~( (1 << SRCLK) | (1 << SER) );
	PORTB &= ~( (1 << Q_H) );

	PORTD &= ~(1 << SPEAKER);
}


void send_to_register(unsigned int data)
{
	unsigned char i = 16;
	
	
	PORTB &= ~(1 << RCLK);
	
	do
	{
		PORTB &= ~(1 << SRCLK);
		if(data & 0x8000)
		{
			PORTB |= (1 << SER);
		}
		else
		{
			PORTB &= ~(1 << SER);
		}
		data = data << 1;
		_delay_us(dt);
		PORTB |= (1 << SRCLK);
		_delay_us(dt);
	}while(--i);
	
	PORTB |= (1 << RCLK);
	_delay_us(dt);
}

unsigned char get_from_register(void)
{
	unsigned char temp, i = 7;
	
	PORTB |= (1 << CLK);
	PORTB &= ~(1 << SH_LD);
	_delay_us(dt);
	PORTB |=  (1 << SH_LD);
	
	do
	{
		if(PINB & (1 << Q_H))
		{
			temp |= (1 << 0);
		}
		else
		{
			temp &= ~(1 << 0);
		}
		temp <<= 1;
		PORTB &= ~(1 << CLK);
		_delay_us(dt);
		PORTB |= (1 << CLK);
		_delay_us(dt);
	}while(--i);
	if(PINB & (1 << Q_H))
	{
		temp |= (1 << 0);
	}
	else
	{
		temp &= ~(1 << 0);
	}


	return temp;
	
}

void play_sound(unsigned char sound_type)
{
	unsigned int k;

	if( sound_type == INVALID )
	{
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);
		
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);
		
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);

		for(k = 0; k < 67; k++)
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(3000);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(3000);
		}	
	}
	else if( sound_type == CORRECT )
	{
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);
		
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);
		
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}
		_delay_ms(100);


		for(k = 0; k < 1538; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(130);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(130);
		}


	}
	else if( sound_type == BUTTON_PUSH )
	{
		for(k = 0; k < 500; k++ )
		{
			PORTD |= (1 << SPEAKER);
			_delay_us(200);
			PORTD &= ~(1 << SPEAKER);
			_delay_us(200);
		}

	}
	else
	{
		// play secret sound

	}






}

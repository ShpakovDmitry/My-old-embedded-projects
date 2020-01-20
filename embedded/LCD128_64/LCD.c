#include <avr/io.h>
#include <util/delay.h>


#define RST	PD7
#define RS	PD6
#define RW	PD5
#define  E	PD4

#define SET_CGRAM	0x80	// установка адреса CGRAM
#define FUNCTION_SET 	0x20
	#define RE 0x04		// 1 - набор расширенных комманд
	#define DL 0x10		// 1 - 8-ми битный режим
#define DISPLAY_STATUS	0x08
	#define B 0x01		// моргание курсора
	#define C 0x02		// включение курсора
	#define D 0x04		// включение дисплея
#define ENTRY_MODE_SET	0x04
	#define  S 0x01		// 
	#define ID 0x02		// движение курсора слева на право
#define RETURN_HOME	0x02	// положение курсора в начало
#define CLEAR		0x01	// очистка экрана



void write_instruction(unsigned char instruction);
void init_LCD(void);
void write_data(unsigned char data);
void busy(void);

void print_string(unsigned char pos, char *str);


void main(void)
{
	unsigned char i, j, k = 0;
	unsigned char hour, min, sec;
	hour = min = sec =  0;
	unsigned char dt = 30;
	
	_delay_ms(50);

	DDRA = 0xff;	PORTA = 0xff;
	DDRC = 0xff;
	DDRD |= (1 << RST) | (1 << RS) | (1 << RW) | (1 << E);
	PORTD = 0x00;
	PORTC = 0x00;


	init_LCD();

	write_instruction(DISPLAY_STATUS | D) ;
	_delay_ms(10);
/*
	write_instruction(FUNCTION_SET | DL);
	_delay_ms(10);
	write_instruction(FUNCTION_SET | RE);
	_delay_ms(10);
	write_instruction(FUNCTION_SET | 0x02);
	_delay_ms(10);
*/

/*	write_instruction(0x80);
	_delay_ms(10);
	write_instruction(0x80);
	_delay_ms(10);

	write_data(0xff);
	_delay_ms(10);
	write_data(0xff);
	_delay_ms(10);
*/
/*
	for(i = 0; i < 32; i++)
	{
		write_instruction(0x80);
		_delay_us(100);
		write_instruction(0x80 + i);
	}
*/
	//print_string(8, "M={0;1}"); 
	/*write_instruction(SET_CGRAM | 0x10); _delay_us(dt);
	for(i = 0; i < 32; i++)
	{
		write_data(0xA7); _delay_us(dt);
		write_data(0x20 + 1 + i); _delay_us(dt);
		_delay_ms(100);
	}
	*/

	write_instruction(SET_CGRAM | 0x10); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x23); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x62); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x56); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x5e); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x71); _delay_us(dt);

	write_instruction(SET_CGRAM | 0x0c); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x5a); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x55); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x57); _delay_us(dt);

	write_data(0xa7); _delay_us(dt);
	write_data(0x64); _delay_us(dt);

	write_instruction(SET_CGRAM | 0x01);
	while(1)
	{
		write_data(' '); _delay_us(dt);

		write_data('0' + (hour / 100) % 10); _delay_us(dt);
		write_data('0' + (hour /  10) % 10); _delay_us(dt);
		write_data('0' + (hour /   1) % 10); _delay_us(dt);

		write_data(':'); _delay_us(dt);
		
		write_data('0' + (min / 10) % 10); _delay_us(dt);
		write_data('0' + (min /  1) % 10); _delay_us(dt);

		write_data(':'); _delay_us(dt);
		
		write_data('0' + (sec / 10) % 10); _delay_us(dt);
		write_data('0' + (sec /  1) % 10); _delay_us(dt);

		write_instruction(SET_CGRAM | 0x01);
		
		sec++;
		if(sec == 60)
		{
			sec = 0;
			min++;
			if(min == 60)
			{
				min = 0;
				hour++;
			}
		}
		_delay_ms(1000);
	}
	
	
	while(1)
	{
		;
	}
}


void init_LCD(void)
{
	PORTD &= ~(1 << RST);
	_delay_ms(500);
	PORTD |= (1 << RST); // вывод экрана из состояния сброса
	_delay_ms(40);
	write_instruction(FUNCTION_SET | DL);
	_delay_us(100);
	write_instruction(FUNCTION_SET | DL);
	_delay_us(37);
	write_instruction(DISPLAY_STATUS | D);
	_delay_us(100);
	write_instruction(CLEAR);
	_delay_ms(10);
	write_instruction(ENTRY_MODE_SET | ID);
	_delay_us(72);
}



void write_instruction(unsigned char instruction)
{
	PORTC = instruction;
	PORTD &= ~(1 << RS);
	PORTD |= (1 << E);
	_delay_us(5);
	PORTD &= ~(1 << E);
	_delay_us(40);
}

void write_data(unsigned char data)
{
	PORTC = data;
	PORTD |= (1 << RS);
	PORTD |= (1 << E);
	_delay_us(5);
	PORTD &= ~(1 << E);
}




void busy(void)
{
	PORTD &= ~(1 << RS);
	PORTD |= (1 << RW);
	PORTD |= (1 << E);

	while(PINC & (1 << 7))
	{
		;
	}

}

void print_string(unsigned char pos, char *str)
{
	unsigned char i = 0;
	
	write_instruction(SET_CGRAM | pos);
	_delay_ms(1);
	while(str[i] != '\0')
	{
		write_data(str[i]);
		_delay_us(30);
		i++;
	}
}


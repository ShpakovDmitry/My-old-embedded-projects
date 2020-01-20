#include <avr/io.h>
#include <util/delay.h>

#define dt 10

void send_to_register(char data);

int main(void)
{
	//Настраиваем нужные пины на выходы
	DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3);
	char temp = 0x00;
	int i;
	
	//Бесконечный цикл
	while(1)
	{
		send_to_register(temp++);
		_delay_ms(500);
	}
	
	//main возвращает 0, как всегда
	return 0;
}

void send_to_register(char data)
{
	USIDR = data;
	//Опускаем флаг переполнения таймера
	USISR |= ( 1 << USIOIF );
	//Отпускаем защелку регистра 74HC595
	PORTB &= ~( 1 << PB4 );
	//Пока не поднялся флаг переполнения счетчика
	while( ( USISR & ( 1 << USIOIF ) ) == 0 )
	{
		//Выставляем трехпроводной режим;
		//Выбираем внешний источник тактирования, с положительным фронтом;
		//Выставляем тактирование по стробу USITC;
		//Собственно, генерируем сам строб USITC
		USICR = ( 1 << USIWM0 ) | ( 1 << USICS1 ) | ( 1 << USICLK ) | ( 1 << USITC );
	}
	//Защелкиваем данные в регистре 74HC595
	PORTB |= ( 1 << PB4 );
	//_delay_us( dt );
}

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "scoring_system.h"
#include "spi_uart.h"
#include "nRF24L01.h"
#include "eeprom.h"
#include "LED.h"

#ifndef CSN
 #define CS
#endif

// определения относящиеся к flash памяти
#define CS PB4
#define continuous_read	0x03
#define flush_buffer1	0x83
#define buffer1_write	0x84
#define read_status_reg	0xd7
#define no_command	0xff


#define INT2_timeout	5000
#define INT12_timeout	5000

// глобальные переменные
unsigned char data[packet_length];	// массив для пакета данных
unsigned char count1 = 0;		// счёт первого игрока
unsigned char count2 = 0;		// счёт второго игрока
unsigned  int dt_count = 0;		// количество шагов времени
unsigned char total_min = 0;		// количество минут общего времени
unsigned char total_sec = 0;		// количество секунд общего времени
unsigned char goal_sec = 0x00;		// количество секунд времени между голами
unsigned char goal_min = 0x00;		// количество минут времени между голами
unsigned char old_goal_sec = 0;		// отметка секунд времени с момента последнего гола
unsigned char old_goal_min = 0;		// отметка минут времени с момента последнего гола
unsigned char dt = 1;			// шаг времени(1 dt =  1/22059 сек = 45.3 микросекунд)
unsigned char dcount = 1;
unsigned char max_count1;		// значение максимально допустимого счёта первого игрока
unsigned char max_count2;		// значение максимально допустимого счёта второго игрока
unsigned  int global_sec;		// количество секунд прошедшее с момента последнего сброса мк.
unsigned char game_ended = 0;		// флаг устанавливается в "1" если максимальный счёт одного из игроков дрстигнут
unsigned char game_not_stopped = 1;	// флаг паузы игры, 0 если игра остановленна
unsigned char sound_name;		// номер муз. файла
unsigned char read_next_byte = 0;	// если 1, то из flash памяти читается следующий байт
unsigned char play_count = 0;		// счётчик проигрывания песен

unsigned int INT2_timeout_ticks = 0;
unsigned char INT2_timeout_flag = 0;
unsigned int INT12_timeout_ticks = 0;





unsigned char value1 = 0;
unsigned char value2 = 0;


void init_LED_indicator(void);					// инициализация индикатора
void LED_indicator(unsigned char x1, unsigned char x2);		// отображение чисел x1 и x2 на индикаторе
void init_start_game(void);					// считывание макс. счёта; лучшего времени
unsigned char read_from_eeprom(unsigned char address);		// чтение из eeprom
void write_to_eeprom(unsigned char address, unsigned char x1);	// запись в eeprom
void init_nRF24L01(void);					// инициализация радио приёмо-передатчика
unsigned char read_register_nRF24L01(const reg);
void write_register_nRF24L01(const reg, unsigned char data);
void receive_data_nRF24L01(unsigned char *dat);
void transmit_data_nRF24L01(unsigned char *dat);
void pwr_up_nRF24L01(void);
void pwr_down_nRF24L01(void);
void enter_receiver_mode_nRF24L01(void);
void enter_transmitter_mode_nRF24L01(void);
unsigned char decode_data_and_execute(unsigned char *data);
unsigned char random();

void init_flash(void);
void play_sound(void);
void get_and_put_next_sound_byte(void);
void get_bytes(unsigned int page, unsigned int offset, unsigned char *byte2, unsigned char *byte1, unsigned char *byte0);


void init_interrupt_INT0(void)
{
	MCUCR |= (1 << ISC01);	// прерывание на INT0 по спадающему фронту
	GICR  |= (1 << INT0);	// разрешить прерывание INT0
}
void init_interrupt_INT1(void)
{
	MCUCR |= (1 << ISC11);	// прерывание на INT1 по спадающему фронту
	GICR  |= (1 << INT1);	// разрешить прерывание INT1
}
void init_interrupt_INT2(void)
{
	MCUCSR |= (1 << ISC2);	// прерывание на INT2 по спадающему фронту
	GICR   |= (1 << INT2);	// разрешить прерывание INT2
}
void init_interrupt_TIMER0(void)	// прерывание от таймера; источник тактирования внешний кварц: F = 18 МГц; требуемая частота прерывания: ~22.050 кГц;
{
	TCCR0 = 0;
	OCR0 = 0x65;
	TCCR0 |= (1 << WGM01) | (0 << CS02) | (1 << CS01) | (0 << CS00);	// предделитель равен 8, режим CTC; частота прерывания 22,059 кГц(ошибка 0,041%)
	TIMSK |= (1 << OCIE0);
}

ISR(INT0_vect)
{
	if(dcount != 0)
	{
		dcount = 0;
		INT12_timeout_ticks = 0;
		count1++;
		if(total_sec > old_goal_sec)
		{
			goal_sec = total_sec - old_goal_sec;
		}
		else
		{
			goal_sec = old_goal_sec - total_sec;
		}
		if(total_min > old_goal_min)
		{
			goal_min = total_min - old_goal_min;
		}
		else
		{
			goal_min = old_goal_min - total_min;
		}
		old_goal_sec = total_sec;
		old_goal_min = total_min;
		
		//play_sound();
		

		// если максимально допустимый счёт достигнут, то остановить игру(делается пауза игры)
		if((count1 == max_count1) && (game_ended == 0) )
		{
			// отключаем/включаем прерывания от датчиков
	/*		if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
			{
				GIFR |= (1 << INTF1) | (1 <<INTF0);
			}
			GICR ^= ( (1 << INT1) | (1 << INT0) );
	*/
			dt ^= (1 << 0);
			//dcount ^= (1 << 0);
			game_ended = 1;
			game_not_stopped = 0;
		}
	}

}

ISR(INT1_vect)
{
	if(dcount != 0)
	{
		dcount = 0;
		INT12_timeout_ticks = 0;
		count2++;
		if(total_sec > old_goal_sec)
		{
			goal_sec = total_sec - old_goal_sec;
		}
		else
		{
			goal_sec = old_goal_sec - total_sec;
		}
		if(total_min > old_goal_min)
		{
			goal_min = total_min - old_goal_min;
		}
		else
		{
			goal_min = old_goal_min - total_min;
		}
		old_goal_sec = total_sec;
		old_goal_min = total_min;
	

		//play_sound();

		// если максимально допустимый счёт достигнут, то остановить игру(делается пауза игры)
		if((count2 == max_count2) && (game_ended == 0))
		{
			// отключаем/включаем прерывания от датчиков
		/*	if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
			{
				GIFR |= (1 << INTF1) | (1 <<INTF0);
			}
			GICR ^= ( (1 << INT1) | (1 << INT0) );
		*/
			dt ^= (1 << 0);
			//dcount ^= (1 << 0);
			game_ended = 1;
			game_not_stopped = 0;
		}
	}


}

ISR(INT2_vect)
{
	INT2_timeout_flag = 1;
	INT2_timeout_ticks = 0;
	GICR &= ~(1 << INT2);

	// отключаем/включаем прерывания от датчиков
/*	if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
	{
		GIFR |= (1 << INTF1) | (1 <<INTF0);
	}
	GICR ^= ( (1 << INT1) | (1 << INT0) );
*/
	dt ^= (1 << 0);
	dcount ^= (1 << 0);
	game_not_stopped ^= (1 << 0);
	read_next_byte = 0;
}

ISR(TIMER0_COMP_vect)
{
	if(INT2_timeout_flag == 1)
	{
		if(++INT2_timeout_ticks > INT2_timeout)
		{
			GIFR = (1 << INTF2);
			GICR |= (1 << INT2);
		}
	}
	if(dcount == 0 && game_not_stopped == 1)
	{
		if(++INT12_timeout_ticks > INT12_timeout)
		{
			dcount = 1;
		}
	}
	dt_count++;
	if(dt_count > 22059)
	{
		if(dt == 1)
		{
			total_sec++;
		}
		dt_count = 0;
		if(global_sec++ > 30)
		{
			global_sec = 0;
		}
	}
	if(total_sec == 60)
	{
		if(dt == 1)
		{
			total_min++;
			total_sec = 0;
		}
	}
	if( (global_sec % 19) < 5)
	{
		LED_indicator(total_min, total_sec);
	}
	else
	{
		LED_indicator(count1, count2);
	}
	//get_and_put_next_sound_byte();
}


void main(void)
{
	unsigned char i, a;
	_delay_ms(500);	// для стабилизации напряжения
	init_start_game();
	uart_init();
	spi_init();
	init_nRF24L01();
	init_flash();
	init_interrupt_INT0();
	init_interrupt_INT1();
	init_interrupt_INT2();
	init_interrupt_TIMER0();
	init_LED_indicator();
	

	SREG |= (1 << SREG_I);	// разрешить прерывания глобально
	// вход в бесконечный цикл
	while(1)
	{
		// переходим в режим приёмника
		enter_receiver_mode_nRF24L01();

		// ждём данных от ведущего
		while((PINB & (1 << IRQ)))
		{
			;	// пока не появится прерывание передатчика, бесконечный цикл
		}
		// если прерывание произошло из-за того, что данные пришли, то считать их
		a = read_register_nRF24L01(STATUS);
		if(a & (1 << RX_DR))
		{
			// принимаем пакет данных
			receive_data_nRF24L01(data);
			// расшифровываем команду и выполняем соответствующие действия
			if(decode_data_and_execute(data))
			{
				// сброс флага прерывания
				a = read_register_nRF24L01(STATUS);
				a |= (1 << RX_DR);
				write_register_nRF24L01(STATUS, a);
				write_register_nRF24L01(FLUSH_RX, 0);
				// переходим в режим передатчика для передачи данных
				enter_transmitter_mode_nRF24L01();
				// передаём данные назад
				i = 0;
				do
				{
					transmit_data_nRF24L01(data);
					_delay_ms(10);
					a = read_register_nRF24L01(STATUS);
					if(a & (1 << MAX_RT))
					{
						a |= (1 << MAX_RT);
						write_register_nRF24L01(STATUS, a);
						write_register_nRF24L01(FLUSH_TX, 0);
					}
					i++;
				}while( ((a & (1 << TX_DS)) == 0) && (i < 100));
				if((a & (1 << TX_DS)))
				{
					a |= (1 << TX_DS);
					write_register_nRF24L01(STATUS, a);
				}
				write_register_nRF24L01(FLUSH_TX, 0);
	

			}
		}
	}
}



/*========================Реализация функций============================*/

void init_start_game(void)
{
	unsigned char max_c1, max_c2;

	max_c1 = read_from_eeprom(max_c1_addr);
	max_c2 = read_from_eeprom(max_c2_addr);

	// если в eeprom памяти записано что один из максимальных счётов равен FF, то установить значение по умолчанию 10
	if((max_c1 == 0xFF) || (max_c2 == 0xFF))
	{
		write_to_eeprom(max_c1_addr, 10);
		write_to_eeprom(max_c2_addr, 10);
		max_count1 = 10;
		max_count2 = 10;
	}
	else
	{
		max_count1 = max_c1;
		max_count2 = max_c2;
	}
}

unsigned char decode_data_and_execute(unsigned char *data)
{
	unsigned char byte2, byte1, byte0;
	unsigned char rdata;
	static unsigned char i = 0;
	static unsigned char f = 0;
	static unsigned long int page = 0;
	static unsigned int offset = 0;
	static unsigned char current = 0xff;
	static unsigned char continue_write = 1;
	//static unsigned char offsetcount = 0;

	if((*data) == score_and_time)
	{
		data[1] = count1;		// счёт первого игрока
		data[2] = count2;		// счёт второго игрока
		data[3] = total_min;		// общее количество минут после сброса
		data[4] = total_sec;		// общее количество секунд после сброса
		data[5] = old_goal_min;		// отметка минут времени последнего гола
		data[6] = old_goal_sec;		// отметка секунд времени последнего гола
		data[7] = game_not_stopped;	// флаг паузы игры
	}
	else
	if((*data) == change_rules)
	{
		if(data[1] == 0 || data[2] == 0)
		{
			data[0] = send_information;
			data[1] = illegal_data;
		}
		else
		{
			max_count1 = data[1];
			max_count2 = data[2];
			write_to_eeprom(max_c1_addr, max_count1);
			write_to_eeprom(max_c2_addr, max_count2);
		}
	}
	else
	if((*data) == change_score_and_time)
	{
		if(data[3] <= data[5])
		{
			if(data[4] < data[6])
			{
				data[0] = send_information;
				data[1] = illegal_data;
			}
		}
		else
		{
			count1 = data[1];
			count2 = data[2];
			total_min = data[3];
			total_sec = data[4];
			old_goal_min = data[5];
			old_goal_sec = data[6];
		}
	}
	else
	if((*data) == pause_game)
	{
		// отключаем прерывания от датчиков
		/*if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
		{
			GIFR |= (1 << INTF1) | (1 <<INTF0);
		}
		GICR &= ~( (1 << INT1) | (1 << INT0) );
		*/
		dcount = 0;
		dt = 0;
		game_not_stopped = 0;
		read_next_byte = 0;
		
	}
	else
	if((*data) == continue_game)
	{
		// включаем прерывания от датчиков
		/*if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
		{
			GIFR |= (1 << INTF1) | (1 <<INTF0);
		}
		GICR |= ( (1 << INT1) | (1 << INT0) );
		*/
		dcount = 1;
		dt = 1;
		game_not_stopped = 1;

	}
	else
	if((*data) == reset_game)
	{
		// отключаем прерывания от датчиков
		/*if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
		{
			GIFR |= (1 << INTF1) | (1 <<INTF0);
		}
		GICR &= ~( (1 << INT1) | (1 << INT0) );
		*/
		dcount = 0;
		dt = 0;
		game_not_stopped = 0;
		
		count1 = 0;		count2 = 0;
		total_min = 0;		total_sec = 0;
		old_goal_min = 0;	old_goal_sec = 0;
		game_ended = 0;


	}
	else
	if((*data) == 0xff)
	{
		// ничего не делать
		return 0;
	}
	//else
	//if((*data) == load_data_to_flash)
	//{
	//	if(game_not_stopped)
	//	{
	//		// отключаем прерывания от датчиков
	//		/*if( (GICR & ((1 << INT0) | (1 << INT1))) == 0)
	//		{
	//			GIFR |= (1 << INTF1) | (1 <<INTF0);
	//		}
	//		GICR &= ~( (1 << INT1) | (1 << INT0) );
	//		*/
	//		dcount = 0;
	//		dt = 0;
	//		game_not_stopped = 0;
	//		read_next_byte = 0;
	//	}
	//	if(data[1] < 16 )
	//	{
	//		while(PORTB & (1 << CSN) == 0)
	//		{
	//			;	// ждать пока приёмо-передатчик не освободит SPI
	//		}

	//		// если номер нового муз. файла не равен старому, то запись нового муз. файла
	//		if(data[1] != current)
	//		{
	//			current = data[1];
	//			offset = 0;
	//			page = current * 256;
	//			continue_write = 1;
	//		}
	//		if(continue_write)
	//		{
	//			// запись в буфер с начальным смещением offset
	//			get_bytes(0, offset, &byte2, &byte1, &byte0);
	//			PORTB &= ~(1 << CS);
	//			spi_transaction(buffer1_write);
	//			spi_transaction(byte2);
	//			spi_transaction(byte1);
	//			spi_transaction(byte0);
	//			for(i = 2; i < 32 && continue_write; i++)
	//			{
	//				spi_transaction(data[i]);
	//				offset++;
	//				if(offset == 528)
	//				{
	//					offset = 0;
	//					PORTB |= (1 << CS);
	//					_delay_us(10);
	//				
	//					// команда записи буфера в flash память
	//					PORTB &= ~(1 << CS);
	//					get_bytes(page, offset, &byte2, &byte1, &byte0);
	//					spi_transaction(flush_buffer1);
	//					spi_transaction(byte2);
	//					spi_transaction(byte1);
	//					spi_transaction(byte0);
	//					PORTB |= (1 << CS);
	//					
	//					_delay_us(10);

	//					// чтение регистра состояния
	//					PORTB &= ~(1 << CS);
	//					spi_transaction(read_status_reg);
	//					do
	//					{
	//						rdata = spi_transaction(no_command);
	//						_delay_us(10);
	//					}while((rdata & (1 << 7)) == 0);	// ждать пока flash память занята
	//					PORTB |= (1 << CS);
	//					_delay_us(10);
	//					// проверяем не вышли ли мы за пределы файла
	//					page++;
	//					if(page > (((unsigned long)current + 1)*256) )
	//					{
	//						data[0] = send_information;
	//						data[1] = file_is_written;
	//						continue_write = 0;
	//					}
	//					else
	//					{
	//						// записываем остальные данные на новую страницу памяти
	//						get_bytes(0, offset, &byte2, &byte1, &byte0);
	//						PORTB &= ~(1 << CS);
	//						spi_transaction(buffer1_write);
	//						spi_transaction(byte2);
	//						spi_transaction(byte1);
	//						spi_transaction(byte0);

	//					}
	//				}
	//			}
	//			// заканчиваем запись в буфер
	//			PORTB |= (1 << CS);
	//			_delay_us(10);
	//			if(data[0] != send_information)
	//			{
	//				data[0] = send_information;
	//				data[1] = packet_is_written;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		data[0] = send_information;
	//		data[1] = illegal_data;
	//	}

	//}
	else
	{
		data[0] = send_information;
		data[1] = unknown_command;
	}

	return 1;
	
}


unsigned char random()
{
	return 0;
	//return total_sec % 14;
}

void init_flash(void)
{
	DDRB |= (1 << CS);
	PORTB |= (1 << CS);
}


void play_sound(void)
{
	if((count1 == max_count1 || count2 == max_count2) && (game_ended == 0))
	{
		sound_name = 0;
	}
	sound_name = 0 + random();
	read_next_byte = 1;
	play_count++;
}

void get_and_put_next_sound_byte(void)
{
	static unsigned char last_play_count = 0x00;
	unsigned char byte0, byte1, byte2;
	static unsigned int page;
	static unsigned int offset;
	static unsigned int pagecount = 0;

	if(read_next_byte == 1)
	{
		if(PORTB & (1 << CSN))	// если SPI не используется приёмо-передатчиком
		{
			if(play_count != last_play_count)	// если номер счёта музыки изменился, то начать выводить новую музыку
			{
				last_play_count = play_count;
				page = sound_name * 256;
				pagecount = 0;
				offset = 0;
				get_bytes(page, offset, &byte2, &byte1, &byte0);

				PORTB &= ~(1 << CS);
				spi_transaction(continuous_read);
				spi_transaction(byte2);
				spi_transaction(byte1);
				spi_transaction(byte0);

				PORTC = spi_transaction(no_command);
				offset++;
				PORTB |= (1 << CS);
			}
			else
			{
				offset++;
				if(offset < 528)
				{
					get_bytes(page, offset, &byte2, &byte1, &byte0);

					PORTB &= ~(1 << CS);
					spi_transaction(continuous_read);
					spi_transaction(byte2);
					spi_transaction(byte1);
					spi_transaction(byte0);

					PORTC = spi_transaction(no_command);
					PORTB |= (1 << CS);
				}
				else
				{
					offset = 0;
					page++;
					pagecount++;
					if(pagecount > 255)
					{
						pagecount = 0;
						read_next_byte = 0;

					}
				}
			}
		}
	}
}

void get_bytes(unsigned int page, unsigned int offset, unsigned char *byte2, unsigned char *byte1, unsigned char *byte0)
{
	*byte0 = *byte1 = *byte2 = 0;

	*byte0 |= offset;
	*byte1 |= (offset >> 6);
	*byte1 |= (page << 2);
	*byte2 |= (page >> 2);
}















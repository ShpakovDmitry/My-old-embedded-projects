#include "spi.h"
#include "uart.h"

// определения для комманд
#define CMD0 0x40
#define CMD1 0x41
#define CMD8 0x48
#define CMD55 0x77
#define CMD58 0x7a
#define ACMD41 0x69

// определения для флагов ответа R1
#define in_reset_state		0x00
#define in_idle_state		0x01
#define erase_reset		0x02
#define illegal_command		0x04
#define com_crc_error		0x08
#define erase_sequence_error	0x10
#define address_error		0x20
#define parameter_error		0x40

// типы карт
#define not_a_card 0x01
#define Ver1_SC_SD 0x02
#define Ver2_SC_SD 0x04
#define Ver2_HC_SD 0x08


unsigned char card_type;


void sd_cmd(char b1, char b2, char b3, char b4, char b5, char b6)		// послать команду карте; команда состоит из 6-ти байтов
{
	spi_transaction(b1); // старт бит и индекс команды; 0 1 CMD;
	
	spi_transaction(b2); // аргумент команды
	spi_transaction(b3);
	spi_transaction(b4);
	spi_transaction(b5);
	
	spi_transaction(b6); // контрольная сумма и стоп бит: CRC 1;
}

unsigned char get_R1(unsigned char *b1)		// получить ответ R1 от карты; длина ответа 1 байт;
{
	unsigned char data;	// переменная для получения данных от карты
	unsigned int count = 0;	// переменная для ограничения времени ожидания ответа R1 от карты
	
	do
	{
		data = spi_transaction(0xff);
		count++;
	} while( ((data & 0x80) != 0) && (count < 0xffff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		return 1;
	}
	else				// в противном случае вернуть полученный ответ от карты
	{
		*b1 = data;
		return 0;
	}
}

// получить ответ R7 от карты; длина ответа 5 байт; b5 это R1;
unsigned char get_R7(unsigned char *b5, unsigned char *b4, unsigned char *b3, unsigned char *b2, unsigned char *b1)
{
	unsigned char data;
	unsigned int count = 0;
	
	do
	{
		data = spi_transaction(0xff);
		count++;
	}while( ( (data & 0x80) != 0 ) && (count < 0xffff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		return 1;
	}
	else				// в противном случае вернуть полученный ответ от карты
	{
		*b5 = data;
		*b4 = spi_transaction(0xff);
		*b3 = spi_transaction(0xff);
		*b2 = spi_transaction(0xff);
		*b1 = spi_transaction(0xff);
		
		return 0;
	}
}

// получить ответ R3 от карты; длина ответа 5 байт; b5 это R1;
unsigned char get_R3(unsigned char *b5, unsigned char *b4, unsigned char *b3, unsigned char *b2, unsigned char *b1)
{
	unsigned char data;
	unsigned int count = 0;
	
	do
	{
		data = spi_transaction(0xff);
		count++;
	}while( ( (data & 0x80) != 0 ) && (count < 0xffff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		return 1;
	}
	else				// в противном случае вернуть полученный ответ от карты
	{
		*b5 = data;
		*b4 = spi_transaction(0xff);
		*b3 = spi_transaction(0xff);
		*b2 = spi_transaction(0xff);
		*b1 = spi_transaction(0xff);
		
		return 0;
	}
}

char sd_init(void)		// инициализация карты
{
	unsigned char i;
	unsigned int n;
	unsigned char b1, b2, b3, b4, b5;
	unsigned char flag = 1;
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	
	for(i = 0; i < 100; i++)	// подача 800 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}
	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	
	sd_cmd(CMD0, 0x00, 0x00, 0x00, 0x00, 0x95);	// сброс карты памяти
	i = get_R1(&b1);
	if( (i != 0) || (b1 != in_idle_state) )		// если время ожидания ответа вышло или карта не в режиме ожидания, то выйти с ошибкой 1
	{
		return 1;
	}
	
	sd_cmd(CMD8, 0x00, 0x00, 0x01, 0xa3, 0x05);	// проверки рабочего напряжения карты + параметры 0x01 и 0xa3, которые карта должна вернуть
	i = get_R7(&b5, &b4, &b3, &b2, &b1);		// получение ответа R7 от карты
	if( i != 0 )	// если вышло время ожидания ответа R7 от карты, то выйти с ошибкой 2
	{
		return 2;
	}
	
	
	if( b5 & illegal_command )			// если карта не приняла комманду, то возможно карта Ver1_SC_SD
	{
		sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
		i = get_R1(&b1);
		
		if( i != 0 )	// если вышло время ожидания ответа R1 от карты, то выйти с ошибкой 3
		{
			return 3;
		}
		
		if( b1 & illegal_command)	// если карта нераспознала команду CMD55, то выйти с ошибкой 4
		{
			return 4;
		}
		
		
		sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);	// команда для инициализации карты
		i = get_R1(&b1);
		if( i != 0 )		// если вышло время ожидания ответа R1 от карты, то выйти с ошибкой 5
		{
			return 5;
		}
		if( b1 & illegal_command )	// вставлена не карта
		{
			card_type = not_a_card;
			return 6;
		}
		
		
		if(b1 == in_reset_state)	// если карта сбросилась b1 == in_reset_state, то карта Ver1_SC_SD
		{
			card_type = Ver1_SC_SD;
			return 0;
		}
		
		// если карта ещё не сбросилась b1 == in_idle_state, то отправка комманды ACMD41 пока не сбросится или выйдет вр.ож.
		if( b1 == in_idle_state )
		{
			for(n = 0; (n < 0xffff) && (b1 == in_idle_state); n++)
			{
				sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);
				i = get_R1(&b1);	// если вышло время ожидания ответа R1 от карты, то выйти с ошибкой 7
				if( i != 0 )
				{
					return 7;
				}
				
				sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);
				i = get_R1(&b1);	// если вышло время ожидания ответа R1 от карты, то выйти с ошибкой 8
				if( i != 0 )
				{
					return 8;
				}
			}
			
			if( b1 == in_reset_state )
			{
				card_type = Ver1_SC_SD;
				return 0;
			}
			else
			{
				return 9;
			}	
		}
		
		
		
	}
	else		// если карта приняла комманду, то возможно карта Ver2_SC_SD или Ver2_HC_SD
	{
		if( (b2 == 0x01) && (b1 == 0xa3) )	
		{
			for(n = 0; (n < 0xffff) && flag; n++)
			{
				sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);
				i = get_R1(&b1);
				if( i != 0 )
				{
					return 10;
				}
				
				sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);
				i = get_R1(&b1);
				if( i != 0 )
				{
					return 11;
				}
				if( b1 == in_reset_state )	// если карта сбросилась, то flag = 0 --> выход из цикла
				{
					flag = 0;
				}
			}
			
			if(  flag == 1)	// если flag == 1, то превышено количество попыток сбросить карту; n = 0xfffe
			{
				return 12;
			}
			
			
			// если карта сбросилась, то читаем регистр карты OCR командой CMD58;
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			if( i != 0 )
			{
				return 13;
			}
			if( b5 != 0 )	// если R1 != 0; поидее он должен быть равен 0 
			{
				return 14;
			}
			if( (b4 & 0x40) == 1 )
			{
				card_type = Ver2_SC_SD;
				return 0;
			}
			else
			{
				card_type = Ver2_HC_SD;
				return 0;
			}
			
		}
		else
		{
			if( b1 != 0xa3 )
			{
				return 15;
			}
			if( b2 != 0x01 )
			{
				card_type = not_a_card;
				return 16;
			}
		}
	}
	
	
}



/*char sd_init(void)
{
	char i, b = 0;
	char flag = 1;
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на SS
	
	for(i = 0; i < 100; i++)
	{
		spi_transaction(0xff);
	}
	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	
	sd_cmd(0x40, 0x00, 0x00, 0x00, 0x00, 0x95);	// посылка команды CMD0 сброса
	
	i = get_R1(&b);		// получение ответа от SD карты

	if( (i != 0) || (b != 0x01))	// если время ожидания вышло или карта не в режиме ожидания, то выйти из инициализации с ошибкой
	{
		return 1;
	}
	
	for(i = 0; flag && (i < 0xffff) ; i++)		// посылка команды ACMD41 = CMD55 + CMD41
	{
		sd_cmd(0x77, 0x00, 0x00, 0x00, 0x00, 0x95);	// посылка команды CMD55
		
		i = get_R1(&b);
		if( (i != 0) || ( b > 1 ))
		{
			return 2;
		}
		
		sd_cmd(0x69, 0x00, 0x00, 0x00, 0x00, 0x95);	// посылка команды CMD41 HCS = 1
		
		i = get_R1(&b);
		if( (i != 0))
		{
			return 3;
		}
		
		if(b == 0 )
		{
			flag = 0;
		}
	}
	if(flag == 1)
	{
		return 4;
	}
	
	
	sd_cmd(0x50, 0x00, 0x00, 0x02, 0x00, 0x95);
	
	i = get_R1(&b);
	if( i != 0 )
	{
		return 5;
	}
	
	return 0;
	
	
}
*/

/*
char sd_readblock(char buff[], unsigned long int block)
{
	char *x = (char *)&block;
	char b = 0;
	int i = 0;
	unsigned char data;
	
	sd_cmd(0x51, *(x + 3), *(x + 2), *(x + 1), *(x + 0), 0x95 );
	
	i = get_R1(&b);
	if( (i != 0) || (b != 0) )
	{
		return 1;
	}
	
	do
	{
		data = spi_transaction(0x00);
		i++;
	}while( data == 0xFF && i<0xfffe );
	
	if(data == 0xFF)
	{
		return 2;
	}
	
	if(data == 0xFD)
	{
		for(i = 0; i<512; i++)
		{
			*(buff + i) = spi_transaction(0x00);
		}
	}
	else
	{
		uart_putc(data);
		return 3;
	}
	
	
	return 0;
	
}*/




























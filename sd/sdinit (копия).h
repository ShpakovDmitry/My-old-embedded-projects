
// определения для комманд
#define CMD0 0x40
#define CMD1 0x41
#define CMD8 0x48
#define CMD55 0x77
#define CMD58 0x7a
#define ACMD41 0x69

// определения для флагов ответа R1
#define reset_state		0x00
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


void sd_cmd(char b1, char b2, char b3, char b4, char b5, char b6)		// послать команду карте
{	
	spi_transaction(b1); // старт бит и индекс команды; 0 1 CMD;
	
	spi_transaction(b2); // аргумент команды
	spi_transaction(b3);
	spi_transaction(b4);
	spi_transaction(b5);
	
	spi_transaction(b6); // контрольная сумма и стоп бит: CRC 1;
}

unsigned char get_R1(unsigned char *b1)		// получить ответ R1 от карты
{
	unsigned char data;
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

char sd_init(void)
{
	unsigned char i;
	unsigned char count = 0;
	unsigned int n;
	unsigned char b1, b2, b3, b4, b5;
	unsigned char flag = 1;
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	
	
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}
	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	
	do
	{
		sd_cmd(CMD0, 0x00, 0x00, 0x00, 0x00, 0x95);	// сброс карты памяти
		i = get_R1(&b1);
		/*if( (i != 0) || (b1 != in_idle_state) )	// если вр. ож. ответа вышло или карта не в режиме ожидания, то выйти с ошибкой 1
		{
			return 1;
		}*/
	}while( (count < 0xff) && (b1 != in_idle_state) )
	if(b1 != in_idle_state)
	{
		return 1;
	}
	
	n = 0;
	do
	{
		sd_cmd(CMD1, 0x00, 0x00, 0x00, 0x00, 0x01);
		i = get_R1(&b1);
		n++;
	}while( (b1 != 0x00) && (n < 0xff) );
	
	/*sd_cmd(CMD1, 0x00, 0x00, 0x00, 0x00, 0x95);	// инициировать процесс инициализации
	i = get_R1(&b1);
	if( (i != 0) )		// если время ожидания ответа вышло или карта не в режиме сброса, то выйти с ошибкой 2
	{
		return 2;
	}
	*/
	return 0;
	
	
}


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
		data = spi_transaction(0xff);
		i++;
	}while( data != 0xfe );
	
	if(data != 0xfe)
	{
		return 2;
	}
	
	if(data == 0xfe)
	{
		for(i = 0; i < 512; i++)
		{
			*(buff + i) = spi_transaction(0xff);
		}
	}
	else
	{
		uart_putc(data);
		return 3;
	}
	
	
	return 0;
	
}




























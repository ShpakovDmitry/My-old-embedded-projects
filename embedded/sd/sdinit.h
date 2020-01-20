
// определения для комманд
#define CMD0	0x40
#define CMD1	0x41
#define CMD8	0x48
#define CMD9	0x49
#define CMD10	0x4a
#define CMD55	0x77
#define CMD58	0x7a
#define ACMD41	0x69
#define ACMD51	0x73

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
#define not_a_card	0x01
#define unusable_card	0x02
#define Ver1_SC_SD	0x04
#define Ver2_SC_SD	0x08
#define Ver2_HC_SD	0x10


unsigned char card_type = not_a_card;

void sd_cmd(char b1, char b2, char b3, char b4, char b5, char b6)		// послать команду карте
{	
	uart_putc('\n');
	uart_putc('<');
	spi_transaction(b1); // старт бит и индекс команды; 0 1 CMD;
	
	spi_transaction(b2); // аргумент команды
	spi_transaction(b3);
	spi_transaction(b4);
	spi_transaction(b5);
	
	spi_transaction(b6); // контрольная сумма и стоп бит: CRC 1;
	
	//spi_transaction(0xff);
	
	uart_putc('>');
	uart_putc('\n');
}

unsigned char get_R1(unsigned char *b1)		// получить ответ R1 от карты
{
	unsigned char data;
	unsigned int count = 0;	// переменная для ограничения времени ожидания ответа R1 от карты
	
	do
	{
		data = spi_transaction(0xff);
		count++;
		// debug(data);
	} while( ((data & 0x80) != 0) && (count < 0xff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		*b1 = data;
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
		// debug(data);
	}while( ( (data & 0x80) != 0 ) && (count < 0xff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		*b5 = data;
		return 1;
	}
	else				// в противном случае вернуть полученный ответ от карты
	{
		*b5 = data;
		*b4 = spi_transaction(0xff);
		// debug(*b4);
		*b3 = spi_transaction(0xff);
		// debug(*b3);
		*b2 = spi_transaction(0xff);
		// debug(*b2);
		*b1 = spi_transaction(0xff);
		// debug(*b1);
		 
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
		// debug(data);
	}while( ( (data & 0x80) != 0 ) && (count < 0xff) );	// цикл пока старший бит data равен 1 и пока счётчик не превысил макс. значение
	
	if( (data & 0x80) != 0 )	// если старший бит data равен 1, то превышено заданное время ожидания ответа R1 от карты
	{
		*b5 = data;
		return 1;
	}
	else				// в противном случае вернуть полученный ответ от карты
	{
		*b5 = data;
		*b4 = spi_transaction(0xff);
		// debug(*b4);
		*b3 = spi_transaction(0xff);
		// debug(*b3);
		*b2 = spi_transaction(0xff);
		// debug(*b2);
		*b1 = spi_transaction(0xff);
		// debug(*b1);
		
		return 0;
	}
}

char sd_init(void)
{
	unsigned char i,j;
	unsigned char count = 0;
	unsigned int n ,count1 = 0;
	unsigned char b1, b2, b3, b4, b5;
	unsigned char flag = 1;
	
	
	j = 0;
	do
	{
		
		SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
			
		uart_puts("\nCMD0_start\n");
		count = 0;
		do
		{
			sd_cmd(CMD0, 0x00, 0x00, 0x00, 0x00, 0x95);	// сброс карты памяти
			i = get_R1(&b1);
			count++;
		}while( (count < 0x0f) && (b1 != in_idle_state) );
		uart_puts("\nCMD0_end\n");
		j++;
	}while( (j < 0x0f) && (b1 != in_idle_state) );
	
	if(b1 != in_idle_state)
	{
		return 1;
	}
	
	
	
	
	
	
	
	/*
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		count = 0;
		uart_puts("\nCMD1_start\n");
		do
		{
			sd_cmd(CMD1, 0x00, 0x00, 0x00, 0x00, 0xf9);
			i = get_R1(&b1);
			count++;
		}while( (count < 0xff) && (b1 != in_reset_state) );
		uart_puts("\nCMD1_end\n");
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		uart_puts("\nCMD58_start\n");
		do
		{
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			
				spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);
			
			count++;
		}while( (count < 0xff)  );
		uart_puts("\nCMD58_end\n");
	*/
	
	
	
	
	
	
	
	
	
	
	//*******************
	
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	count = 0;
	uart_puts("\nCMD8_start\n");
	do
	{
		//sd_cmd(CMD8, 0x00, 0x00, 0x01, 0xa3, 0x05);
		//sd_cmd(CMD8, 0x00, 0x00, 0x01, 0xaa, 0x87);
		sd_cmd(CMD8, 0x00, 0x00, 0x01, 0xe5, 0xa0);
		i = get_R7(&b5, &b4, &b3, &b2, &b1);
		count++;
	}while( (count < 0x0f) && (b5 != in_idle_state) && !(b5 & illegal_command));
	uart_puts("\nCMD8_end\n");
	
	if(b5 & illegal_command)
	{
		SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		
		count1 = 0;
		uart_puts("\nCMD55+ACMD41_start\n");
		do
		{
			sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
			i = get_R1(&b1);
			 uart_puts("*** ");
			sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);	// команда для инициализации карты
			i = get_R1(&b1);
			 uart_puts("--> ");
			
			count1++;
		}while( (b1 != in_reset_state) && (count1 < 0x200) );
		uart_puts("\nCMD55+ACMD41_end\n");
		if(b1 != in_reset_state)
		{
			return 2;
		}
		
		card_type = Ver1_SC_SD;	
			
	}
	else
	{
		if( (b2 != 0x01) || (b1 != 0xe5) )
		{
			return 3;
		}
		
		
		/*SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		uart_puts("\nCMD58_start\n");
		do
		{
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			
				spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);
			
			count++;
		}while( (count < 0x0f)  );
		uart_puts("\nCMD58_end\n");
		*/
		
		
		SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		
		count1 = 0;
		uart_puts("\nCMD55+ACMD41_start\n");
		do
		{
			sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
			i = get_R1(&b1);
			 uart_puts("*** ");
			sd_cmd(ACMD41, 0x40, 0x00, 0x00, 0x00, 0x77);	// команда для инициализации карты
			i = get_R1(&b1);
			 uart_puts("--> ");
			
			count1++;
		}while( (b1 != in_reset_state) && (count1 < 0x200) );
		uart_puts("\nCMD55+ACMD41_end\n");
		if(b1 != in_reset_state)
		{
			return 4;
		}
		
		
		
		SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		count = 0;
		uart_puts("\nCMD58_start\n");
		do
		{
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			
				spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);spi_transaction(0xff);
			
			count++;
		}while( (count < 0x0f) /*&& (b5 != in_reset_state)*/ );
		uart_puts("\nCMD58_end\n");
		
		if(b5 != in_reset_state)
		{
			return 5;
		}
		
		if( (b4 & 0x40) == 1 )
		{
			card_type = Ver2_SC_SD;
		}
		else
		{
			card_type = Ver2_HC_SD;
		}
		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*count1 = 0;
	do
	{
		SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
		for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
		{
			spi_transaction(0xff);
		}	
		SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
		
		uart_puts("\nCMD58_start\n");
		do
		{
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			count++;
		}while( (count < 0x0f) && (b5 != in_reset_state) );
		uart_puts("\nCMD58_end\n");
		count1++;
	}while(count1 < 0x0f);
	*/
	
	uart_putc('\n');
	debug(card_type);
	uart_putc('\n');
	
	return 0;
	
	
	
	
	/*
	else if ( (b2 != 0x01) || (b1 != 0xaa) )
	{
		card_type = not_a_card;
		return 1;
	}
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	uart_puts("\nCMD55+ACMD41_start\n");
	do
	{
		sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
		i = get_R1(&b1);
		// debug(b1);
		 uart_puts("*** ");
		sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);	// команда для инициализации карты
		i = get_R1(&b1);
		// debug(b1);
		 uart_puts("--> ");
		
		count1++;
	}while( (b1 != in_reset_state) && (count1 < 0x1f0) );
	uart_puts("\nCMD55+ACMD41_end\n");
	if(b1 != in_reset_state)
	{
		return 2;
	}
	
	if(card_type != Ver1_SC_SD)
	{
		init_clocks();
		
		uart_puts("\nCMD58_start\n");
		do
		{
			sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
			i = get_R3(&b5, &b4, &b3, &b2, &b1);
			// debug(b5);
			count++;
		}while( (count < 0x0f) && (b5 != in_reset_state) );
		uart_puts("\nCMD58_end\n");
		
		if(b5 != in_reset_state)
		{
			return 3;
		}
		
		if( (b4 & 0x40) == 1 )
			{
				card_type = Ver2_SC_SD;
			}
			else
			{
				card_type = Ver2_HC_SD;
			}
		
	}
	uart_puts("Card_type\n");
	uart_putc('A' - 1 + card_type);
	*/
	
	/*
	//SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	spi_transaction(0xff);
	spi_transaction(0xff);
	
	count = 0;
	//SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	uart_puts("\nCMD55+ACMD41_start\n");
	do
	{
		sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
		i = get_R1(&b1);
		// debug(b1);
		 uart_puts("*** ");
		sd_cmd(ACMD41, 0x00, 0x00, 0x00, 0x00, 0xe5);	// команда для инициализации карты
		i = get_R1(&b1);
		// debug(b1);
		 uart_puts("--> ");
		
		count++;
	}while( (b1 != in_reset_state) && (count < 0xff) );
	uart_puts("\nCMD55+ACMD41_end\n");
	if(b1 != in_reset_state)
	{
		return 2;
	}
	
	//SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	spi_transaction(0xff);
	spi_transaction(0xff);
	
	count = 0;
	//SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	uart_puts("\nCMD58_start\n");
	do
	{
		sd_cmd(CMD58, 0x00, 0x00, 0x00, 0x00, 0xfd);
		i = get_R3(&b5, &b4, &b3, &b2, &b1);
		// debug(b5);
		count++;
	}while( (count < 0xff) && (b5 != in_reset_state) );
	uart_puts("\nCMD58_end\n");
	
	return 0;
	*/
	
}

char read_CID(void)
{
	unsigned char data, i;
	unsigned int count = 0;
	
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	do
	{
		sd_cmd(CMD10, 0x00, 0x00, 0x00, 0x00, 0x1b);
		i = get_R1(&data);
		count++;
	}while((count < 0x0f) && (data != in_reset_state));
	
	if(data != in_reset_state)
	{
		return 1;
	}
	
	count = 0;
	do
	{
		data = spi_transaction(0xff);
		count++;
	}while( (data != 0xfe) && (count < 0x200) );
	
	if(data == 0xfe)
	{
		uart_puts("\nCID data start\n");
		do
		{
			data = spi_transaction(0xff);
			count++;
		}while(count < 512);
		uart_puts("\nCID data end\n");
		return 0;
	}
	else
	{
		return 2;
	}
	
}

char read_CSD(void)
{
	unsigned char data, i;
	unsigned int count = 0;
	
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	do
	{
		sd_cmd(CMD9, 0x00, 0x00, 0x00, 0x00, 0x1b);
		i = get_R1(&data);
		count++;
	}while((count < 0x0f) && (data != in_reset_state));
	
	if(data != in_reset_state)
	{
		return 1;
	}
	
	count = 0;
	do
	{
		data = spi_transaction(0xff);
		count++;
	}while( (data != 0xfe) && (count < 0x200) );
	
	if(data == 0xfe)
	{
		uart_puts("\nCSD data start\n");
		do
		{
			data = spi_transaction(0xff);
			count++;
		}while(count < 512);
		uart_puts("\nCSD data end\n");
		return 0;
	}
	else
	{
		return 2;
	}
	
}

char read_SCR(void)
{
	unsigned char data, i;
	unsigned int count = 0;
	
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	do
	{
		sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x1b);
		i = get_R1(&data);
		sd_cmd(ACMD51, 0x00, 0x00, 0x00, 0x00, 0x1b);
		i = get_R1(&data);
		count++;
	}while((count < 0x0f) && (data != in_reset_state));
	
	if(data != in_reset_state)
	{
		return 1;
	}
	
	count = 0;
	do
	{
		data = spi_transaction(0xff);
		count++;
	}while( (data != 0xfe) && (count < 0x200) );
	
	if(data == 0xfe)
	{
		uart_puts("\nSCR data start\n");
		do
		{
			data = spi_transaction(0xff);
			count++;
		}while(count < 512);
		uart_puts("\nSCR data end\n");
		return 0;
	}
	else
	{
		return 2;
	}
	
}



char sd_readblock(char buff[], unsigned long int block)
{
	char *x = (char *)&block;
	char b = 0;
	unsigned long int i = 0;
	unsigned char data, count = 0;
	unsigned int count1;
	
	
	count1 = 0;
	uart_puts("\nCMD55+ACMD41_start\n");
	do
	{
		sd_cmd(CMD55, 0x00, 0x00, 0x00, 0x00, 0x65);	// команда APP_CMD
		i = get_R1(&b);
		 uart_puts("*** ");
		sd_cmd(ACMD41, 0x40, 0x00, 0x00, 0x00, 0x77);	// команда для инициализации карты
		i = get_R1(&b);
		 uart_puts("--> ");
		
		count1++;
	}while( (b != in_reset_state) && (count1 < 0x200) );
	uart_puts("\nCMD55+ACMD41_end\n");
	if(b != in_reset_state)
	{
		return 4;
	}	
	
	
	
	SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	for(i = 0; i < 10; i++)	// подача 80 тактов на карту (по спецификации нужно ~80 не меньше)
	{
		spi_transaction(0xff);
	}
	
	SPI_PORT &= ~(1 << SPI_SS);	// низкий уровень на SS
	
	
	do
	{
		sd_cmd(0x51, *(x + 3), *(x + 2), *(x + 1), *(x + 0), 0x95 );
		i = get_R1(&b);
		count++;
	}while((count < 0x0f) && (b != in_reset_state));
	
	if( b != in_reset_state )
	{
		return 1;
	}
	
	//SPI_PORT |= (1 << SPI_SS);	// высокий уровень на выходе SS
	i = 0;
	do
	{
		data = spi_transaction(0xff);
		i++;
	}while( (data != 0xfe) && (i < 0xfff) );
	
	if(data != 0xfe)
	{
		return 2;
	}
	
	if(data == 0xfe)
	{
		for(i = 0; i < 530; i++)
		{
			*(buff + i) = spi_transaction(0xff);
		}
	}
	else
	{
		uart_putc(data);
		return 3;
	}
	
	spi_transaction(0xff);
	spi_transaction(0xff);
	
	spi_transaction(0xff);
	
	
	return 0;
	
}





























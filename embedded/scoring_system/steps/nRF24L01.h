#ifndef packet_length
 #define packet_length	32
#endif


#ifndef CS
 #define CS PB4
#endif

// определения относящиеся к радио приёмо-передатчику nRF24L01
#define CE  PB0
#define IRQ PB1
#define CSN PB3
	// комманды
#define R_REGISTER	0x00
#define W_REGISTER	0x20
#define R_RX_PAYLOAD	0x61
#define W_TX_PAYLOAD	0xa0
#define FLUSH_TX	0xe1
#define FLUSH_RX	0xe2
#define NOP		0xff

	// регистры
#define CONFIG		0x00
#define STATUS		0x07
#define RX_PW_P0	0x11
	// битовые маски
#define PWR_UP		1
#define PRIM_RX		0
		// STATUS register
#define RX_DR		6		
#define TX_DS		5
#define MAX_RT		4

#define COMMAND_delay 1	// задержка между посылками команд - мкс
#define startup_delay 1.5 // время выхода из спящего режима - мс
#define mode_enter_delay 150 // время в мкс для входа в режим приёмника или передатчика
#define transmit_delay	1 // время в мс для отсылки пакета данных

/*==============================Определения функций=======================================*/
void init_nRF24L01(void);					// инициализация радио приёмо-передатчика
unsigned char read_register_nRF24L01(const reg);
void write_register_nRF24L01(const reg, unsigned char data);
void receive_data_nRF24L01(unsigned char *dat);
void transmit_data_nRF24L01(unsigned char *dat);
void pwr_up_nRF24L01(void);
void pwr_down_nRF24L01(void);
void enter_receiver_mode_nRF24L01(void);
void enter_transmitter_mode_nRF24L01(void);




/*==============================Реализация функций=======================================*/
void receive_data_nRF24L01(unsigned char *dat)
{
	while(PORTB & (1 << CS) == 0)
	{
		;	// ждём пока flash память освободит SPI
	}
	unsigned char rdata, i;
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(R_RX_PAYLOAD);	// команда
		for(i = 0; i < packet_length; i++)
		{
			rdata = spi_transaction(NOP);
			dat[i] = rdata;
		}
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды
	
}

void transmit_data_nRF24L01(unsigned char *dat)
{
	unsigned char i, rdata;

	while(PORTB & (1 << CS) == 0)
	{
		;	// ждём пока flash память освободит SPI
	}
	
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(W_TX_PAYLOAD);	// команда
		for(i = 0; i < packet_length; i++)
		{
			rdata = spi_transaction(dat[i]);
		}
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды

	PORTB |= (1 << CE);
	_delay_us(10);
	PORTB &= ~(1 << CE);

	_delay_us(mode_enter_delay);

}

unsigned char read_register_nRF24L01(const reg)
{
	unsigned char rdata;

	while(PORTB & (1 << CS) == 0)
	{
		;	// ждём пока flash память освободит SPI
	}
	
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(R_REGISTER | reg);	// команда чтения из регистра: reg
		rdata = spi_transaction(NOP);	// считывание значения регистра
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды
	
	return rdata;	// возвращаем считанное значение
}

void write_register_nRF24L01(const reg, unsigned char data)
{
	unsigned char rdata;

	while(PORTB & (1 << CS) == 0)
	{
		;	// ждём пока flash память освободит SPI
	}
	
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(W_REGISTER | reg); // команда записи в регистр: reg
		rdata = spi_transaction(data);	// пишем data в регистр
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды
}

void pwr_up_nRF24L01(void)
{
	unsigned char rdata;
	
	rdata = read_register_nRF24L01(CONFIG);
	rdata |= (1 << PWR_UP);	// установка флага PWR_UP в 1
	write_register_nRF24L01(CONFIG, rdata);
	
	_delay_ms(startup_delay);	// ждём ~1.5 мс для выхода из спящего режима
}

void pwr_down_nRF24L01(void)
{
	unsigned char rdata;
	
	rdata = read_register_nRF24L01(CONFIG);
	rdata &= ~(1 << PWR_UP);	// установка флага PWR_UP в 0
	write_register_nRF24L01(CONFIG, rdata);
	
}

void enter_receiver_mode_nRF24L01(void)
{
	unsigned char rdata;
	
	PORTB &= ~(1 << CE);
	rdata = read_register_nRF24L01(CONFIG);
	rdata |= (1 << PRIM_RX);	// установка флага PRIM_RX в 1
	write_register_nRF24L01(CONFIG, rdata);
	PORTB |= (1 << CE);
	_delay_us(mode_enter_delay);



}

void enter_transmitter_mode_nRF24L01(void)
{
	unsigned char rdata;
	
	PORTB &= ~(1 << CE);
	rdata = read_register_nRF24L01(CONFIG);
	rdata &= ~(1 << PRIM_RX);	// установка флага PRIM_RX в 0
	write_register_nRF24L01(CONFIG, rdata);
}

void init_nRF24L01(void)
{
	unsigned char a;
	DDRB |= (1 << CSN) | (1 << CE);	// настроить как выходы
	PORTB |= (1 << CSN);		// высокий уровень на CSN
	PORTB &= ~(1 << CE);		// низкий уровень на CE

	write_register_nRF24L01(RX_PW_P0, packet_length); // установка размера конвейера равным 32-м байтам
	pwr_up_nRF24L01();
	enter_receiver_mode_nRF24L01();
	
	a = read_register_nRF24L01(STATUS);
	if(a & (1 << MAX_RT))
	{
		a |= (1 << MAX_RT);
		write_register_nRF24L01(STATUS, a);
		write_register_nRF24L01(FLUSH_TX, 0);
		write_register_nRF24L01(FLUSH_RX, 0);
	}
	else
	if(a & (1 << TX_DS))
	{
		a |= (1 << TX_DS);
		write_register_nRF24L01(STATUS, a);
		write_register_nRF24L01(FLUSH_TX, 0);
		write_register_nRF24L01(FLUSH_RX, 0);
	}
	else
	if(a & (1 << RX_DR))
	{
		a |= (1 << RX_DR);
		write_register_nRF24L01(STATUS, a);
		write_register_nRF24L01(FLUSH_TX, 0);
		write_register_nRF24L01(FLUSH_RX, 0);
	}



}


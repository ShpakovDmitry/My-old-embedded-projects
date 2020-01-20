// куда подключены управляющие выводы приёмопередатчика; SPI стандартные выводы
#define CSN PB0
#define CE  PB1
#define IRQ PB2

// комманды SPI
#define R_REGISTER 0x00
#define w_REGISTER 0x20

#define NOP 0xff

// регистры
#define CONFIG 		0x00
#define SETUP_AW	0x03
#define RX_PW_P0	0x11

// битовые маски
 //config
#define PWR_UP		0x01
#define PRIM_RX		0x00

// разные постоянные
#define COMMAND_delay 1	// задержка между посылками команд - мкс
#define startup_delay 1.5 // время выхода из спящего режима - мс

// используемые прототипы функций из других библиотек
void _delay_ms(double ms);
void _delay_us(double us);
void uart_puts(unsigned char *s);
unsigned char spi_transaction(unsigned char data);

// прототипы функций данной библиотеки
unsigned char read_register(const reg);
unsigned char write_register(const reg, unsigned char data);
unsigned char is_nRF24L01_connected(void);
unsigned char nRF24L01_interrupt(void);
void nRF24L01_init(unsigned char payload_len);
void pwr_up(void);
void pwr_down(void);
void enter_receiver_mode(void);
void enter_transmitter_mode(void);
unsigned char receive_data(unsigned char *data, unsigned char n);
unsigned char transmit_data(unsigned char *data, unsigned char n);




unsigned char nRF24L01_



unsigned char read_register(const reg)
{
	unsigned char rdata;
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(R_REGISTER | reg);	// команда чтения из регистра: reg
		rdata = spi_transaction(NOP);	// считывание значения регистра
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды
	
	return rdata;	// возвращаем считанное значение
}

write_register(const reg, unsigned char data)
{
	unsigned char rdata;
	PORTB &= ~(1 << CSN);
		rdata = spi_transaction(W_REGISTER | reg); // команда записи в регистр: reg
		rdata = spi_transaction(data);	// пишем data в регистр
	PORTB |= (1 << CSN);
	_delay_us(COMMAND_delay);	// ждём ~1 мкс для посылки след. комманды
}

unsigned char is_nRF24L01_connected(void)
{
	unsigned char rdata;
	
	rdata = read_register(SETUP_AW);
	if(rdata == 0x03)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char nRF24L01_interrupt(void)
{
	while(PINB)
}

void nRF24L01_init(unsigned char payload_len)
{
	unsigned char rdata;
	
	PORTB &= ~(1 << CE);
	if(payload_len > 32)
	{
		payload_len = 32;
	}
	
	write_register(RX_PW_P0, payload_len);
	
}

void pwr_up(void)
{
	unsigned char rdata;
	
	rdata = read_register(CONFIG);
	rdata |= (1 << PWR_UP);	// установка флага PWR_UP в 1
	write_register(CONFIG, rdata);
	
	_delay_ms(startup_delay);	// ждём ~1.5 мс для выхода из спящего режима
}

void pwr_down(void)
{
	unsigned char rdata;
	
	rdata = read_register(CONFIG);
	rdata &= ~(1 << PWR_UP);	// установка флага PWR_UP в 0
	write_register(CONFIG, rdata);
}

void enter_receiver_mode(void)
{
	unsigned char rdata;
	
	rdata = read_register(CONFIG);
	rdata |= (1 << PRIM_RX);	// установка флага PRIM_RX в 1
	write_register(CONFIG, rdata);
}

void enter_transmitter_mode(void)
{
	unsigned char rdata;
	
	rdata = read_register(CONFIG);
	rdata &= ~(1 << PRIM_RX);	// установка флага PRIM_RX в 0
	write_register(CONFIG, rdata);
}

unsigned char receive_data(unsigned char *data, unsigned char n)
{
	
}





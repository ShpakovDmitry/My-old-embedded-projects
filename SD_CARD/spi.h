#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SS   PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK  PB7


void spi_init_Master(void);
void spi_init_Slave(void);

char spi_transaction_Master(char c);
char spi_transaction_Slave(char c);

void spi_send_Master(char c);
char spi_receive_Slave(void);


void spi_init_Master(void)
{
	// настраиваем выводы MOSI, SCK, SS как выходы
	SPI_DDR = (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	// разрешаем SPI, Master, режим 4 - частота 1/128 F_CPU, старший бит первый(по умолчанию)
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);

}

void spi_init_Slave(void)
{
	// настраиваем вывод MISO на выход
	SPI_DDR = (1 << SPI_MISO);
	
	// разрешаем SPI, Slave
	SPCR = (1 << SPE);
}

char spi_transaction_Master(char c)
{
	char ret;
	
	SPI_PORT &= ~(1 << SPI_SS);	// сброс SS в 0
	SPDR = c;			// передаваемые данные
	while( !( SPSR & (1 << SPIF) ) )
		;			// ждём окончания передачи
	ret = SPDR; 			// считываем принятые данные
	SPI_PORT |= (1 << SPI_SS);	// выставляем SS в 1
	return ret;
}

char spi_transaction_Slave(char c)
{	
	SPDR = c;
	while( !( SPSR & (1 << SPIF) ) )
		;			// ждём окончания передачи
	return SPDR;
}

void spi_send_Master(char c)
{
	SPI_PORT &= ~(1 << SPI_SS); 
	SPDR = c;			// передаваемые данные
	while( !( SPSR & (1 << SPIF) ) )
		;			// ждём окончания передачи
	SPI_PORT |= (1 << SPI_SS);
}

char spi_receive_Slave(void)
{
	while( !( SPSR & (1 << SPIF) ) )
		;			// ждём окончания передачи
	return SPDR;
}












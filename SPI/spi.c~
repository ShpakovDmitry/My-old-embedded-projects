#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SS   PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK  PB7

void spi_init(void);
unsigned char spi_transaction(char c);


int main(void)
{
	
	
	return 0;
}


void spi_init(void)
{
	// настраиваем выводы MOSI, SCK, SS на выход
	SPI_DDR = (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	// выставляем SS в 1
	SPI_PORT |= (1 << SPI_SS);
	
	// разрешаем SPI, Master, режим 4, частота 1/128 F_CPU, старший бит первый
	SPCR = (1 << SPE) | (1 << SPR1) | (1 << SPR0);
}

unsigned char spi_transaction(char c)
{
	unsigned char ret;
	
	SPI_PORT &= ~(1 << SPI_SS);	// сброс SS в 0
	SPDR = c;			// передаваемые данные
	while( !( SPSR & (1 << SPIF) ) )
		;			// ждём окончания передачи
	ret = SPDR; 			// считываем принятые данные
	SPI_PORT |= (1 << SPI_SS);	// выставляем SS в 1
	return ret;
}








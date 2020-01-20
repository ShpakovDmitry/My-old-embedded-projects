// выводы для SPI
#define SPI_DDR  DDRB
#define SPI_PORT PORTB
#define SPI_SS   PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK  PB7


void spi_init(void);
void spi_speedup(void);
void spi_slowdown(void);
unsigned char spi_transaction(unsigned char c);	// без управления SS


void spi_init(void)	// инициализация SPI, без управления SS
{
	// настраиваем выводы MOSI, SCK, SS на выходы
	SPI_DDR = (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	// разрешаем SPI, МК <- Master, частота 1/32 F_CPU, старший бит первый
	SPSR = (1 << SPI2X);
	SPCR = (0 << SPIE) | (1 << SPE) | (0 << DORD) | (1 << MSTR) | (0 << CPOL) | (0 << CPHA) | (1 << SPR1) | (0 << SPR0); // короче SPCR = 0x42;
	// делитель частоты
	//	SPI2X	SPR1	SPR0	SCK_F		F_CPU=12MHz	F_CPU=1MHZ
	//	  0	 0	 0	F_CPU/4		3000.00	kHz	250.000	kHz
	//	  0	 0	 1	F_CPU/16	 750.00	kHz	 62.500	kHz
	//	  0	 1	 0	F_CPU/64	 187.50	kHz	 15.625	kHz
	//	  0	 1	 1	F_CPU/128	  93.75	kHz	  7.8125kHz
	//	  1	 0	 0	F_CPU/2		6000.00	kHz	500.000	kHz
	//	  1	 0	 1	F_CPU/8		1500.00	kHz	125.000	kHz
	//	  1	 1	 0	F_CPU/32	 375.00	kHz	 31.250	kHz
	//	  1	 1	 1	F_CPU/64	 187.50	kHz	 15.625	kHz

}

void spi_speedup(void)
{
	SPSR = (0 << SPI2X);
	//SPCR &= ~( (1 << SPR1) | (1 << SPR0) );
}

void spi_slowdown(void)
{
	SPSR = (1 << SPI2X);
	SPCR &= ~(1 << 0);
	SPCR |=  (1 << 1);
}

unsigned char spi_transaction(unsigned char c)	// без управления SS
{
	SPDR = c;	// передаваемые данные
	while( !( SPSR & (1 << SPIF) ) )
		;	// ждём окончания передачи
	 debug(SPDR);
	return SPDR;	// возвращаем принятые данные	
}


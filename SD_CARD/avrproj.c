#include <avr/io.h>
#include <util/delay.h>

//#include <avr/iom16.h>
//#include <avr/interrupt.h>

//#define FOSC 6400000

#define DI 6                         // Port B bit 6 (pin7): data in (data from MMC)
#define DT 5                        // Port B bit 5 (pin6): data out (data to MMC)
#define CLK 7                     // Port B bit 7 (pin8): clock
#define CS 4                        // Port B bit 4 (pin5): chip select for MMC

char chars[512];

void ini_SPI(void) {
     DDRB &= ~(_BV(DI));                     //input
     DDRB |= _BV(CLK);                     //outputs
     DDRB |= _BV(DT);                     //outputs
     DDRB |= _BV(CS);                     //outputs
     SPCR |= _BV(SPE);                     //SPI enable
     SPCR |= _BV(MSTR);                     //Master SPI mode
     SPCR &= ~(_BV(SPR1));                    //fosc/16
     SPCR |= _BV(SPR0);                    //fosc/16
     SPSR &= ~(_BV(SPI2X));                    //speed is not doubled
     PORTB &= ~(_BV(CS));                     //Enable CS pin for the SD card
}

char SPI_sendchar(char chr) {
     char receivedchar = 0;
     SPDR = chr;
     while(!(SPSR & (1<<SPIF)));
     receivedchar = SPDR;
     return (receivedchar);
}

char Command(char cmd, uint16_t ArgH, uint16_t ArgL, char crc ) {
     SPI_sendchar(0xFF);
     SPI_sendchar(cmd);
     SPI_sendchar((uint8_t)(ArgH >> 8));
     SPI_sendchar((uint8_t)ArgH);
     SPI_sendchar((uint8_t)(ArgL >> 8));
     SPI_sendchar((uint8_t)ArgL);
     SPI_sendchar(crc);
     SPI_sendchar(0xFF);
     return SPI_sendchar(0xFF);                // Returns the last byte received
}

void ini_SD(void) {
     char i;
     PORTB |= _BV(CS);                    //disable CS
     for(i=0; i < 10; i++) 
     	SPI_sendchar(0xFF);                // Send 10 * 8 = 80 clock pulses 400 kHz
     PORTB &= ~(_BV(CS));                 //enable CS
     for(i=0; i < 2; i++) 
       SPI_sendchar(0xFF);                // Send 2 * 8 = 16 clock pulses 400 kHz
     Command(0x40,0,0,0x95);              // reset
     PORTA = 0x01;
     _delay_ms(1000);
idle_no:
     if (Command(0x41,0,0,0xFF) !=0)
     { 
     	PORTA = 0x00;
     	goto idle_no;                        //idle = L?
     }
     SPCR &= ~(_BV(SPR0));                //fosc/4
}


int read(void) {

    int i;
    uint16_t ix;
    char r1 =  Command(0x51,0,512,0xFF);
   
    for (ix = 0; ix < 50000; ix++) {
        if (r1 == (char)0x00) break;
        r1 = SPI_sendchar(0xFF);
    }
    if (r1 != (char)0x00) {
    return 1;
    }
    
    //read from the card will start after the framework
    while(SPI_sendchar(0xFF) != (char)0xFE);
    PORTA = 0x01;
    for(i=0; i < 512; i++) {
        while(!(SPSR & (1<<SPIF)));
        chars[i] = SPDR;
        SPDR = SPI_sendchar(0xFF);
    }
    SPI_sendchar(0xFF);
    SPI_sendchar(0xFF);

    return 0;

}

 

int main(void) {

	DDRA = 0x01;
	PORTA = 0x00;
    ini_SPI();

    ini_SD();

    //sei();

    read();

	while(1)
 		;

    return 0;

}



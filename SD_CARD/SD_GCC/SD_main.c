//**********************************************************
// **** MAIN routine FOR Interfacing microSD/SD CARD ******
//**********************************************************
//Controller: ATmega32 (8 Mhz internal)
//Compiler	: AVR-GCC
//Version	: 2.2
//Author	: CC Dharmani, Chennai (India)
// 			  www.dharmanitech.com
//Date		: 15 July 2009
//********************************************************

//Link to the Post: http://www.dharmanitech.com/2009/01/sd-card-interfacing-with-atmega8-fat32.html

#define F_CPU 12000000UL		//freq 12 MHz
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI_routines.c"
#include "SD_routines.c"
#include "UART_routines.c"
#include "FAT32.c"


void port_init(void)
{
PORTA = 0x00;
DDRA  = 0x00;
PORTB = 0xEF;
DDRB  = 0xBF; //MISO line i/p, rest o/p
PORTC = 0x00;
DDRC  = 0x00;
PORTD = 0x00;
DDRD  = 0xFE;
}

//UART0 initialize
// desired baud rate: 19200
// actual: baud rate:19231 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
/*
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = (1 << URSEL) | 0x06;
 UBRRL = 0x02; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x18;
*/
	// настройка скорости обмена
	
	//UBRRH = 0x04; UBRRL = 0xE1; // 50 бод
	//UBRRH = 0x02; UBRRL = 0x37; // 110 бод
	//UBRRH = 0x01; UBRRL = 0xf3; // 200 бод
	//UBRRH = 0x00; UBRRL = 0x67; // 600 бод
	//UBRRH = 0x00; UBRRL = 0x02; // 19200 бод
	//UBRRH = 0x00; UBRRL = 0x00; // 57600 бод
	UBRRH = 0x00; UBRRL = 0x02; // 500000 бод; 12 MHz; x2
	
	
	// 8 бит данных, 1 стоп бит, без контроля чётности
	UCSRC = ( 1 << URSEL ) | ( 1 << UCSZ1 ) | ( 1 << UCSZ0 );// | ( 1 << UPM1 ) | ( 1 << UPM0 );
	
	// разрешить приём и передачу данных
	UCSRB = ( 1 << TXEN ) | ( 1 << RXEN ) ;
	
	// удвоенная скорость
	UCSRA = (1 << U2X); // |(1 << UDRE)
}

//call this routine to initialize all peripherals
void init_devices(void)
{
 cli();
 port_init();
 spi_init();
 uart0_init();

 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x00; //timer interrupt sources
 //all peripherals are now initialized
}

//*************************** MAIN *******************************//
int main(void)
{
unsigned char option, error, data, FAT32_active;
unsigned int i;
unsigned char fileName[13];

_delay_ms(100);  //delay for VCC stabilization

init_devices();

PORTD |= 0x04; //switching ON the LED (for testing purpose only)

/*TX_NEWLINE;
TX_NEWLINE;
transmitString_F (PSTR("***********************************"));
TX_NEWLINE;
transmitString_F (PSTR(" Dharmani's microSD Card Testing.."));
TX_NEWLINE;
transmitString_F (PSTR("***********************************"));
TX_NEWLINE;
*/

SD_init();
SPI_HIGH_SPEED;	//SCK - 4 MHz
_delay_ms(1);   //some delay

if( SD_readSingleBlock(0) != 0 )
{
	transmitString_F (PSTR("Error\n"));
}
//transmitString_F (PSTR("FlaG\n"));
for(i = 0; i < 512; i++ )
{
	transmitByte(buffer[i]);
	//transmitString_F (PSTR("FlaG2\n"));
	//transmitString_F(buffer[i]);
	//transmitString_F (PSTR(buffer));
}

while(1)
{
	
}

/*
FAT32_active = 1;

error = getBootSectorData (); //read boot sector and keep necessary data in global variables
if(error) 	
{
  transmitString_F (PSTR("FAT32 not found!"));  //FAT32 incompatible drive
  FAT32_active = 0;
}

while(1)
{
TX_NEWLINE;
transmitString_F(PSTR("Press any key..."));
TX_NEWLINE;
option = receiveByte();
TX_NEWLINE;
transmitString_F(PSTR("> 0 : Erase Blocks"));
TX_NEWLINE;
transmitString_F(PSTR("> 1 : Write single Block"));
TX_NEWLINE;
transmitString_F(PSTR("> 2 : Read single Block"));

#ifndef FAT_TESTING_ONLY
TX_NEWLINE;
transmitString_F(PSTR("> 3 : Write multiple Blocks"));
TX_NEWLINE;
transmitString_F(PSTR("> 4 : Read multiple Blocks"));
#endif


TX_NEWLINE;
transmitString_F(PSTR("> 5 : Get file list"));
TX_NEWLINE;
transmitString_F(PSTR("> 6 : Read File"));
TX_NEWLINE;
transmitString_F(PSTR("> 7 : Write File"));
TX_NEWLINE;
transmitString_F(PSTR("> 8 : Delete File"));
TX_NEWLINE;
transmitString_F(PSTR("> 9 : Read SD Memory Capacity (Total/Free)"));

TX_NEWLINE;
TX_NEWLINE;
transmitString_F(PSTR("> Select Option (0-9): "));


/*WARNING: If option 0, 1 or 3 is selected, the card may not be detected by PC/Laptop again,
as it disturbs the FAT format, and you may have to format it again with FAT32.
This options are given for learnig the raw data transfer to & from the SD Card*/

/*
option = receiveByte();
transmitByte(option);

if(option >=0x35 && option <=0x39)  //options 5 to 9 disabled if FAT32 not found
{
  if(!FAT32_active) 
  {
    TX_NEWLINE;
    TX_NEWLINE;
	transmitString_F(PSTR("FAT32 options disabled!"));
	continue;
  } 
}


if((option >= 0x30) && (option <=0x34)) //get starting block address for options 0 to 4
{
TX_NEWLINE;
TX_NEWLINE;
transmitString_F(PSTR("Enter the Block number (0000-9999):"));
data = receiveByte(); transmitByte(data);
startBlock = (data & 0x0f) * 1000;
data = receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 100;
data = receiveByte(); transmitByte(data);
startBlock += (data & 0x0f) * 10;
data = receiveByte(); transmitByte(data);
startBlock += (data & 0x0f);
TX_NEWLINE;
}

totalBlocks = 1;

#ifndef FAT_TESTING_ONLY

if((option == 0x30) || (option == 0x33) || (option == 0x34)) //get total number of blocks for options 0, 3 or 4
{
TX_NEWLINE;
TX_NEWLINE;
transmitString_F(PSTR("How many blocks? (000-999):"));
data = receiveByte(); transmitByte(data);
totalBlocks = (data & 0x0f) * 100;
data = receiveByte(); transmitByte(data);
totalBlocks += (data & 0x0f) * 10;
data = receiveByte(); transmitByte(data);
totalBlocks += (data & 0x0f);
TX_NEWLINE;
}
#endif

switch (option)
{
case '0': //error = SD_erase (block, totalBlocks);
          error = SD_erase (startBlock, totalBlocks);
          TX_NEWLINE;
          if(error)
              transmitString_F(PSTR("Erase failed.."));
          else
              transmitString_F(PSTR("Erased!"));
          break;

case '1': TX_NEWLINE;
          transmitString_F(PSTR(" Enter text (End with ~):"));
          i=0;
            do
            {
                data = receiveByte();
                transmitByte(data);
                buffer[i++] = data;
                if(data == 0x0d)
                {
                    transmitByte(0x0a);
                    buffer[i++] = 0x0a;
                }
                if(i == 512) break;
            }while (data != '~');

            error = SD_writeSingleBlock (startBlock);
            TX_NEWLINE;
            TX_NEWLINE;
            if(error)
                transmitString_F(PSTR("Write failed.."));
            else
                transmitString_F(PSTR("Write successful!"));
            break;

case '2': error = SD_readSingleBlock (startBlock);
          TX_NEWLINE;
          if(error)
            transmitString_F(PSTR("Read failed.."));
          else
          {
            for(i=0;i<512;i++)
            {
                if(buffer[i] == '~') break;
                transmitByte(buffer[i]);
            }
            TX_NEWLINE;
            TX_NEWLINE;
            transmitString_F(PSTR("Read successful!"));
          }

          break;
//next two options will work only if following macro is cleared from SD_routines.h
#ifndef FAT_TESTING_ONLY

case '3': 
          error = SD_writeMultipleBlock (startBlock, totalBlocks);
          TX_NEWLINE;
          if(error)
            transmitString_F(PSTR("Write failed.."));
          else
            transmitString_F(PSTR("Write successful!"));
          break;

case '4': error = SD_readMultipleBlock (startBlock, totalBlocks);
          TX_NEWLINE;
          if(error)
            transmitString_F(PSTR("Read failed.."));
          else
            transmitString_F(PSTR("Read successful!"));
          break;
#endif

case '5': TX_NEWLINE;
  		  findFiles(GET_LIST,0);
          break;

case '6': 
case '7': 
case '8': TX_NEWLINE;
		  TX_NEWLINE;
          transmitString_F(PSTR("Enter file name: "));
          for(i=0; i<13; i++)
			fileName[i] = 0x00;   //clearing any previously stored file name
          i=0;
          while(1)
          {
            data = receiveByte();
            if(data == 0x0d) break;  //'ENTER' key pressed
			if(data == 0x08)	//'Back Space' key pressed
	 		{ 
	   			if(i != 0)
	   			{ 
	     			transmitByte(data);
					transmitByte(' '); 
	     			transmitByte(data); 
	     			i--; 
	   			} 
	   			continue;     
	 		}
			if(data <0x20 || data > 0x7e) continue;  //check for valid English text character
			transmitByte(data);
            fileName[i++] = data;
            if(i==13){transmitString_F(PSTR(" file name too long..")); break;}
          }
          if(i>12) break;
       
	      TX_NEWLINE;
		  if(option == '6')
		     readFile( READ, fileName);
		  if(option == '7')
		  	 writeFile(fileName);
 		  if(option == '8')
		     deleteFile(fileName);
          break;

case '9': memoryStatistics();
          break;

default: TX_NEWLINE;
         TX_NEWLINE;
         transmitString_F(PSTR(" Invalid option!"));
         TX_NEWLINE;
}

TX_NEWLINE;
}
*/
return 0;
}
//********** END *********** www.dharmanitech.com *************

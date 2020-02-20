#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define CARRIER_ON	(1 << PD5)
#define CARRIER_OFF	(0 << PD5)
#define DOT_DURATION 150
#define MAX_REPEAT 5
#define RED_LED PD3
#define GREEN_LED PD4

const char morsePhraseToPlay[] = "472";

typedef struct{
	char symbol;
	char *code;
}MorseCode;


unsigned char carrier = CARRIER_OFF;

void playUnit(){
	unsigned char i;
	
	for(i = 0; i < DOT_DURATION; i++)
	{
		PORTD ^= ((1 << PD5) & carrier);
		_delay_ms(1);
	}
}

void playDot(){
	carrier = CARRIER_ON;
	playUnit();
	carrier = CARRIER_OFF;
}

void playDash(){
	carrier = CARRIER_ON;
	playUnit(); playUnit(); playUnit();
	carrier = CARRIER_OFF;
}

void playShortSpace(){
	playUnit();
}

void playLetterSpace(){
	playUnit(); playUnit(); playUnit();
}

void playWordSpace(){
	playUnit(); playUnit(); playUnit(); playUnit(); playUnit(); playUnit(); playUnit();
}

char* findMorseChar (char letter) {
	static const MorseCode morseAlphabet[] = { 
		{'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
		{'4', "....-"}, {'5', "....."},	{'6', "-...."}, {'7', "--..."},
		{'8', "---.."}, {'9', "----."},	{'A',    ".-"}, {'B',  "-..."},
		{'C',  "-.-."}, {'D',   "-.."}, {'E',     "."}, {'F',  "..-."},
		{'G',   "--."}, {'H',  "...."}, {'I',    ".."}, {'J',  ".---"},
		{'K',   "-.-"}, {'L',  ".-.."}, {'M',    "--"}, {'N',    "-."},
		{'O',   "---"}, {'P',  ".--."}, {'Q',  "--.-"}, {'R',   ".-."},
		{'S',   "..."}, {'T',     "-"}, {'U',   "..-"}, {'V',  "...-"},
		{'W',   ".--"}, {'X',  "-..-"}, {'Y',  "-.--"}, {'Z',  "--.."},
		{ 0, NULL}
	};
	
	unsigned char low, mid, high;
	
	low = 0;
	high = (unsigned char) ( sizeof(morseAlphabet) / sizeof(morseAlphabet[0]) ) - 1;
	
	while (low <= high) {
		mid = (low + high) / 2;
		
		if ( letter < morseAlphabet[mid].symbol )
			high = mid - 1;
		else if (letter > morseAlphabet[mid].symbol )
			low = mid + 1;
		else
			return morseAlphabet[mid].code;
	}
	
	return NULL;
}

void playMorseLetter(char letter){
	char* str;
	
	str = findMorseChar(letter);
	
	for(unsigned char i = 0; str[i] != '\0'; i++){
		switch( str[i] ){
			case '.' :
				playDot();
				break;
			case '-' :
				playDash();
				break;
		}
		playShortSpace();
	}
	
	return;
}

void playMorsePhrase(const char* morsePhraseToPlay){
		for(unsigned char i = 0; morsePhraseToPlay[i] != '\0'; i++){
			playMorseLetter(morsePhraseToPlay[i]);
			playLetterSpace();
		}
		
		playWordSpace();
		
		return;
}


unsigned char playFlag = 0;
unsigned char button = 0;

unsigned char button1 = ~(1 << PB6);
unsigned char button2 = ~(1 << PB1);
unsigned char button3 = ~(1 << PB4);

ISR(PCINT_vect){
	static unsigned char i = 0;
	unsigned char data = PINB;
	
	// delay while noise finishes and clear interrupt flag
	// to do not cause extra interrupt
	_delay_ms(500);
	EIFR |= 1 << PCIF;
	
	if(data == 0xff){
		return;
	}
	
	if(!(data & 0x01) || !(data & 0x02) || !(data & 0x04) || !(data & 0x08) || \
	   !(data & 0x10) || !(data & 0x20) || !(data & 0x40) || !(data & 0x80)){
			if(i == 0){
				PORTD &= ~( (1 << RED_LED) | (1 << GREEN_LED) );
				if(data == button1){
					i = 1;
				}
				else{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 1){
				if(data == button2){
					i = 2;
				}
				else if(data == button1){
					i = 1;
				}
				else{
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if(i == 2){
				if(data == button3){
					playFlag = 1;
					i = 0;
					PORTD |= (1 << GREEN_LED);
				}
				else if(data == button1){
					i = 1;
				}
				else{
					i = 0;
					PORTD |= (1 << RED_LED);
					return;
				}
			}
			
	}
	else{
		i = 0;
	}
	
	if(playFlag){
		playFlag = MAX_REPEAT;
		for(;playFlag > 0; playFlag--){
			playMorsePhrase(morsePhraseToPlay);
			
		}
	}
}


void main(){
	PORTD &= ~( (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED) );
	DDRD |= (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED);
	PORTB = 0xff;
	DDRB = 0x00;
	unsigned char k;
	
	PCMSK = (1 << PCINT7) | (1 << PCINT6) | (1 << PCINT5) | (1 << PCINT4) | \
		 (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1) | (1 << PCINT0);
	GIMSK |= (1 << PCIE);
	SREG |= (1 << 7);
	
	while(1){
		;
	}
}








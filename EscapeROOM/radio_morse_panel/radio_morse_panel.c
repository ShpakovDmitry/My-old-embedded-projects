#include <stdio.h>
#include <stdbool.h>	// true, false
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>		// uint32_t;

#define CARRIER_ON	(1 << PD5)
#define CARRIER_OFF	(0 << PD5)
#define DOT_DURATION 150
#define MAX_REPEAT 5
#define RED_LED PD3
#define GREEN_LED PD4

static volatile uint32_t msFromReset;

const char morsePhraseToPlay[] = "472";

unsigned char carrier = CARRIER_OFF;

void playUnit() {
	unsigned char i;
	
	for ( i = 0; i < DOT_DURATION; i++ ) {
		PORTD ^= ( (1 << PD5) & carrier );
		_delay_ms(1);
	}
}

void playDot() {
	carrier = CARRIER_ON;
	playUnit();
	carrier = CARRIER_OFF;
}

void playDash() {
	carrier = CARRIER_ON;
	playUnit(); playUnit(); playUnit();
	carrier = CARRIER_OFF;
}

void playShortSpace() {
	playUnit();
}

void playLetterSpace() {
	playUnit(); playUnit(); playUnit();
}

void playWordSpace() {
	playUnit(); playUnit(); playUnit(); playUnit(); playUnit(); playUnit(); playUnit();
}

char* findMorseChar (char letter) {
	typedef struct {
		char symbol;
		char *code;
	} MorseCode;
	
	static const MorseCode morseAlphabet[] = { 
		{' ',    NULL},
		{'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
		{'4', "....-"}, {'5', "....."},	{'6', "-...."}, {'7', "--..."},
		{'8', "---.."}, {'9', "----."},	{'A',    ".-"}, {'B',  "-..."},
		{'C',  "-.-."}, {'D',   "-.."}, {'E',     "."}, {'F',  "..-."},
		{'G',   "--."}, {'H',  "...."}, {'I',    ".."}, {'J',  ".---"},
		{'K',   "-.-"}, {'L',  ".-.."}, {'M',    "--"}, {'N',    "-."},
		{'O',   "---"}, {'P',  ".--."}, {'Q',  "--.-"}, {'R',   ".-."},
		{'S',   "..."}, {'T',     "-"}, {'U',   "..-"}, {'V',  "...-"},
		{'W',   ".--"}, {'X',  "-..-"}, {'Y',  "-.--"}, {'Z',  "--.."},
		{ 0,     NULL}
	};
	
	unsigned char low, mid, high;
	
	low = 0;
	high = (unsigned char) ( sizeof(morseAlphabet) / sizeof(morseAlphabet[0]) ) - 1;
	
	while (low <= high) {
		mid = (low + high) / 2;
		
		if ( letter < morseAlphabet[mid].symbol ) {
			high = mid - 1;
		} else if (letter > morseAlphabet[mid].symbol ) {
			low = mid + 1;
		} else {
			return morseAlphabet[mid].code;
		}
	}
	
	return NULL;
}

void playMorseLetter (char letter) {
	char* str = findMorseChar( letter );
	
	if ( letter == ' ' ) {
		playWordSpace();
	}

	if ( str == NULL ) {
		return;
	}
	
	for (unsigned char i = 0; str[i] != '\0'; i++) {
		switch ( str[i] ) {
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

void playMorsePhrase (const char* morsePhraseToPlay) {
		for (unsigned char i = 0; morsePhraseToPlay[i] != '\0'; i++) {
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

enum ButtonState { PRESSED, RELEASED, HOLD };

typedef struct {
	unsigned char pin : 3;
	unsigned char isPressed : 1;
	unsigned char isReleased : 1;
	unsigned char isHold : 1;
	unsigned char lastState : 2;
} Button;

Button buttons[] = {
	{ PB0, false, true, false, RELEASED },
	{ PB1, false, true, false, RELEASED },
	{ PB2, false, true, false, RELEASED },
	{ PB3, false, true, false, RELEASED },
	{ PB4, false, true, false, RELEASED },
	{ PB5, false, true, false, RELEASED },
	{ PB6, false, true, false, RELEASED },
	{ PB7, false, true, false, RELEASED }
};

ISR (PCINT_vect) {
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
	   !(data & 0x10) || !(data & 0x20) || !(data & 0x40) || !(data & 0x80)) {
			if (i == 0) {
				PORTD &= ~( (1 << RED_LED) | (1 << GREEN_LED) );
				if (data == button1) {
					i = 1;
				}
				else {
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if (i == 1) {
				if (data == button2) {
					i = 2;
				}
				else if (data == button1) {
					i = 1;
				}
				else {
					i = 0;
					PORTD |= (1 << RED_LED);
				}
				
				return;
			}
			else if (i == 2) {
				if (data == button3) {
					playFlag = 1;
					i = 0;
					PORTD |= (1 << GREEN_LED);
				}
				else if (data == button1) {
					i = 1;
				}
				else {
					i = 0;
					PORTD |= (1 << RED_LED);
					return;
				}
			}
			
	}
	else {
		i = 0;
	}
	
	if (playFlag) {
		playFlag = MAX_REPEAT;
		for (;playFlag > 0; playFlag--) {
			playMorsePhrase(morsePhraseToPlay);
		}
	}
}

ISR (TIMER0_COMPA_vect) {
	msFromReset++;
}

void initTimer0(void) {
 	//init TIMER0 in CTC mode, to generate 1ms frequency interrupt, assuming that F_CPU = 16 MHz
	TCCR0A |= (1 << WGM01);
	// 1/64 F_CPU prescaler
	TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
	//
	OCR0A = 250;
	// enable output compare A match interrupt
	TIMSK |= (1 << OCIE0A);
	// start counting ms at this point; actually when glob interrupt flag is set
	msFromReset = 0;   
}

void main() {
	PORTD &= ~( (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED) );
	DDRD |= (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED);
	PORTB = 0xff;
	DDRB = 0x00;
	unsigned char k;
    
    initTimer0();
	
	PCMSK = (1 << PCINT7) | (1 << PCINT6) | (1 << PCINT5) | (1 << PCINT4) | \
		 (1 << PCINT3) | (1 << PCINT2) | (1 << PCINT1) | (1 << PCINT0);
	GIMSK |= (1 << PCIE);
	SREG |= (1 << 7);
	
	while(1) {
		;
	}
}








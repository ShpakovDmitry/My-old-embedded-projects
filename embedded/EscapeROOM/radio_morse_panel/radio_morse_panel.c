#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#define CARRIER_ON	(1 << PD5)
#define CARRIER_OFF	(0 << PD5)
#define DOT_DURATION 150
#define MAX_REPEAT 5
#define RED_LED PD3
#define GREEN_LED PD4

static volatile uint32_t msFromReset;

const char morsePhraseToPlay[] = "472";
#define MAX_PASS_LEN 5
const char PASSWORD[MAX_PASS_LEN] = "357\0";

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

enum ButtonState { PRESSED, RELEASED };

typedef struct {
	unsigned char pin : 3;
	char keyChar;
} Button;

Button buttons[] = {
	{ PB0, '1'},
	{ PB1, '2'},
	{ PB2, '3'},
	{ PB3, '4'},
	{ PB4, '5'},
	{ PB5, '6'},
	{ PB6, '7'},
	{ PB7, '8'}
};

#define BTN_1_REL (RELEASED << PB0)
#define BTN_2_REL (RELEASED << PB1)
#define BTN_3_REL (RELEASED << PB2)
#define BTN_4_REL (RELEASED << PB3)
#define BTN_5_REL (RELEASED << PB4)
#define BTN_6_REL (RELEASED << PB5)
#define BTN_7_REL (RELEASED << PB6)
#define BTN_8_REL (RELEASED << PB7)

#define ALL_RELEASED	BTN_1_REL | BTN_2_REL | BTN_3_REL | BTN_4_REL | \
						BTN_5_REL | BTN_6_REL | BTN_7_REL | BTN_8_REL

uint32_t debounceTime = 100;

// cyclic buffer implementation
typedef struct {
	unsigned char pos;
	unsigned char size;
	char buff[MAX_PASS_LEN];
}CyclicBuffer;

// functions to handle cyclic buffer
void initCyclicBuffer(CyclicBuffer *buff);
void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch);
bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq);

CyclicBuffer cyclicBuffer;

void initCyclicBuffer(CyclicBuffer *buff) {
	buff->pos = 0;
	buff->size = MAX_PASS_LEN;
	memset (buff->buff, 0, buff->size);
}

void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch) {
	buff->buff[buff->pos] = ch;
	buff->pos++;
	if (buff->pos >= buff->size) {
		buff->pos = 0;
	}
}

bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq) {
	unsigned char i;
	bool flag = true;
	
	for(i = 0; i < buff->size && seq[i] != '\0'; ++i)
	{
		if( buff->buff[(buff->pos + i) % (buff->size)] != seq[i] )
			flag = false;
	}
	
	return flag;
}


ISR (TIMER0_COMPA_vect) {
	msFromReset++;
}

bool isOneBitSet (uint8_t x) {
	return ( x && !(x & (x - 1)) ) ? true : false;
}

uint8_t getSetBitPosition (uint8_t x) {
	uint8_t pos = 0;
	
	while ( x != 1 ) {
		x >>= 1;
		pos++;
	}
	return pos;
}

uint32_t getDeltaTime (uint32_t t1, uint32_t t0) {
	if (t1 > t0) {
		return t1 - t0;
	}
	else if (t1 < t0) {
		uint32_t tmp = UINT32_MAX - t0;
		return tmp + t1 + 1;
	}
	else {
		return (uint32_t)0;
	}
}

void updateButtonState(void) {
	static uint32_t oldMs = 0;
	
	if (oldMs != msFromReset) {
		static unsigned char currButtonState = ALL_RELEASED;
		static unsigned char lastButtonState = ALL_RELEASED;
		static uint32_t lastDebounceTime = 0;
		
		oldMs = msFromReset;
		unsigned char cachedPinB = PINB;
		
		if (cachedPinB != lastButtonState) {
			lastDebounceTime = msFromReset;
		}
		
		uint32_t deltaTime = getDeltaTime(msFromReset, lastDebounceTime);
		
		if ( deltaTime > debounceTime ) {
			if (cachedPinB != currButtonState) {
				currButtonState = cachedPinB;
				
				if ( isOneBitSet(~currButtonState) == true ) {
					uint8_t i = getSetBitPosition( ~currButtonState );
					addCharToCyclicBuffer(&cyclicBuffer, buttons[i].keyChar);
				}
			}
		}
		lastButtonState = cachedPinB;
	}
}

void initIO(void) {
	PORTD &= ~( (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED) );
	DDRD |= (1 << PD5) | (1 << RED_LED) | (1 << GREEN_LED);
	PORTB = 0xff;
	DDRB = 0x00;
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

void initContoroller(void) {
	initIO();
	initTimer0();
	sei();
}

void main() {
    initContoroller();
	
	initCyclicBuffer(&cyclicBuffer);
	
	while(1) {
		updateButtonState();
		if (findSequenceInCyclicBuffer(&cyclicBuffer, PASSWORD) == true) {
			initCyclicBuffer(&cyclicBuffer);
			for (uint8_t i = 0; i < MAX_REPEAT; i++) {
				playMorsePhrase(morsePhraseToPlay);
			}
		}
	
	}
}








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
const char keysToPress[] = "357";
#define PASS_LEN sizeof(keysToPress) / sizeof(keysToPress[0])

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
	unsigned char currState : 2;
	unsigned char lastState : 2;
	char keyChar;
	uint16_t lastDebounceTime;
} Button;

Button buttons[] = {
	{ PB0, RELEASED, RELEASED, '1', 0x0000},
	{ PB1, RELEASED, RELEASED, '2', 0x0000},
	{ PB2, RELEASED, RELEASED, '3', 0x0000},
	{ PB3, RELEASED, RELEASED, '4', 0x0000},
	{ PB4, RELEASED, RELEASED, '5', 0x0000},
	{ PB5, RELEASED, RELEASED, '6', 0x0000},
	{ PB6, RELEASED, RELEASED, '7', 0x0000},
	{ PB7, RELEASED, RELEASED, '8', 0x0000}
};

volatile unsigned char pinB;    // here we store RAW PINB data
volatile bool flagBtnUpdateValid = false;
uint32_t debounceTime = 100;

// cyclic buffer implementation
typedef struct {
	char buff[PASS_LEN];
	unsigned char pos;
	unsigned char size;
}CyclicBuffer;

// functions to handle cyclic buffer
void initCyclicBuffer(CyclicBuffer *buff);
void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch);
bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq);

CyclicBuffer cyclicBuffer;

void initCyclicBuffer(CyclicBuffer *buff) {
	buff->pos = 0;
	buff->size = PASS_LEN;
}

void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch) {
	*(buff->buff + buff->pos) = ch;
	if (buff->pos == buff->size - 1) {
		buff->pos = 0;
	}
	else {
		buff->pos += 1;
	}
}

bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq) {
	unsigned char i;
	bool flag = true;
	
	for(i = 0; i < buff->size && seq[i] != '\0'; ++i)
	{
		if( *(buff->buff + (buff->pos + i) % (buff->size) ) != seq[i] )
			flag = false;
	}
	
	return flag;
}


ISR (TIMER0_COMPA_vect) {
	msFromReset++;
    pinB = PINB;
    flagBtnUpdateValid = true;
}

void updateButtonState(void) {
    if (flagBtnUpdateValid == true) {
		// low-pass-filter using debounce time
        for (unsigned char i = 0; i < 8; i++) {
			uint8_t buttonTmp = ( pinB & (1 << buttons[i].pin) ) ? RELEASED : PRESSED;
			
			if (buttonTmp != buttons[i].lastState) {
				buttons[i].lastDebounceTime = msFromReset;
			}
			
			if ( (msFromReset - buttons[i].lastDebounceTime ) > debounceTime) {
				if (buttonTmp != buttons[i].currState) {
					buttons[i].currState = buttonTmp;
					if (buttons[i].currState == PRESSED) {
						addCharToCyclicBuffer(&cyclicBuffer, buttons[i].keyChar);
					}
				}
			}
			buttons[i].lastState = buttonTmp;
        }
        flagBtnUpdateValid = false;
    }
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
	
	initCyclicBuffer(&cyclicBuffer);
	
	while(1) {
		updateButtonState();
		if (findSequenceInCyclicBuffer(&cyclicBuffer, keysToPress) == true) {
			for (uint8_t i = 0; i < MAX_REPEAT; i++) {
				playMorsePhrase(morsePhraseToPlay);
			}
		}
	
	}
}








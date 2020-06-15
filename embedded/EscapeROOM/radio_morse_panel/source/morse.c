#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "globals.h"
#include "morse.h"
#include "misc.h"
#include "hw_config.h"

#define UNIT_LENGTH DOT_DURATION_JIFFIES

#define  DOT_LENGTH         (1 * UNIT_LENGTH)
#define DASH_LENGTH         (3 * UNIT_LENGTH)
#define  SHORT_SPACE_LENGTH (1 * UNIT_LENGTH)
#define LETTER_SPACE_LENGTH (3 * UNIT_LENGTH)
#define   WORD_SPACE_LENGTH (7 * UNIT_LENGTH)

typedef enum {CARRIER_OFF, CARRIER_ON} Carrier;

static void playTone(Carrier carrier, uint32_t duration) {
	uint32_t lastJiffies;

	TURN_BUZZER_OFF();
	lastJiffies = getJiffies();
	if (carrier == CARRIER_OFF) {
		while (getJiffies() - lastJiffies < duration) {
			;
		}
	} else {
		uint32_t lastCarrierJiffies;
		uint32_t i = 0;
		while (getJiffies() - lastJiffies < duration) {
			lastCarrierJiffies = getJiffies();
			while (getJiffies() - lastCarrierJiffies < CARRIER_CYCLE_DURATION_JIFFIES) {
				;
			}
			if ( i++ % 2 ) {
				TURN_BUZZER_OFF();
			}
			else {
				TURN_BUZZER_ON();
			}
		}
	}
	TURN_BUZZER_OFF();
}

static void playDot() {
	playTone(CARRIER_ON, DOT_LENGTH);
}

static void playDash() {
	playTone(CARRIER_ON, DASH_LENGTH);
}

static void playShortSpace() {
	playTone(CARRIER_OFF, SHORT_SPACE_LENGTH);
}

static void playLetterSpace() {
	playTone(CARRIER_OFF, LETTER_SPACE_LENGTH);
}

static void playWordSpace() {
	playTone(CARRIER_OFF, WORD_SPACE_LENGTH);
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

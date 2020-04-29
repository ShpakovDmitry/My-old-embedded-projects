#include <stdint.h>
#include <stdbool.h>
#include "globals.h"
#include "misc.h"

#define BTN_1_REL (RELEASED << PB0)
#define BTN_2_REL (RELEASED << PB1)
#define BTN_3_REL (RELEASED << PB2)
#define BTN_4_REL (RELEASED << PB3)
#define BTN_5_REL (RELEASED << PB4)
#define BTN_6_REL (RELEASED << PB5)
#define BTN_7_REL (RELEASED << PB6)
#define BTN_8_REL (RELEASED << PB7)

#define ALL_RELEASED	( BTN_1_REL | BTN_2_REL | BTN_3_REL | BTN_4_REL | \
						BTN_5_REL | BTN_6_REL | BTN_7_REL | BTN_8_REL )

enum ButtonState { PRESSED, RELEASED };

typedef struct {
	unsigned char pin : 3;
	char keyChar;
} Button;

static Button buttons[] = {
	{ PB0, '1'},
	{ PB1, '2'},
	{ PB2, '3'},
	{ PB3, '4'},
	{ PB4, '5'},
	{ PB5, '6'},
	{ PB6, '7'},
	{ PB7, '8'}
};

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
		return 0;
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
		
		if ( deltaTime > DEBOUNCE_TIME ) {
			if (cachedPinB != currButtonState) {
				currButtonState = cachedPinB;
				
				if ( isOneBitSet(~currButtonState) == true ) {
					uint8_t i = getSetBitPosition( ~currButtonState );
					addCharToCyclicBuffer(&pressedButtons, buttons[i].keyChar);
				}
			}
		}
		lastButtonState = cachedPinB;
	}
}

